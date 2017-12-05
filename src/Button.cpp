#include <Arduino.h>

#define PREVIOUS 2
#define BACK 3
#define SELECT 4
#define NEXT 5

void initButton() {
    Serial.println("Initializing buttons...");
    pinMode(PREVIOUS, INPUT);
    pinMode(BACK, INPUT);
    pinMode(SELECT, INPUT);
    pinMode(NEXT, INPUT);
}
