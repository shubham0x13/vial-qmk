// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Generic board for the NXP MCXC243 (MCX C, Cortex-M0+, 48 MHz, 128 KB flash,
 * 32 KB SRAM).
 *
 * ChibiOS has no MCX C port. The MCXC243 uses the KL2x port of ChibiOS-Contrib
 * with the KL27Zxxx device description: the peripheral registers that
 * ChibiOS-Contrib declares for the KL27Zxxx are at the same offsets on the
 * MCXC243 (checked against NXP's MCXC243 device header), the interrupt vectors
 * match, and the MCXC243 has the same double-buffered I2C (reference manual
 * section 36.5.10) that the KL27Z workaround in the I2C driver is for.
 */
#define BOARD_GENERIC_NXP_MCXC243
#define BOARD_NAME "Generic NXP MCXC243"

#define KL27Zxxx

/*
 * USBx_CTL_RESUME (bit 2 of USBx_CTL, reference manual section 33.5.10) is
 * used for USB remote wakeup. Older ChibiOS-Contrib does not define it for the
 * KL27Zxxx device, so define it here when the device header does not. The value
 * is the one of the other Kinetis headers in ChibiOS-Contrib.
 */
#ifndef USBx_CTL_RESUME
#    define USBx_CTL_RESUME ((uint8_t)0x04)
#endif

/*
 * Flash option byte FOPT (reference manual table 6-2). The ChibiOS-Contrib
 * default (0xFF) selects the ROM bootloader as boot source, so the firmware
 * would never start. 0x3D:
 *   BOOTSRC_SEL   = 00  boot from flash
 *   FAST_INIT     = 1   fast initialization
 *   LPBOOT        = 11  core and system clock divided by 1, bus clock by 2,
 *                       RUN mode out of reset (reference manual 5.5.1)
 *   RESET_PIN_CFG = 1   RESET_b pin dedicated
 *   NMI_DIS       = 1   NMI_b pin and interrupt enabled out of reset; board.c
 *                       turns the pin off
 *   BOOTPIN_OPT   = 0   the ROM bootloader is entered when the NMI_b/BOOTCFG0
 *                       pin (PTA4) is low at reset
 * FSEC keeps the ChibiOS-Contrib default 0x7E: flash unsecured, mass erase
 * enabled, backdoor key disabled.
 */
#define KINETIS_NV_FOPT_BYTE 0x3D

#if !defined(_FROM_ASM_)
#    ifdef __cplusplus
extern "C" {
#    endif
void boardInit(void);
#    ifdef __cplusplus
}
#    endif
#endif /* _FROM_ASM_ */
