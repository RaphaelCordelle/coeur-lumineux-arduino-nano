constexpr byte LED_COUNT = 16;
constexpr byte REFRESH_GROUP_COUNT = 4;
constexpr unsigned int GROUP_TIME_US = 1200;

constexpr byte BUTTON_PIN = A4;
constexpr byte RANDOM_SEED_PIN = A5;

constexpr unsigned long LONG_PRESS_MS = 1300;
constexpr unsigned long DEBOUNCE_MS = 25;

const byte LED_PINS[LED_COUNT] = {
  2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13,
  A0, A1, A2, A3
};

bool heartEnabled = true;
bool specialAnimationRequested = false;
bool specialAnimationRunning = false;

bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;

unsigned long lastButtonChangeTime = 0;
unsigned long buttonPressStarted = 0;

bool longPressHandled = false;

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

void checkButton() {
  bool rawState = digitalRead(BUTTON_PIN);
  unsigned long now = millis();

  if (rawState != lastRawButtonState) {
    lastRawButtonState = rawState;
    lastButtonChangeTime = now;
  }

  if ((now - lastButtonChangeTime >= DEBOUNCE_MS) &&
      (rawState != stableButtonState)) {

    stableButtonState = rawState;

    if (stableButtonState == LOW) {
      buttonPressStarted = now;
      longPressHandled = false;
    } else {
      if (!longPressHandled &&
          heartEnabled &&
          !specialAnimationRunning) {
        specialAnimationRequested = true;
      }
    }
  }

  if (stableButtonState == LOW &&
      !longPressHandled &&
      now - buttonPressStarted >= LONG_PRESS_MS) {

    longPressHandled = true;
    heartEnabled = !heartEnabled;
    specialAnimationRequested = false;

    if (!heartEnabled) {
      allOff();
    }
  }
}

bool responsiveDelay(
  unsigned long durationMs,
  bool stopForSpecialAnimation = true
) {
  unsigned long startedAt = millis();

  while (millis() - startedAt < durationMs) {
    checkButton();

    if (!heartEnabled) {
      allOff();
      return false;
    }

    if (stopForSpecialAnimation &&
        specialAnimationRequested &&
        !specialAnimationRunning) {
      allOff();
      return false;
    }

    delay(1);
  }

  return true;
}

bool showMask(
  uint16_t mask,
  unsigned long durationMs,
  bool stopForSpecialAnimation = true
) {
  unsigned long startedAt = millis();

  while (millis() - startedAt < durationMs) {
    for (byte group = 0; group < REFRESH_GROUP_COUNT; ++group) {
      checkButton();

      if (!heartEnabled) {
        allOff();
        return false;
      }

      if (stopForSpecialAnimation &&
          specialAnimationRequested &&
          !specialAnimationRunning) {
        allOff();
        return false;
      }

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
  return true;
}

uint16_t allLedsMask() {
  return static_cast<uint16_t>(
    (1UL << LED_COUNT) - 1UL
  );
}

bool chase(byte repetitions) {
  for (byte repetition = 0; repetition < repetitions; ++repetition) {
    for (byte i = 0; i < LED_COUNT; ++i) {
      showOneLed(i);

      if (!responsiveDelay(75, false)) {
        allOff();
        return false;
      }
    }
  }

  allOff();
  return true;
}

bool fillHeart(bool stopForSpecialAnimation = true) {
  uint16_t mask = 0;

  for (byte i = 0; i < LED_COUNT; ++i) {
    mask |= static_cast<uint16_t>(1UL << i);

    if (!showMask(mask, 90, stopForSpecialAnimation)) {
      return false;
    }
  }

  return showMask(
    allLedsMask(),
    350,
    stopForSpecialAnimation
  );
}

bool heartbeat(bool stopForSpecialAnimation = true) {
  if (!showMask(allLedsMask(), 110, stopForSpecialAnimation)) {
    return false;
  }

  allOff();

  if (!responsiveDelay(90, stopForSpecialAnimation)) {
    return false;
  }

  if (!showMask(allLedsMask(), 180, stopForSpecialAnimation)) {
    return false;
  }

  allOff();

  return responsiveDelay(
    350,
    stopForSpecialAnimation
  );
}

bool sparkle() {
  for (byte i = 0; i < 20; ++i) {
    const byte firstLed = random(LED_COUNT);
    const byte secondLed = random(LED_COUNT);

    const uint16_t mask =
      static_cast<uint16_t>(1UL << firstLed) |
      static_cast<uint16_t>(1UL << secondLed);

    if (!showMask(mask, 80, false)) {
      return false;
    }

    if (!responsiveDelay(35, false)) {
      return false;
    }
  }

  return true;
}

void runSpecialAnimation() {
  specialAnimationRunning = true;
  specialAnimationRequested = false;

  allOff();

  if (!responsiveDelay(150, false)) goto animationEnd;
  if (!chase(2)) goto animationEnd;
  if (!responsiveDelay(120, false)) goto animationEnd;
  if (!fillHeart(false)) goto animationEnd;
  if (!responsiveDelay(250, false)) goto animationEnd;
  if (!heartbeat(false)) goto animationEnd;
  if (!heartbeat(false)) goto animationEnd;
  if (!sparkle()) goto animationEnd;
  if (!showMask(allLedsMask(), 900, false)) goto animationEnd;

animationEnd:
  allOff();
  specialAnimationRunning = false;
  specialAnimationRequested = false;
}

void breathe() {
  for (int pulseTime = 12; pulseTime >= 3; --pulseTime) {
    for (byte pulse = 0; pulse < 3; ++pulse) {
      if (!showMask(allLedsMask(), pulseTime)) {
        return;
      }

      allOff();

      if (!responsiveDelay(pulseTime)) {
        return;
      }
    }
  }

  if (!responsiveDelay(150)) {
    return;
  }

  for (int pulseTime = 3; pulseTime <= 12; ++pulseTime) {
    for (byte pulse = 0; pulse < 3; ++pulse) {
      if (!showMask(allLedsMask(), pulseTime)) {
        return;
      }

      allOff();

      if (!responsiveDelay(pulseTime)) {
        return;
      }
    }
  }

  responsiveDelay(300);
}

void setup() {
  for (byte i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(RANDOM_SEED_PIN));

  delay(500);

  fillHeart(false);
  heartbeat(false);
}

void loop() {
  checkButton();

  if (!heartEnabled) {
    allOff();
    checkButton();
    delay(2);
    return;
  }

  if (specialAnimationRequested) {
    runSpecialAnimation();
    return;
  }

  breathe();
}
