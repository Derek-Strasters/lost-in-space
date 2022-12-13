#include <Arduino.h>

#define RED_LED 12


void morseSymbol(unsigned long holdTime, unsigned long releaseTime) {
    digitalWrite(RED_LED, HIGH);
    delay(holdTime);
    digitalWrite(RED_LED, LOW);
    delay(releaseTime);
}

void dash() {
    morseSymbol(850, 250);
}

void dot() {
    morseSymbol(250, 250);
}

void space() {
    delay(750);
}

void end() {
    delay(1750);
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
