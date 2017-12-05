#include <Arduino.h>

#include "Display.h"
#include "Button.h"

#define BAUD_RATE 9600

#define PREVIOUS 2
#define BACK 3
#define SELECT 4
#define NEXT 5

// enum ScreenTest { TEST1, TEST2, TEST3, TEST4 };

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);
    initDisplay();
    initButton();
}

void loop() {
    uint8_t previous_state = digitalRead(PREVIOUS);
    uint8_t back_state = digitalRead(BACK);
    uint8_t select_state = digitalRead(SELECT);
    uint8_t next_state = digitalRead(NEXT);
    if (previous_state == HIGH) {
        setDisplay("1-1-1.bmp");
    }
    else if (back_state == HIGH) {
        setDisplay("1-1-2.bmp");
    }
    else if (select_state == HIGH) {
        setDisplay("1-1-3.bmp");
    }
    else if (next_state == HIGH) {
        setDisplay("parrot.bmp");
    }
}
