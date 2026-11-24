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

## Keymaps

* `vial`: QWERTY
* `vial_qwertz`, `vial_azerty`: QWERTZ and AZERTY (currently copies of the QWERTY keymap)

## Firmware version

The firmware version is `usb.device_version` in `keyboard.json`, reported as the USB device release number (bcdDevice) in binary-coded decimal: `0.1.0` is `0x0010` and `1.2.3` is `0x0123`. Keep it equal to the release tag.

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the Esc dome switch and plug in the keyboard
* **Keycode in layout**: Press the key mapped to `QK_BOOT` (Fn + the bottom-right key)
* **Physical reset button**: Short the BOOT pad to ground while plugging in the keyboard

## Default combo

Left click + right click sends the middle mouse button. It is a Vial combo, so it can be changed or deleted in Vial. It is written again after a reset of the keyboard's EEPROM data (a full reset, or a firmware update) and is active from the next power-up.

## Pointing stick

The Pivot Pip pointing stick is read through an ADS1115 on I2C0 (`pivot_pip.c`, a custom pointing device driver). Its signal processing, `lib/pip_core` (`pip_core.c` and `pip_core.h`), is copied unchanged from [mecha-org/mecha-pivot-pip-zephyr](https://github.com/mecha-org/mecha-pivot-pip-zephyr) (`lib/pip_core`, commit `1972e2e`), so the stick behaves as in the ZMK and Zephyr firmware. To update it, copy both files again into `lib/pip_core` from that repository and update the commit here.

To build without the pointing stick (and without QMK's pointing device feature):

    qmk compile -kb mecha/comet_keyboard -km vial -e PIVOT_PIP_ENABLE=no
