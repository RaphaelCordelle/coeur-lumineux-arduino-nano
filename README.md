# Arduino LED Heart

DIY embedded LED heart built around an ATmega328P-based Arduino Nano. The project combines point-to-point soldering on perfboard, direct GPIO control, Arduino C/C++ animations, and hands-on hardware bring-up.

## Project status

| Area | Status |
| --- | --- |
| 16-LED perfboard assembly | Built and debugged at component and connection level |
| 16-channel Nano firmware | Implemented; full-system validation after rewiring is work in progress |
| Push-button control | Implemented in firmware; final hardware integration is planned |
| ESP32 bring-up tests | Preserved as separate experiments; not part of the final architecture |

## Technical objectives

- Drive 16 LEDs independently from microcontroller GPIOs.
- Build a compact circuit using hand-soldered point-to-point wiring.
- Implement simple light patterns in embedded C/C++ without external libraries.
- Integrate an active-low push button using the internal pull-up resistor.
- Apply a repeatable hardware/software bring-up and validation process.

## Hardware architecture

- Arduino Nano V3-compatible board, ATmega328P, 5 V / 16 MHz
- 16 pink 5 mm LEDs
- One 220 ohm series resistor per LED
- 50 x 70 mm perfboard
- Common ground for all LED cathodes
- Point-to-point soldered wiring
- Push button between A4 and GND (`INPUT_PULLUP`)
- USB power and firmware upload

Each LED anode is driven from one GPIO through its own resistor. The cathodes share a common ground. The firmware refreshes four interleaved LED groups so that all 16 outputs are not held active simultaneously. Supply current and GPIO loading still need to be checked against measured LED current and the ATmega328P electrical limits.

More detail is available in [hardware/README.md](hardware/README.md).

## Software architecture

The main sketch is [src/led_heart/led_heart.ino](src/led_heart/led_heart.ino). It uses:

- a fixed GPIO mapping stored in a pin array;
- a 16-bit mask to describe the LEDs to display;
- grouped software refresh for multi-LED patterns;
- short-press and long-press button handling with basic debouncing;
- `millis()`, `delay()`, and `delayMicroseconds()` for animation timing;
- an unused analog input as the random seed for the sparkle pattern.

No third-party Arduino library is required.

### Firmware features

Implemented in the sketch:

- progressive fill;
- double heartbeat;
- perimeter chase;
- two-LED random sparkle;
- pulse-based idle breathing effect;
- short press to run the special animation;
- long press to enable or disable the idle animation.

The physical button integration and complete 16-LED regression test are still planned / work in progress.

## Pin mapping

| LED | Arduino Nano pin | LED | Arduino Nano pin |
| ---: | :---: | ---: | :---: |
| 1 | D2 | 9 | D10 |
| 2 | D3 | 10 | D11 |
| 3 | D4 | 11 | D12 |
| 4 | D5 | 12 | D13 |
| 5 | D6 | 13 | A0 |
| 6 | D7 | 14 | A1 |
| 7 | D8 | 15 | A2 |
| 8 | D9 | 16 | A3 |

| Input | Connection | Configuration |
| --- | --- | --- |
| Button | A4 to GND | `INPUT_PULLUP`, active low |

## Build and upload

1. Open `src/led_heart/led_heart.ino` in Arduino IDE.
2. Select **Arduino Nano** and **ATmega328P**.
3. Select the serial port exposed by the board.
4. Compile and upload the sketch.

For some Nano-compatible boards using a CH340 USB-to-serial interface, the correct driver and **ATmega328P (Old Bootloader)** setting may be required.

## Bring-up and debugging

The prototype required iterative electrical and firmware checks:

- continuity tests on ground and point-to-point connections;
- voltage measurements on GPIO and supply rails;
- individual LED tests to confirm polarity and pin mapping;
- inspection for crossed bare conductors and possible short circuits;
- repair or rerouting around perfboard pads damaged during desoldering;
- replacement of long exposed metal links with tinned copper or insulated wire;
- serial-port, CH340, bootloader, and upload troubleshooting;
- partial rewiring followed by repeat validation.

The repeatable procedure and acceptance checks are documented in [docs/bring-up-and-validation.md](docs/bring-up-and-validation.md).

## Skills and tools

`Embedded C/C++` · `ATmega328P` · `GPIO` · `hardware/software integration` · `basic circuit design` · `perfboard prototyping` · `point-to-point wiring` · `soldering and rework` · `multimeter measurements` · `continuity testing` · `fault isolation`

## Possible improvements

- Replace blocking animation delays with a non-blocking state machine for more responsive button input.
- Add a schematic and measured current budget.
- Add a transistor or dedicated LED driver if higher brightness or simultaneous drive is required.
- Add connectorized wiring or a small PCB revision for easier maintenance.
- Capture front, rear-wiring, and validation photographs.
- Add a repeatable automated build check for the Arduino sketch.

## Repository layout

```text
.
|-- src/led_heart/               Main Arduino Nano firmware
|-- hardware/                    Circuit and assembly notes
|-- docs/                        Bring-up and validation record
|-- images/                      Project photographs (planned)
|-- experiments/
|   |-- esp32_arduino_blink/     Early ESP32 Arduino GPIO test
|   `-- esp32-idf-blink/         Early ESP-IDF GPIO test
`-- .gitignore                   Generated files and local tooling exclusions
```
