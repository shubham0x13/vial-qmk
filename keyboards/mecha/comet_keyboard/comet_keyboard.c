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

#ifdef CAPS_WORD_ENABLE
// The caps lock LED also shows Caps Word
bool led_update_kb(led_t led_state) {
    bool res = led_update_user(led_state);

    if (res) {
        led_update_ports(led_state);
        if (is_caps_word_on()) {
            // The caps lock LED is active high
            gpio_write_pin_high(LED_CAPS_LOCK_PIN);
        }
    }

    return res;
}

void housekeeping_task_kb(void) {
    static bool caps_word_was_on;

    if (is_caps_word_on() != caps_word_was_on) {
        caps_word_was_on = is_caps_word_on();
        led_update_kb(host_keyboard_led_state());
    }

    housekeeping_task_user();
}

// Caps Word ends when the host suspends the bus, so the caps lock LED goes off
// with the other indicators (QMK calls this before led_suspend())
void suspend_power_down_kb(void) {
    caps_word_off();

    suspend_power_down_user();
}
#endif // CAPS_WORD_ENABLE
