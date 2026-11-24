// Copyright 2026 Mecha Systems
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

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
