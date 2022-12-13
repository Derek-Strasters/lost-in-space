#include <Arduino.h>

#define RED_LED 12

void dash() {
    digitalWrite(RED_LED, HIGH);
    delay(850);
    digitalWrite(RED_LED, LOW);
    delay(250);
}

void dot() {
    digitalWrite(RED_LED, HIGH);
    delay(250);
    digitalWrite(RED_LED, LOW);
    delay(250);
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
