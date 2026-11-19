// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include <hal.h>

#if HAL_USE_PAL || defined(__DOXYGEN__)
/**
 * @brief   PAL setup.
 * @details palInit() writes every pad of ports A to E from this table, so any
 *          pin setting left by the ROM bootloader (reference manual chapter 13)
 *          is replaced. Every pad is disabled (mux 0) except the pads whose
 *          reset function is SWD or RESET_b: PTA0 (SWD_CLK), PTA3 (SWD_DIO) and
 *          PTA20 (RESET_b) stay on mux 7. PTA4 (NMI_b) is disabled on purpose:
 *          FOPT[NMI_DIS] = 1 enables the NMI function out of reset, and a low
 *          level on the pin would raise an NMI while the firmware runs. The ROM
 *          bootloader entry through that pin at reset is not affected. QMK sets
 *          up the pins it uses at run time.
 */
const PALConfig pal_default_config = {
    .ports = {
        {
            /* PORTA */
            .port = IOPORT1,
            .pads = {
                PAL_MODE_ALTERNATIVE_7, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_ALTERNATIVE_7, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_ALTERNATIVE_7,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
            },
        },
        {
            /* PORTB */
            .port = IOPORT2,
            .pads = {
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
            },
        },
        {
            /* PORTC */
            .port = IOPORT3,
            .pads = {
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
            },
        },
        {
            /* PORTD */
            .port = IOPORT4,
            .pads = {
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
            },
        },
        {
            /* PORTE */
            .port = IOPORT5,
            .pads = {
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
                PAL_MODE_UNCONNECTED, PAL_MODE_UNCONNECTED,
            },
        },
    },
};
#endif

/**
 * @brief   Early initialization code.
 * @details This initialization must be performed just after stack setup
 *          and before any other initialization.
 */
void __early_init(void) {
    kl2x_clock_init();
}

/**
 * @brief   Board-specific initialization code.
 * @details PAL_MODE_ALTERNATIVE_7 writes only the mux field. Restore the full
 *          reset value of the SWD and RESET_b pads (NXP MCXC243 SVD 1.6):
 *          PTA0 0x706 (pull-down), PTA3 0x703 (pull-up) and PTA20 0x717
 *          (pull-up and passive filter).
 */
void boardInit(void) {
    PORTA->PCR[0]  = 0x706;
    PORTA->PCR[3]  = 0x703;
    PORTA->PCR[20] = 0x717;
}
