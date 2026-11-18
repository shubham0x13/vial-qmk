// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

// Wear-leveling backing store in the program flash of Kinetis-style MCUs with
// the FTFA flash memory module, such as the NXP MCXC243 (reference manual
// chapter 46). The backing store is the flash area that the linker script keeps
// free between __eeprom_workarea_start__ and __eeprom_workarea_end__.

#include <stdbool.h>
#include <ch.h>
#include <hal.h>
#include "_util.h"
#include "timer.h"
#include "wear_leveling.h"
#include "wear_leveling_kinetis_ftfa_config.h"
#include "wear_leveling_internal.h"

STATIC_ASSERT(BACKING_STORE_WRITE_SIZE == 4, "The FTFA programs 4 bytes at a time (Program Longword)");
STATIC_ASSERT((WEAR_LEVELING_BACKING_SIZE) % (WEAR_LEVELING_KINETIS_FTFA_SECTOR_SIZE) == 0, "Backing size must be a multiple of the flash sector size");

extern uint8_t __eeprom_workarea_start__[];
extern uint8_t __eeprom_workarea_end__[];

// Flash command codes (reference manual 46.4.8.2)
#define FTFA_CMD_PROGRAM_LONGWORD 0x06
#define FTFA_CMD_ERASE_FLASH_SECTOR 0x09

#define FTFA_FSTAT_ERRORS (FTFA_FSTAT_RDCOLERR | FTFA_FSTAT_ACCERR | FTFA_FSTAT_FPVIOL)

static uint32_t backing_store_base;

// Launch the command loaded in FCCOB and wait for it to complete. The flash
// cannot be read while a command changes it (reference manual 46.4.6,
// FSTAT[RDCOLERR]), so this runs from RAM and is called with interrupts
// disabled.
static void RESIDENT_IN_RAM(ftfa_launch_and_wait)(void) {
    FTFA->FSTAT = FTFA_FSTAT_CCIF;
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF) == 0) {
    }
}

static bool ftfa_execute(void) {
    chSysLock();
    ftfa_launch_and_wait();
    chSysUnlock();

    // The flash memory controller caches flash data (reference manual chapter
    // 45); clear the cache so that reads return the new contents (errata
    // ERR008595 shows the stale-data effect).
    MCM->PLACR |= MCM_PLACR_CFCC;

    return (FTFA->FSTAT & (FTFA_FSTAT_ERRORS | FTFA_FSTAT_MGSTAT0)) == 0;
}

// Command write sequence (reference manual 46.4.8.1): the previous command has
// completed and the error flags are cleared before the FCCOB registers are
// loaded.
static void ftfa_prepare(uint8_t command, uint32_t address) {
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF) == 0) {
    }
    FTFA->FSTAT  = FTFA_FSTAT_ERRORS;
    FTFA->FCCOB0 = command;
    FTFA->FCCOB1 = (uint8_t)(address >> 16);
    FTFA->FCCOB2 = (uint8_t)(address >> 8);
    FTFA->FCCOB3 = (uint8_t)address;
}

static bool ftfa_erase_sector(uint32_t address) {
    ftfa_prepare(FTFA_CMD_ERASE_FLASH_SECTOR, address);
    return ftfa_execute();
}

static bool ftfa_program_longword(uint32_t address, uint32_t value) {
    ftfa_prepare(FTFA_CMD_PROGRAM_LONGWORD, address);
    // Reference manual 46.4.10.5: the FCCOB7 byte is programmed at the given
    // address and the FCCOB4 byte at address + 3, so that a 32-bit read of the
    // location returns value.
    FTFA->FCCOB4 = (uint8_t)(value >> 24);
    FTFA->FCCOB5 = (uint8_t)(value >> 16);
    FTFA->FCCOB6 = (uint8_t)(value >> 8);
    FTFA->FCCOB7 = (uint8_t)value;
    return ftfa_execute();
}

bool backing_store_init(void) {
    bs_dprintf("Init\n");
    uint32_t start = (uint32_t)__eeprom_workarea_start__;
    uint32_t end   = (uint32_t)__eeprom_workarea_end__;
    if ((start % (WEAR_LEVELING_KINETIS_FTFA_SECTOR_SIZE)) != 0 || (end - start) < (WEAR_LEVELING_BACKING_SIZE)) {
        // The linker script does not reserve enough flash, on a sector boundary
        return false;
    }
    backing_store_base = start;
    return true;
}

bool backing_store_unlock(void) {
    bs_dprintf("Unlock\n");
    return true;
}

bool backing_store_erase(void) {
#ifdef WEAR_LEVELING_DEBUG_OUTPUT
    uint32_t start = timer_read32();
#endif

    bool ret = true;
    for (uint32_t offset = 0; offset < (WEAR_LEVELING_BACKING_SIZE); offset += (WEAR_LEVELING_KINETIS_FTFA_SECTOR_SIZE)) {
        if (!ftfa_erase_sector(backing_store_base + offset)) {
            ret = false;
        }
    }

    bs_dprintf("Backing store erase took %ldms to complete\n", ((long)(timer_read32() - start)));
    return ret;
}

bool backing_store_write(uint32_t address, backing_store_int_t value) {
    uint32_t offset = backing_store_base + address;
    bs_dprintf("Write ");
    wl_dump(offset, &value, sizeof(value));
    // Erased flash reads as ones, the wear-leveling algorithm expects zeros.
    return ftfa_program_longword(offset, ~value);
}

bool backing_store_lock(void) {
    bs_dprintf("Lock  \n");
    return true;
}

bool backing_store_read(uint32_t address, backing_store_int_t *value) {
    uint32_t                   offset = backing_store_base + address;
    const backing_store_int_t *loc    = (const backing_store_int_t *)offset;
    *value                            = ~(*loc);
    bs_dprintf("Read  ");
    wl_dump(offset, value, sizeof(backing_store_int_t));
    return true;
}
