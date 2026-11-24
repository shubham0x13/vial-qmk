// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define VIAL_KEYBOARD_UID \
    { 0xBE, 0xF9, 0x9B, 0xA6, 0xCA, 0x0E, 0x24, 0xCB }

// Unlock combo: Fn + Left Ctrl, the keys that unlock ZMK Studio in the ZMK firmware
#define VIAL_UNLOCK_COMBO_ROWS \
    { 3, 3 }
#define VIAL_UNLOCK_COMBO_COLS \
    { 1, 0 }

// Behaviour of the ZMK firmware: one-shot Fn and Num layers locked by a double
// tap, double-tap Shift for Caps Word, the middle-click combo
#define ONESHOT_TIMEOUT 5000
#define ONESHOT_TAP_TOGGLE 2
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define COMBO_TERM 50
