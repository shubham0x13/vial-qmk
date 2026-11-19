# Comet Keyboard Extension

The keyboard extension of the Mecha Comet, a modular open-source Linux handheld: 46 keys with dome switches and a five-way switch, an NXP MCXC243 microcontroller and USB.

* Keyboard Maintainer: [Shubham Patel](https://github.com/shubham0x13)
* Hardware Supported: Comet Keyboard Extension (NXP MCXC243)
* Hardware Availability: Mecha Systems

Make example for this keyboard (after setting up your build environment):

    make mecha/comet_keyboard:vial

Flashing example for this keyboard:

    make mecha/comet_keyboard:vial:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the Esc dome switch and plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` (Fn + the bottom-right key)
* **Physical reset button**: Short the BOOT pad to ground while plugging in the keyboard

## Default combo

Left click + right click sends the middle mouse button. It is a Vial combo, so it can be changed or deleted in Vial. It is written again after a reset of the keyboard's EEPROM data (a full reset, or a firmware update) and is active from the next power-up.
