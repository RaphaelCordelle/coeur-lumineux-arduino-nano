# Bring-up and validation

This project is validated as a physical prototype rather than through automated unit tests. The checks below turn the original troubleshooting work into a repeatable bring-up procedure.

## 1. Unpowered inspection

1. Disconnect USB power.
2. Confirm LED polarity and the presence of one 220 ohm resistor in every anode path.
3. Inspect the rear of the perfboard for solder bridges, crossed bare conductors, loose joints, and damaged pads.
4. Use continuity mode to confirm the common cathode ground path.
5. Check that 5 V and GND are not shorted.
6. Check adjacent GPIO routes for unintended continuity.

## 2. Board and upload check

1. Connect the Nano without the LED assembly if the board or USB interface is uncertain.
2. Confirm that a serial port appears and that the expected board is selected.
3. If upload synchronization fails on a Nano-compatible board, try the `ATmega328P (Old Bootloader)` processor option.
4. Check the CH340 driver and USB cable if the serial port is missing or unstable.
5. Upload the sketch and confirm that the board restarts normally.

## 3. GPIO and LED validation

1. Power the assembly from USB while monitoring for unexpected heating or resets.
2. Verify the 5 V rail and selected GPIO levels with a multimeter.
3. Exercise the chase pattern and compare each physical LED with the pin map.
4. For a missing LED, check the GPIO, resistor, anode connection, cathode return, and LED polarity in that order.
5. Record and repair any open circuit or unintended short, then repeat the complete sequence rather than testing only the repaired channel.

## 4. Button validation (planned)

1. Confirm A4 reads high with the button released and low when pressed.
2. Verify that a short press starts the special animation.
3. Verify that a press longer than 1.3 seconds toggles the idle animation.
4. Check for repeated or missed events caused by switch bounce.

## Issues encountered and corrective actions

| Observation | Diagnostic approach | Corrective action |
| --- | --- | --- |
| Intermittent LED operation | Continuity test while inspecting joints and routes | Re-soldered or rerouted the connection |
| Long bare leads crossed | Visual inspection and short-circuit check | Replaced exposed links with tinned or insulated wire |
| Pad damaged during desoldering | Continuity check from component lead to next sound node | Bypassed the damaged pad with point-to-point wiring |
| Upload failed or behaved intermittently | Port, cable, CH340, processor, and bootloader checks | Corrected serial setup and retried with the appropriate Nano bootloader option |
| LED did not match expected GPIO | GPIO voltage measurement and one-channel-at-a-time test | Corrected mapping, polarity, or wiring |

## Acceptance criteria

- No short circuit between 5 V and GND with power removed.
- Stable USB connection and repeatable firmware upload.
- Every LED follows the expected GPIO mapping.
- No intermittent response during a gentle wiring inspection.
- Supply and GPIO measurements remain consistent during animation playback.
- Button checks pass after the physical input is integrated.
