//
// Created by derek on 2022-12-11.
//

#include <Arduino.h>
#include <IntDivision.h>

using namespace IntDivision;


void blinkOnce(const unsigned int duration, const uint8_t pin) {
    // Turn LED on for half the duration in ms. LED stays on an extra ms for odd values.
    const unsigned int halfPeriod = duration / 2;
    const unsigned int remainder = duration % 2;
    digitalWrite(pin, HIGH);
    delay(halfPeriod + remainder);
    digitalWrite(pin, LOW);
    delay(halfPeriod);
}

/// Blink the light.
void ditheredBlink(const unsigned int duration, const unsigned int count, const uint8_t pin) {
    DitheredDivider divider = DitheredDivider(duration, count);
    for (unsigned int index = 0; index < count; index++) {
        blinkOnce(divider.quotientSize(index), pin);
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    ditheredBlink(2000, 40, 12);
}

void loop() {
    const int maxReps = 10;
    const uint8_t pinBoard = LED_BUILTIN;
    const uint8_t pin12 = PIN_SPI_MISO;
    for (int i = 0; i <= 40; i++) {
//        digitalWrite(pin12, i % 2 == 0 ? HIGH : LOW);

        const unsigned int base_duration = 2000 - i * 25;
        const unsigned int count = i + 1;
        const unsigned int setRepetitions = max(maxReps - i * 2, 1);

        if (setRepetitions > 1) {
            const unsigned int reducedDuration = static_cast<unsigned long>(base_duration) * count / (count + 1);
            const unsigned int tweenDeltaTime = base_duration - reducedDuration;
            DitheredDivider divider = DitheredDivider(tweenDeltaTime, setRepetitions);
            for (unsigned int j = 0; j < setRepetitions; j++) {
                const unsigned int blinkDuration = base_duration - divider.total(j);
                ditheredBlink(blinkDuration, count, pinBoard);
                delay(i * 25);
            }
        } else {
            ditheredBlink(base_duration, count, pinBoard);
            delay(i * 25);
        }

    }
}