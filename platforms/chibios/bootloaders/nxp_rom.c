// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include "bootloader.h"

#include <hal.h>

/*
 * NXP ROM bootloader (MCX C parts such as the MCXC243).
 *
 * Reference manual 22.2.5: while RCM_FM[FORCEROM] is not 00, every system reset
 * boots from the ROM. FORCEROM = 01 starts the ROM as if the BOOTCFG0 pin had
 * been held low (RCM_MR[1] set). The board clears FORCEROM again when the
 * firmware starts, so later resets boot the firmware.
 */
__attribute__((weak)) void bootloader_jump(void) {
    RCM->FM = RCM_FM_FORCEROM(1);
    NVIC_SystemReset();
}

__attribute__((weak)) void mcu_reset(void) {
    NVIC_SystemReset();
}
