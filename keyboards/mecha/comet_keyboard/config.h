// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Layer indicator LEDs, active high: on while layer 1 / layer 2 is active
#define LAYER_1_LED_PIN D5
#define LAYER_2_LED_PIN D4

// I2C0 on PTE24 (SCL) and PTE25 (SDA), alternative 5 (reference manual,
// signal multiplexing); open drain is automatic for I2C, the board has 4.7 kOhm
// pull-ups
#define I2C1_SCL_PIN E24
#define I2C1_SDA_PIN E25
#define I2C1_SCL_PAL_MODE PAL_MODE_ALTERNATIVE_5
#define I2C1_SDA_PAL_MODE PAL_MODE_ALTERNATIVE_5

// Pivot Pip pointing stick: settings of the ZMK firmware (the others keep the
// defaults of the mecha,pivot-pip binding, see pivot_pip.c)
#define PIVOT_PIP_FULL_SCALE 80
#define POINTING_DEVICE_INVERT_Y
