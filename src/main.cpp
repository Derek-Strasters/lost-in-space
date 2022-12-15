#include <Arduino.h>

#define RED_LED            12

#define BASE_TIME          70

#define DAH                ( BASE_TIME * 3 )
#define DIT                ( BASE_TIME )
#define ELEMENT_SEPARATOR  ( BASE_TIME )
#define LETTER_SEPARATOR   ( BASE_TIME * 2 ) // Should be 3x BASE, assumed to be preceded by ELEMENT_SEPARATOR.
#define SPACE              ( BASE_TIME * 5 ) // Should be 7x BASE, assumed to have element separators on either side.
#define END_MESSAGE        ( BASE_TIME * 14 )


void morseSymbol(unsigned long holdTime, unsigned long releaseTime) {
    digitalWrite(RED_LED, HIGH);
    delay(holdTime);
    digitalWrite(RED_LED, LOW);
    delay(releaseTime);
}

template<size_t N>
void printMorseChars(__attribute__((unused)) const char (&message)[N]) {
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
    printStringInMorse("Did you ever hear the tragedy of Darth Plagueis The Wise? I thought not. "
                       "It’s not a story the Jedi would tell you. It’s a Sith legend. "
                       "Darth Plagueis was a Dark Lord of the Sith, so powerful and so wise he "
                       "could use the Force to influence the midichlorians to create life… "
                       "He had such a knowledge of the dark side that he could even keep the ones "
                       "he cared about from dying. The dark side of the Force is a pathway to many "
                       "abilities some consider to be unnatural. He became so powerful… "
                       "the only thing he was afraid of was losing his power, which eventually, "
                       "of course, he did. Unfortunately, he taught his apprentice everything he knew, "
                       "then his apprentice killed him in his sleep. Ironic. He could save others "
                       "from death, but not himself.");
}
