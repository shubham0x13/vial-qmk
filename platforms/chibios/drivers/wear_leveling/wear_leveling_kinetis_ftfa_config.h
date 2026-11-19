// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// 4-byte writes: the FTFA Program Longword command
#ifndef BACKING_STORE_WRITE_SIZE
#    define BACKING_STORE_WRITE_SIZE 4
#endif // BACKING_STORE_WRITE_SIZE

// 8kB backing space allocated
#ifndef WEAR_LEVELING_BACKING_SIZE
#    define WEAR_LEVELING_BACKING_SIZE 8192
#endif // WEAR_LEVELING_BACKING_SIZE

// 4kB logical EEPROM
#ifndef WEAR_LEVELING_LOGICAL_SIZE
#    define WEAR_LEVELING_LOGICAL_SIZE ((WEAR_LEVELING_BACKING_SIZE) / 2)
#endif // WEAR_LEVELING_LOGICAL_SIZE

// Flash sector (erase) size
#ifndef WEAR_LEVELING_KINETIS_FTFA_SECTOR_SIZE
#    define WEAR_LEVELING_KINETIS_FTFA_SECTOR_SIZE 1024
#endif // WEAR_LEVELING_KINETIS_FTFA_SECTOR_SIZE
