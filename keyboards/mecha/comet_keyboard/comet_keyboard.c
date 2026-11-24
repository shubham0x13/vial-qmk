// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

void keyboard_pre_init_kb(void) {
    gpio_set_pin_output(LAYER_1_LED_PIN);
    gpio_write_pin_low(LAYER_1_LED_PIN);
    gpio_set_pin_output(LAYER_2_LED_PIN);
    gpio_write_pin_low(LAYER_2_LED_PIN);

    keyboard_pre_init_user();
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);

    gpio_write_pin(LAYER_1_LED_PIN, layer_state_cmp(state, 1));
    gpio_write_pin(LAYER_2_LED_PIN, layer_state_cmp(state, 2));

    return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }

    // One-shot modifiers (Ctrl, Alt, ...) never latch, as in the ZMK firmware:
    // from the second tap on, QMK sees a first tap again and only re-arms the
    // one-shot. One-shot layers keep ONESHOT_TAP_TOGGLE (a double tap latches
    // Fn / Num).
    if (IS_QK_ONE_SHOT_MOD(keycode) && record->event.pressed && record->tap.count > 1) {
        record->tap.count = 1;
    }

    switch (keycode) {
        case CK_EMOJI:
            // GUI + period opens the emoji picker, then Space
            if (record->event.pressed) {
                register_code(KC_LGUI);
                wait_ms(30);
                tap_code(KC_DOT);
                wait_ms(30);
                unregister_code(KC_LGUI);
                wait_ms(30);
                tap_code(KC_SPC);
            }
            return false;
    }

    return true;
}
