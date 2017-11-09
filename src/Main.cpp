#include <Arduino.h>

#include "Display.h"

#define BAUD_RATE 9600

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);
    initDisplay();
}

void loop() {
    
}
