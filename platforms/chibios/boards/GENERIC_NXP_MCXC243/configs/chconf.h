// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * The system tick of the Kinetis port runs from SysTick in periodic mode only
 * (no tickless mode), so set a periodic 1 kHz tick, as QMK's Teensy LC
 * (Kinetis KL2x) configuration does.
 */
#define CH_CFG_ST_FREQUENCY 1000
#define CH_CFG_ST_TIMEDELTA 0
#define CH_CFG_TIME_QUANTUM 20

#include_next <chconf.h>
