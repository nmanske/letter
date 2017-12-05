#include <Arduino.h>

#define PREV_BTN 2
#define BACK_BTN 3
#define SELECT_BTN 4
#define NEXT_BTN 5

void initButton() {
    Serial.println("Initializing buttons...");
    pinMode(PREV_BTN, INPUT);
    pinMode(BACK_BTN, INPUT);
    pinMode(SELECT_BTN, INPUT);
    pinMode(NEXT_BTN, INPUT);
}
