#include <Arduino.h>

#define DIP01 2
#define DIP02 3
#define DIP03 4

#define BLUE_LED 12
#define GREEN_LED 11
#define RED_LED 10

#define PULSE_WIDTH_MAX 30U


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

class PWM {
    bool _state = false;
    unsigned int _timeOn = 0;
    unsigned int _timeOff = PULSE_WIDTH_MAX;
    MSCounter _counter;

public:

    PWM() : _state(false), _timeOn(0), _timeOff(PULSE_WIDTH_MAX) {
        _counter = MSCounter();
    };

    void setPulseWidth(const unsigned int time) {
        _timeOn = time;
        _timeOff = PULSE_WIDTH_MAX - time;
    }

    bool getState() {
        // If the pulse width is 0 or the max, don't proceed to logic that can change state.
        if (_timeOn == PULSE_WIDTH_MAX || _timeOff == PULSE_WIDTH_MAX) {
            _state = (_timeOn == PULSE_WIDTH_MAX);
            _counter.reset();
            return _state;
        }

        unsigned int msCount = _counter.getCount();
        if (_state && msCount >= _timeOn) {
            _state = false;
            _counter.restartCount();
            return _state;
        }

        if (!_state && msCount >= _timeOff) {
            _state = true;
            _counter.restartCount();
            return _state;
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
//    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}

void loop() {
    static PWM pwm01 = PWM();
    pwm01.setPulseWidth(wave(2011, PULSE_WIDTH_MAX));
    digitalWrite(BLUE_LED, pwm01.getState() ? HIGH : LOW);

    static PWM pwm02 = PWM();
    pwm02.setPulseWidth(wave(3001, PULSE_WIDTH_MAX));
    digitalWrite(GREEN_LED, pwm02.getState() ? HIGH : LOW);

    static PWM pwm03 = PWM();
    pwm03.setPulseWidth(wave(4003, PULSE_WIDTH_MAX));
    digitalWrite(RED_LED, pwm03.getState() ? HIGH : LOW);

//    static PWM pwm04 = PWM();
//    pwm04.setPulseWidth(wave(1000, PULSE_WIDTH_MAX));
//    digitalWrite(LED_BUILTIN, pwm04.getState() ? HIGH : LOW);
}