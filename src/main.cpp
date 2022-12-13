#include <Arduino.h>

#define DIP01 2
#define DIP02 3
#define DIP03 4

#define RED_LED 12

#define PULSE_WIDTH_MAX 15U


class MSCounter {
    unsigned long lastTime = millis();
    unsigned int count = 0;

public:
    MSCounter() = default;

    void reset() {
        lastTime = millis();
        count = 0;
    }

    void restartCount() {
        count = 0;
    }

    unsigned int getCount() {
        unsigned long now = millis();
        count += now >= lastTime ? now - lastTime : now - (0 - lastTime);
        lastTime = now;

        return count;
    }
};

class HysteresisDivider {
    unsigned int _denominator;
    unsigned int _remainder;

public:
    explicit HysteresisDivider() :
            _denominator(1),
            _remainder(0) {}

    unsigned int divide(unsigned int numerator, unsigned int denominator) {
        if (_denominator != denominator) {
            _remainder = _remainder * denominator / _denominator;
            _denominator = denominator;
        }

        unsigned int quotient = numerator / denominator;
        unsigned int remainder = numerator % denominator;
        if (_remainder + remainder >= denominator) {
            _remainder = (_remainder + remainder) % denominator;
            return quotient + 1;
        }

        _remainder += remainder;
        return quotient;
    }
};

class PWM {
    bool _state = false;
    unsigned int _timeOn = 0;
    unsigned int _timeOff = PULSE_WIDTH_MAX;
    unsigned int _onTrim = 0;
    unsigned int _offTrim = 0;
    MSCounter _counter = MSCounter();

public:

    PWM() = default;

    void setPulseWidth(const unsigned int time) {
        _timeOn = time;
        _timeOff = PULSE_WIDTH_MAX - time;
    }

    bool getState() {
        // If the pulse width is 0 or the max, don't proceed to logic that can change state.
        if (_timeOn == PULSE_WIDTH_MAX || _timeOff == PULSE_WIDTH_MAX) {
            _onTrim = 0;
            _offTrim = 0;
            _state = (_timeOn == PULSE_WIDTH_MAX);
            _counter.reset();
            return _state;
        }

        unsigned int msCount = _counter.getCount();
        if (_state && msCount >= _timeOn + _onTrim) {
            _onTrim = 0;
            _offTrim = (msCount - _timeOn) * _timeOff / _timeOn;
            _state = false;
            _counter.restartCount();
        } else if (!_state && msCount >= _timeOff + _offTrim) {
            _offTrim = 0;
            _onTrim = (msCount - _timeOff) * _timeOn / _timeOff;
            _state = true;
            _counter.restartCount();
        }

        return _state;
    }
};

unsigned int wave(int period, int amplitude) {
    return static_cast<unsigned int>(
            lround(
                    (sin(2 * PI * double(millis()) / double(period)) + 1) * 0.5 * double(amplitude)
            )
    );
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(DIP01, INPUT);
    pinMode(DIP02, INPUT);
    pinMode(DIP03, INPUT);
}

void loop() {
    int cycleTime = 2000;
    int brightness = 0;
    if (digitalRead(DIP01) == HIGH) {
        cycleTime -= 500;
        brightness += 2;
    }
    if (digitalRead(DIP02) == HIGH) {
        cycleTime -= 500;
        brightness += 5;
    }
    if (digitalRead(DIP03) == HIGH) {
        cycleTime -= 500;
        brightness += 8;
    }

    static PWM pwm01 = PWM();
    pwm01.setPulseWidth(wave(cycleTime, PULSE_WIDTH_MAX));
    digitalWrite(LED_BUILTIN, pwm01.getState() ? HIGH : LOW);

    static PWM pwm02 = PWM();
    pwm02.setPulseWidth(brightness);
    digitalWrite(RED_LED, pwm02.getState() ? HIGH : LOW);
}