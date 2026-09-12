# ESP32 Arduino GPIO blink

Early bring-up experiment used to validate an external LED and a 220 ohm series resistor on GPIO 23. It is preserved for development traceability and is not part of the Arduino Nano LED-heart architecture.

```text
GPIO 23 -> 220 ohm resistor -> LED anode
                              LED cathode -> GND
```

Open `esp32_arduino_blink.ino` in Arduino IDE and select the appropriate ESP32 board before uploading.
