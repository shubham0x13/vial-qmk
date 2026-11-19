// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define KL2x_MCUCONF

/*
 * HAL driver system settings.
 *
 * MCG-Lite in HIRC mode: the 48 MHz internal oscillator is the system clock
 * and also clocks USB (SIM_SOPT2[USBSRC] = 1, with USB clock recovery, so no
 * crystal is needed). The KL2x port does not write SIM_CLKDIV1 in this mode,
 * so the dividers keep their reset values, which FOPT[LPBOOT] = 11 sets to
 * 1 (core/system, 48 MHz) and 2 (bus/flash, 24 MHz) (reference manual 5.5.1).
 * Same settings as the KL27Z test project of ChibiOS-Contrib.
 */
#define KINETIS_MCGLITE_MODE KINETIS_MCGLITE_MODE_HIRC
#define KINETIS_SYSCLK_FREQUENCY 48000000UL
#define KINETIS_CLKDIV1_OUTDIV1 1

/*
 * GPT driver settings.
 *
 * PIT channel 0 is used by QMK's wait_us() (WAIT_US_TIMER in config.h).
 */
#define KINETIS_GPT_USE_PIT0 TRUE

/*
 * USB driver settings.
 *
 * The ChibiOS-Contrib default priority (5) is meant for the K20x and is not
 * valid on this core (priorities 0 to 3).
 */
#define KINETIS_USB_USE_USB0 TRUE
#define KINETIS_USB_USB0_IRQ_PRIORITY 2
