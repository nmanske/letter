#include <Arduino.h>

void initButtons(uint8_t btn1, uint8_t btn2, uint8_t btn3, uint8_t btn4) {
    Serial.println("Initializing buttons...");
    pinMode(btn1, INPUT);
    pinMode(btn2, INPUT);
    pinMode(btn3, INPUT);
    pinMode(btn4, INPUT);
}

bool buttonsNotPressed(uint8_t btn1, uint8_t btn2, uint8_t btn3, uint8_t btn4) {
    if (btn1 > 0 || btn2 > 0 || btn3 > 0 || btn4 > 0) return false;
    return true;
}
