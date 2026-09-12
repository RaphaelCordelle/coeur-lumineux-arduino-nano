// External LED bring-up test for a standard ESP32 development board.
// GPIO 23 -> 220 ohm resistor -> LED anode; LED cathode -> GND.

constexpr int LED_PIN = 23;
constexpr unsigned long BLINK_INTERVAL_MS = 1000;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(BLINK_INTERVAL_MS);

  digitalWrite(LED_PIN, LOW);
  delay(BLINK_INTERVAL_MS);
}
