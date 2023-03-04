#include <Arduino.h>

#define RED_LED 13


void morseSymbol(int holdTime, int releaseTime) {
    digitalWrite(RED_LED, HIGH);
    delay(holdTime);
    digitalWrite(RED_LED, LOW);
    delay(releaseTime);
}

int quarterSecondsToMs(int quarterSeconds) {
    int milliseconds = quarterSeconds * 250;
    return milliseconds;
}

void dash() {
    morseSymbol(quarterSecondsToMs(3), quarterSecondsToMs(1));
}

void dot() {
    morseSymbol(quarterSecondsToMs(1), quarterSecondsToMs(1));
}

void space() {
    delay(quarterSecondsToMs(3));
}

void end() {
    delay(quarterSecondsToMs(7));
}

void setup() {
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, HIGH);
}


void loop() {
    dash();
    space();

    dot();
    dot();
    dot();
    dot();
    space();

    dot();
    space();

    dash();
    dash();
    dot();
    space();

    dot();
    dash();
    space();

    dash();
    dash();
    space();

    dot();
    end();
}
