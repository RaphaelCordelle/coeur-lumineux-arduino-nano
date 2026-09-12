/*

  LED 1..12 -> D2..D13
  LED 13    -> A0
  LED 14    -> A1
  LED 15    -> A2
  LED 16    -> A3

  Button: A4 -> push button -> GND (INPUT_PULLUP)

  Each LED:
  GPIO -> 220 ohm resistor -> LED anode
  LED cathode -> common GND
*/

constexpr byte LED_COUNT = 16;
constexpr byte REFRESH_GROUP_COUNT = 4;
constexpr unsigned int GROUP_TIME_US = 1200;
constexpr byte BUTTON_PIN = A4;
constexpr byte RANDOM_SEED_PIN = A5;
constexpr unsigned long LONG_PRESS_MS = 1300;

const byte LED_PINS[LED_COUNT] = {
  2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, A0, A1, A2, A3
};

bool heartEnabled = true;

void allOff() {
  for (byte i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

void showOneLed(byte index) {
  allOff();

  if (index < LED_COUNT) {
    digitalWrite(LED_PINS[index], HIGH);
  }
}

// Refreshes several LEDs in four interleaved groups.
void showMask(uint16_t mask, unsigned long durationMs) {
  const unsigned long startedAt = millis();

  while (millis() - startedAt < durationMs) {
    for (byte group = 0; group < REFRESH_GROUP_COUNT; ++group) {
      allOff();

      for (byte i = group; i < LED_COUNT; i += REFRESH_GROUP_COUNT) {
        if (mask & static_cast<uint16_t>(1UL << i)) {
          digitalWrite(LED_PINS[i], HIGH);
        }
      }

      delayMicroseconds(GROUP_TIME_US);
    }
  }

  allOff();
}

uint16_t allLedsMask() {
  return static_cast<uint16_t>((1UL << LED_COUNT) - 1UL);
}

void chase(byte repetitions) {
  for (byte repetition = 0; repetition < repetitions; ++repetition) {
    for (byte i = 0; i < LED_COUNT; ++i) {
      showOneLed(i);
      delay(75);
    }
  }

  allOff();
}

void fillHeart() {
  uint16_t mask = 0;

  for (byte i = 0; i < LED_COUNT; ++i) {
    mask |= static_cast<uint16_t>(1UL << i);
    showMask(mask, 90);
  }

  showMask(allLedsMask(), 350);
}

void heartbeat() {
  showMask(allLedsMask(), 110);
  allOff();
  delay(90);

  showMask(allLedsMask(), 180);
  allOff();
  delay(350);
}

void sparkle() {
  for (byte i = 0; i < 20; ++i) {
    const byte firstLed = random(LED_COUNT);
    const byte secondLed = random(LED_COUNT);
    const uint16_t mask =
      static_cast<uint16_t>(1UL << firstLed) |
      static_cast<uint16_t>(1UL << secondLed);

    showMask(mask, 80);
    delay(35);
  }
}

void runSpecialAnimation() {
  allOff();
  delay(150);

  chase(2);
  delay(120);

  fillHeart();
  delay(250);

  heartbeat();
  heartbeat();
  sparkle();

  showMask(allLedsMask(), 900);
  allOff();
}

// Simulates breathing without relying on hardware PWM on every output.
void breathe() {
  for (int pulseTime = 12; pulseTime >= 3; --pulseTime) {
    for (byte pulse = 0; pulse < 3; ++pulse) {
      showMask(allLedsMask(), pulseTime);
      allOff();
      delay(pulseTime);
    }
  }

  delay(150);

  for (int pulseTime = 3; pulseTime <= 12; ++pulseTime) {
    for (byte pulse = 0; pulse < 3; ++pulse) {
      showMask(allLedsMask(), pulseTime);
      allOff();
      delay(pulseTime);
    }
  }

  delay(300);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}

void handleButton() {
  if (!isButtonPressed()) {
    return;
  }

  const unsigned long pressStartedAt = millis();
  delay(30);

  if (!isButtonPressed()) {
    return;
  }

  while (isButtonPressed()) {
    if (millis() - pressStartedAt > LONG_PRESS_MS) {
      heartEnabled = !heartEnabled;
      allOff();

      if (heartEnabled) {
        showMask(allLedsMask(), 300);
      }

      while (isButtonPressed()) {
        delay(10);
      }

      delay(100);
      return;
    }

    delay(10);
  }

  if (heartEnabled) {
    runSpecialAnimation();
  }

  delay(100);
}

void setup() {
  for (byte i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(RANDOM_SEED_PIN));

  delay(500);
  fillHeart();
  heartbeat();
}

void loop() {
  handleButton();

  if (!heartEnabled) {
    allOff();
    delay(10);
    return;
  }

  breathe();
  handleButton();
}
