#include <Arduino.h>

#define RED_LED 12
#define DAH 300
#define DIT 100
#define ELEMENT_SEPARATOR 100
#define LETTER_SEPARATOR 200
#define SPACE 600
#define END_MESSAGE 1800


void morseSymbol(unsigned long holdTime, unsigned long releaseTime) {
    digitalWrite(RED_LED, HIGH);
    delay(holdTime);
    digitalWrite(RED_LED, LOW);
    delay(releaseTime);
}

template<size_t N>
void printMorseString(char (&message)[N]) {
    for (char element: message) {
        switch (element) {
            case '-':
                morseSymbol(DAH, ELEMENT_SEPARATOR);
                break;
            case '.':
                morseSymbol(DIT, ELEMENT_SEPARATOR);
                break;
            case ' ':
                delay(LETTER_SEPARATOR);
                break;
            case '/':
                delay(SPACE);
                break;
            case '\n':
                delay(END_MESSAGE);
                break;
            default:
                break;
        }
    }
}

void setup() {
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, HIGH);
}


void loop() {
    char message[] = "- .... . / --. .- -- .\n";
    printMorseString(message);
}
