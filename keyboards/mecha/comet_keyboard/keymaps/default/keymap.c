// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Left click + right click: middle click
const uint16_t PROGMEM middle_click_combo[] = {MS_BTN1, MS_BTN2, COMBO_END};
combo_t                key_combos[]         = {
    COMBO(middle_click_combo, MS_BTN3),
};

enum layers {
    _BASE,
    _FN,
    _NUM,
    _EXTRA,
};

// Rows of the layout: the dome switches and the five-way switch (Right, Up,
// Left, centre, Down), then the four rows of keys.
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,         MS_BTN1, MS_BTN2, KC_RGHT, KC_UP,   KC_LEFT, KC_LGUI, KC_DOWN,
        KC_Q,           KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,           KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_BSPC,
        OSM(MOD_LSFT),  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_ENT,
        OSM(MOD_LCTL),  OSL(_FN), KC_TAB, KC_COMM, KC_SPC,  KC_DOT,  OSM(MOD_LALT),     OSL(_FN), OSL(_NUM)
    ),
    [_FN] = LAYOUT(
        _______,        _______, _______, _______, _______, _______, _______, _______,
        KC_EXLM,        KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
        KC_TILD,        KC_GRV,  KC_COLN, KC_SCLN, KC_LBRC, KC_RBRC, KC_UNDS, _______, KC_BSLS, KC_DEL,
        _______,        _______, KC_LCBR, KC_RCBR, KC_LT,   KC_GT,   KC_DQUO, KC_QUOT, QK_RBT,
        _______,        _______, _______, KC_SLSH, _______, KC_QUES, CK_EMOJI,          _______, QK_BOOT
    ),
    [_NUM] = LAYOUT(
        _______,        _______, _______, _______, _______, _______, _______, _______,
        KC_1,           KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        _______,        _______, _______, KC_UP,   _______, _______, KC_PMNS, KC_PPLS, KC_EQL,  _______,
        _______,        KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______,
        _______,        _______, _______, _______, _______, _______, _______,          _______, _______
    ),
    [_EXTRA] = LAYOUT(
        _______,        _______, _______, _______, _______, _______, _______, _______,
        _______,        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,        _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,        _______, _______, _______, _______, _______, _______, _______, _______,
        _______,        _______, _______, _______, _______, _______, _______,          _______, _______
    )
};
// clang-format on
