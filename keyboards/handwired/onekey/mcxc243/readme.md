# MCXC243 onekey

Bring-up keyboard for the NXP MCXC243 (MCX C, Cortex-M0+, 48 MHz, 128 KB flash, 32 KB SRAM), with the pins of the Mecha Comet keyboard extension:

* The key is the RIGHT switch of the five-way switch, between row pin *D1* and column pin *C2*. The matrix is `COL2ROW`: the row pin is driven low and the column pin, which has a pull-up on the board, is read.
* The caps lock indicator LED is on *D6* (on when the pin is high).

The MCU runs from its 48 MHz internal oscillator (MCG-Lite, HIRC). USB uses the same oscillator with USB clock recovery, so no crystal is needed.

## Flashing

The MCXC243 has an NXP ROM bootloader that shows up on USB as `15a2:0073`. Hold the BOOT pad (PTA4) low while resetting or plugging in to enter it, then flash with NXP's `blhost`:

```sh
blhost -u 0x15a2,0x0073 -- flash-erase-all
blhost -u 0x15a2,0x0073 -- write-memory 0x0 handwired_onekey_mcxc243_default.bin
blhost -u 0x15a2,0x0073 -- reset
```
