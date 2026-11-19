// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Program flash size of the MCXC243 (128 KB), for the embedded_flash
// wear-leveling driver
#ifndef WEAR_LEVELING_EFL_FLASH_SIZE
#    define WEAR_LEVELING_EFL_FLASH_SIZE (128 * 1024)
#endif

// wait_us() on a hardware timer (PIT channel 0, see mcuconf.h): without it,
// wait_us() sleeps for at least one system tick (1 ms) on this core
#ifndef WAIT_US_TIMER
#    define WAIT_US_TIMER GPTD1
#endif
