// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// GPT driver for the microsecond wait_us() (WAIT_US_TIMER in config.h)
#ifndef HAL_USE_GPT
#    define HAL_USE_GPT TRUE
#endif

#include_next <halconf.h>
