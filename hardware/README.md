# Hardware notes

## Functional diagram

```text
USB 5 V
  |
Arduino Nano / ATmega328P
  |-- GPIO D2..D13, A0..A3 -- 220 ohm -- LED anodes
  |                                      LED cathodes -- common GND
  |
  `-- A4 -- push button -- GND
```

The button input uses the ATmega328P internal pull-up. It therefore reads `HIGH` when released and `LOW` when pressed.

## Bill of materials

| Quantity | Component | Notes |
| ---: | --- | --- |
| 1 | Arduino Nano V3-compatible board | ATmega328P, 5 V / 16 MHz |
| 16 | 5 mm pink LED | Verify polarity before soldering |
| 16 | 220 ohm resistor | One series resistor per LED |
| 1 | Push button | Planned mode / animation input |
| 1 | 50 x 70 mm perfboard | Point-to-point assembly |
| As required | Tinned copper and insulated wire | Signal and common-ground routing |

## Assembly decisions

- Every LED is connected to a dedicated GPIO; there is no external shift register or LED driver.
- All cathodes share a common ground return.
- Each LED has its own current-limiting resistor.
- Exposed conductors should be kept short and separated. Insulated wire is preferred where routes cross.
- Mechanically weak or heat-damaged pads should be bypassed with a clean point-to-point connection to a sound pad.

## Electrical validation

The 220 ohm resistor value alone does not determine whether the complete load is within the microcontroller limits. Actual current depends on the USB supply, LED forward voltage, wiring, and duty cycle. Measure representative LED current and verify both per-pin and aggregate GPIO limits against the ATmega328P datasheet before extended operation.

The current firmware refreshes four interleaved groups, with at most four selected LEDs driven during one refresh slot. This reduces simultaneous GPIO loading but does not replace current measurement.

## Planned documentation

- schematic or annotated wiring diagram;
- front-side component photograph;
- rear-side point-to-point wiring photograph;
- measured LED current and 5 V rail voltage.
