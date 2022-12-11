#include <Arduino.h>


void blinkOnce(const unsigned int duration, const uint8_t pin) {
    // Turn LED on for half the duration in ms. LED stays on an extra ms for odd values.
    const unsigned int halfPeriod = duration / 2;
    const unsigned int remainder = duration % 2;
    digitalWrite(pin, HIGH);
    delay(halfPeriod + remainder);
    digitalWrite(pin, LOW);
    delay(halfPeriod);
}

/**
 * Divide an integer into integer parts that will sum to the original.
 *
 * Say the integer 1,000 is divided by 60 - the integer solution is 16.
 * However 16 * 60 = 960 which shows that if the quotients are iteratively
 * added (16 + 16 + ... ) = 960 the sum will fall short.
 *
 * DitheredDivider provides a quotientSize that can be used when iterating
 * using the quotientSize method to provide integer quotients given their
 * index that are periodically larger as needed such that when summed
 * together they will produce the original numerator:
 * (16 + 17 + 17 + 16 + 17 + 17 + 16 + ...) = 1,000
 */
class DitheredDivider {
public:
    const unsigned int denominator;
    const unsigned int quotient;
    const unsigned int remainder;
    const unsigned int ditherThreshold;

    explicit DitheredDivider(const unsigned int numerator, const unsigned int denominator) :
            denominator(denominator),
            quotient(numerator / denominator),
            remainder(numerator % denominator),
            ditherThreshold(denominator - remainder) {
    }

    unsigned int quotientSize(const unsigned int index) const {
        const unsigned int plus = index + 1;
        const unsigned int dither = (plus * remainder) % denominator >= ditherThreshold;
        return quotient + dither;
    }

    unsigned int total(const unsigned int index) const {
        const unsigned int plus = index + 1;
        return plus * quotient + (plus * remainder) / denominator;
    }
};

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