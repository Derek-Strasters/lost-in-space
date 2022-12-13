#include <Arduino.h>

#define RED_LED 12
#define DAH 600
#define DIT 200
#define ELEMENT_SEPARATOR 200
#define LETTER_SEPARATOR 600 // Should be 3x DIT, but assumed to be preceded by element separator.
#define SPACE 1000 // Should be 7x DIT, but assumed to have element separators on either side.
#define END_MESSAGE 2000


void morseSymbol(unsigned long holdTime, unsigned long releaseTime) {
    digitalWrite(RED_LED, HIGH);
    delay(holdTime);
    digitalWrite(RED_LED, LOW);
    delay(releaseTime);
}

template<size_t N>
void printMorseChars(const char (&message)[N]) {
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

template<size_t N>
void printStringInMorse(__attribute__((unused)) const char (&message)[N]) {
    const char lettersMap[37][7] = {
            ".- ", "-... ", "-.-. ", "-.. ",
            ". ", "..-. ", "--. ", ".... ",
            ".. ", ".--- ", "-.- ", ".-.. ",
            "-- ", "-. ", "--- ", ".--. ",
            "--.- ", ".-. ", "... ", "- ",
            "..- ", "...- ", ".-- ", "-..- ",
            "-.-- ", "--.. ", "----- ", ".---- ",
            "..--- ", "...-- ", "....- ", "..... ",
            "-.... ", "--... ", "---.. ", "----. ",
            "/ "
    };

    for (char symbol: message) {
        if (symbol >= 97 && symbol <= 122) { // Convert lower case ascii to an index in the above array.
            printMorseChars(lettersMap[symbol - 97]);
        } else if (symbol >= 65 && symbol <= 90) { // Convert lower case ascii to an index in the above array.
            printMorseChars(lettersMap[symbol - 65]);
        } else if (symbol >= 48 && symbol <= 57) { // Convert numeral ascii to an index in the above array.
            printMorseChars(lettersMap[symbol - 22]);
        } else if (symbol == 32 || symbol == 44) { // Convert space and comma.
            printMorseChars(lettersMap[36]);
        } else if (symbol == 13 || symbol == 10 || symbol == 46) { // Convert newline, return line, and period.
            printMorseChars("\n");
        }
    }
}

void setup() {
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, HIGH);
}


void loop() {
    printStringInMorse("SOS. Hey come buy me 1 taco.");
}
