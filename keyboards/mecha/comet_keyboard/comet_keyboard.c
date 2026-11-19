// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#ifdef VIA_ENABLE
#    include "via.h"
#endif
#ifdef VIAL_ENABLE
#    include "dynamic_keymap.h"
#    include "vial.h"
#endif

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

#ifdef VIAL_COMBO_ENABLE
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

// Bootloader configuration area (BCA) of the ROM bootloader, at 0x3C0 (MCXC243
// reference manual, section 13.3.2, table 13-3). Only the USB strings are set;
// the other fields keep their defaults (0xFF: all peripherals, 5 s detection
// timeout, VID/PID 15A2:0073).
#define BCA_USB_STRING(name, str)                           \
    static const struct {                                   \
        uint8_t  bLength;                                   \
        uint8_t  bDescriptorType;                           \
        uint16_t bString[sizeof(str) / sizeof(str[0]) - 1]; \
    } __attribute__((packed)) name = {sizeof(name), 0x03, str}

static const uint8_t bca_usb_language_ids[] = {4, 0x03, 0x09, 0x04};
BCA_USB_STRING(bca_usb_manufacturer, u"Mecha Systems");
BCA_USB_STRING(bca_usb_product, u"Comet Keyboard Bootloader");
BCA_USB_STRING(bca_usb_serial, u"0");

static const uint8_t *const bca_usb_string_table[] = {
    bca_usb_language_ids,
    (const uint8_t *)&bca_usb_manufacturer,
    (const uint8_t *)&bca_usb_product,
    (const uint8_t *)&bca_usb_serial,
};

static const uint8_t bca_usb_string_sizes[] = {
    sizeof(bca_usb_language_ids),
    sizeof(bca_usb_manufacturer),
    sizeof(bca_usb_product),
    sizeof(bca_usb_serial),
};

// String table pointed to by usbStringsPointer (reference manual section
// 13.4.3.2, g_languages)
static const struct {
    const uint8_t *language_ids;
    uint8_t        language_ids_size;
    struct {
        uint16_t              language_id;
        const uint8_t *const *strings;
        const uint8_t        *string_sizes;
    } language;
} bca_usb_strings = {
    .language_ids      = bca_usb_language_ids,
    .language_ids_size = sizeof(bca_usb_language_ids),
    .language =
        {
            .language_id  = 0x0409,
            .strings      = bca_usb_string_table,
            .string_sizes = bca_usb_string_sizes,
        },
};

typedef struct __attribute__((packed)) {
    uint8_t     tag[4];
    uint8_t     reserved_04[12];
    uint8_t     enabled_peripherals;
    uint8_t     i2c_slave_address;
    uint16_t    peripheral_detection_timeout;
    uint16_t    usb_vid;
    uint16_t    usb_pid;
    const void *usb_strings;
    uint8_t     clock_flags;
    uint8_t     clock_divider;
    uint8_t     reserved_1e[34];
} bootloader_config_t;

_Static_assert(sizeof(bootloader_config_t) == 0x40, "BCA size");
_Static_assert(offsetof(bootloader_config_t, enabled_peripherals) == 0x10, "BCA enabledPeripherals offset");
_Static_assert(offsetof(bootloader_config_t, usb_vid) == 0x14, "BCA usbVid offset");
_Static_assert(offsetof(bootloader_config_t, usb_strings) == 0x18, "BCA usbStringsPointer offset");
_Static_assert(offsetof(bootloader_config_t, clock_flags) == 0x1C, "BCA clockFlags offset");

__attribute__((section(".bootloader_config"), used)) static const bootloader_config_t bootloader_config = {
    .tag                          = {'k', 'c', 'f', 'g'},
    .reserved_04                  = {[0 ... 11] = 0xFF},
    .enabled_peripherals          = 0xFF,
    .i2c_slave_address            = 0xFF,
    .peripheral_detection_timeout = 0xFFFF,
    .usb_vid                      = 0x15A2,
    .usb_pid                      = 0x0073,
    .usb_strings                  = &bca_usb_strings,
    .clock_flags                  = 0xFF,
    .clock_divider                = 0xFF,
    .reserved_1e                  = {[0 ... 33] = 0xFF},
};
