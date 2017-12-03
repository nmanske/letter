#include <Arduino.h>

#define LEFT_BTN 2
#define RIGHT_BTN 3
#define JUMP_BTN 4

void initButton() {
    Serial.println("Initializing buttons...");
    pinMode(LEFT_BTN, INPUT);
    pinMode(RIGHT_BTN, INPUT);
    pinMode(JUMP_BTN, INPUT);
}
