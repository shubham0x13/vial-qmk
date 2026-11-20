// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

// Mecha Pivot Pip pointing stick, as a QMK custom pointing device driver.
//
// The X and Y axes of the stick are read through AIN0 and AIN1 of an ADS1115
// and processed by pip_core (pip_core.c, copied unchanged from
// mecha-org/mecha-pivot-pip-zephyr), as in the Zephyr driver of that
// repository used by the ZMK and gamepad firmware. The ADS1115 is configured
// as by Zephyr's ADS1x1x driver with the settings of the ZMK firmware.
//
// Conversions are not waited for: each call of
// pointing_device_driver_get_report() moves the sampling one step on, so the
// matrix scan is not delayed by the about 16 ms one sample of both axes takes.

#include QMK_KEYBOARD_H
#include "i2c_master.h"
#include "pointing_device_internal.h"
#include "pip_core.h"

#define ADS1115_ADDRESS (0x48 << 1)
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG 0x01

// Config register fields
#define ADS1115_CONFIG_OS (1U << 15)
#define ADS1115_CONFIG_MUX(x) ((x) << 12)
#define ADS1115_CONFIG_PGA(x) ((x) << 9)
#define ADS1115_CONFIG_MODE_SINGLE_SHOT (1U << 8)
#define ADS1115_CONFIG_DR(x) ((x) << 5)
#define ADS1115_CONFIG_COMP_MODE (1U << 4)

#define ADS1115_MUX_AIN0 4
#define ADS1115_MUX_AIN1 5
#define ADS1115_PGA_2048 2
#define ADS1115_DR_128 4

// Start a single-shot conversion of one single-ended input with the settings
// of Zephyr's ADS1x1x driver: gain 1 (+-2.048 V), 128 SPS, comparator mode bit
#define ADS1115_CONFIG(mux) (ADS1115_CONFIG_OS | ADS1115_CONFIG_MUX(mux) | ADS1115_CONFIG_PGA(ADS1115_PGA_2048) | ADS1115_CONFIG_MODE_SINGLE_SHOT | ADS1115_CONFIG_DR(ADS1115_DR_128) | ADS1115_CONFIG_COMP_MODE)

// Conversion time at 128 SPS (7813 us in Zephyr's driver), in timer
// milliseconds; after it the OS bit is polled until the conversion is done
#define ADS1115_CONVERSION_MS 8

// Settings, with the defaults of the mecha,pivot-pip devicetree binding
#ifndef PIVOT_PIP_FULL_SCALE
#    error "PIVOT_PIP_FULL_SCALE must be set: deflection at full travel, in raw counts"
#endif
#ifndef PIVOT_PIP_POLL_PERIOD_MS
#    define PIVOT_PIP_POLL_PERIOD_MS 8
#endif
#ifndef PIVOT_PIP_MAX_SPEED
#    define PIVOT_PIP_MAX_SPEED 1000
#endif
#ifndef PIVOT_PIP_FILTER_TIME_CONSTANT_MS
#    define PIVOT_PIP_FILTER_TIME_CONSTANT_MS 12
#endif
#ifndef PIVOT_PIP_DEADZONE_PERCENT
#    define PIVOT_PIP_DEADZONE_PERCENT 6
#endif
#ifndef PIVOT_PIP_RECENTER_WINDOW_PERCENT
#    define PIVOT_PIP_RECENTER_WINDOW_PERCENT 3
#endif
#ifndef PIVOT_PIP_RECENTER_DELAY_MS
#    define PIVOT_PIP_RECENTER_DELAY_MS 500
#endif
#ifndef PIVOT_PIP_RECENTER_LIMIT_PERCENT
#    define PIVOT_PIP_RECENTER_LIMIT_PERCENT 5
#endif
#ifndef PIVOT_PIP_I2C_TIMEOUT_MS
#    define PIVOT_PIP_I2C_TIMEOUT_MS 10
#endif

// Longest time between two samples given to pip_core, as in the Zephyr driver
#define PIVOT_PIP_MAX_ELAPSED_US 100000

typedef enum {
    PIVOT_PIP_START,
    PIVOT_PIP_CONVERT_X,
    PIVOT_PIP_CONVERT_Y,
} pivot_pip_state_t;

static struct pip_config pip;
static struct pip_axis   axis_x;
static struct pip_axis   axis_y;
static int32_t           remainder_x;
static int32_t           remainder_y;
static bool              pip_ready;

static pivot_pip_state_t state;
static uint32_t          poll_start;
static uint32_t          conversion_start;
static uint32_t          last_sample;
static int32_t           raw_x;

static bool ads1115_start(uint8_t mux) {
    uint16_t config  = ADS1115_CONFIG(mux);
    uint8_t  data[2] = {config >> 8, config & 0xFF};

    conversion_start = timer_read32();
    return i2c_write_register(ADS1115_ADDRESS, ADS1115_REG_CONFIG, data, sizeof(data), PIVOT_PIP_I2C_TIMEOUT_MS) == I2C_STATUS_SUCCESS;
}

// Reads the conversion of the input being converted. Returns false while the
// conversion is not done or on an I2C error (*error set).
static bool ads1115_read(int32_t *raw, bool *error) {
    uint8_t data[2];

    *error = false;
    if (timer_elapsed32(conversion_start) < ADS1115_CONVERSION_MS) {
        return false;
    }
    if (i2c_read_register(ADS1115_ADDRESS, ADS1115_REG_CONFIG, data, sizeof(data), PIVOT_PIP_I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
        *error = true;
        return false;
    }
    if (((data[0] << 8) & ADS1115_CONFIG_OS) == 0) {
        return false;
    }
    if (i2c_read_register(ADS1115_ADDRESS, ADS1115_REG_CONVERSION, data, sizeof(data), PIVOT_PIP_I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
        *error = true;
        return false;
    }

    // As Zephyr's ADS1x1x driver: the signed result scaled to 15 bits, stored
    // as a 16-bit sample, which the Pivot Pip driver reads as unsigned for a
    // single-ended input
    *raw = (uint16_t)((int16_t)((data[0] << 8) | data[1]) / 2);
    return true;
}

void pointing_device_driver_init(void) {
    const struct pip_params params = {
        .full_scale              = PIVOT_PIP_FULL_SCALE,
        .filter_time_constant_ms = PIVOT_PIP_FILTER_TIME_CONSTANT_MS,
        .deadzone_percent        = PIVOT_PIP_DEADZONE_PERCENT,
        .recenter_window_percent = PIVOT_PIP_RECENTER_WINDOW_PERCENT,
        .recenter_limit_percent  = PIVOT_PIP_RECENTER_LIMIT_PERCENT,
        .recenter_delay_ms       = PIVOT_PIP_RECENTER_DELAY_MS,
    };

    i2c_init();

    pip_ready = pip_config_init(&pip, &params) == 0;
    pip_axis_init(&axis_x);
    pip_axis_init(&axis_y);
    remainder_x = 0;
    remainder_y = 0;

    state       = PIVOT_PIP_START;
    last_sample = timer_read32();
    poll_start  = last_sample - PIVOT_PIP_POLL_PERIOD_MS;
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    int32_t  raw_y;
    uint32_t now;
    uint32_t elapsed_us;
    int32_t  x;
    int32_t  y;
    bool     error;

    if (!pip_ready) {
        return mouse_report;
    }

    switch (state) {
        case PIVOT_PIP_START:
            if (timer_elapsed32(poll_start) < PIVOT_PIP_POLL_PERIOD_MS) {
                break;
            }
            poll_start = timer_read32();
            if (ads1115_start(ADS1115_MUX_AIN0)) {
                state = PIVOT_PIP_CONVERT_X;
            }
            break;

        case PIVOT_PIP_CONVERT_X:
            if (!ads1115_read(&raw_x, &error)) {
                if (error) {
                    state = PIVOT_PIP_START;
                }
                break;
            }
            state = ads1115_start(ADS1115_MUX_AIN1) ? PIVOT_PIP_CONVERT_Y : PIVOT_PIP_START;
            break;

        case PIVOT_PIP_CONVERT_Y:
            if (!ads1115_read(&raw_y, &error)) {
                if (error) {
                    state = PIVOT_PIP_START;
                }
                break;
            }
            state = PIVOT_PIP_START;

            // Speed and timing follow the measured time, as in the Zephyr driver
            now         = timer_read32();
            elapsed_us  = MIN(TIMER_DIFF_32(now, last_sample) * 1000U, PIVOT_PIP_MAX_ELAPSED_US);
            last_sample = now;

            x = pip_axis_update(&pip, &axis_x, raw_x, elapsed_us);
            y = pip_axis_update(&pip, &axis_y, raw_y, elapsed_us);
            pd_dprintf("Pivot Pip: raw x %ld y %ld, deflection x %ld y %ld\n", (long)raw_x, (long)raw_y, (long)x, (long)y);

            mouse_report.x = pip_motion(&remainder_x, x, PIVOT_PIP_MAX_SPEED, elapsed_us);
            mouse_report.y = pip_motion(&remainder_y, y, PIVOT_PIP_MAX_SPEED, elapsed_us);
            break;
    }

    return mouse_report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return 0;
}

void pointing_device_driver_set_cpi(uint16_t cpi) {}
