// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#ifdef VIAL_ENABLE
#    include "vial.h"
#endif

// VIAL_COMBO_ENABLE is defined by vial.h
#ifdef VIAL_COMBO_ENABLE
#    include "dynamic_keymap.h"
#    include "via.h"

// Default Vial combo: the left and right mouse buttons together send the
// middle mouse button. Vial clears its combos whenever its EEPROM data is
// reset, so the default is written again after each reset: from
// eeconfig_init_kb() after a full EEPROM reset, and from
// keyboard_post_init_kb() when via_init() found the VIA data invalid (for
// example after a firmware update).
static bool via_eeprom_was_invalid;

static void set_default_combos(void) {
    const vial_combo_entry_t middle_click = {
        .input  = {MS_BTN1, MS_BTN2},
        .output = MS_BTN3,
    };

    dynamic_keymap_set_combo(0, &middle_click);
}

void via_init_kb(void) {
    via_eeprom_was_invalid = !via_eeprom_is_valid();
}

void eeconfig_init_kb(void) {
    set_default_combos();

    eeconfig_init_user();
}

void keyboard_post_init_kb(void) {
    if (via_eeprom_was_invalid) {
        set_default_combos();
    }

    keyboard_post_init_user();
}
#endif // VIAL_COMBO_ENABLE
