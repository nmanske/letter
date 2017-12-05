#include <Arduino.h>

#include "Display.h"
#include "Button.h"

#define BAUD_RATE 9600

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);
    initDisplay();
    initButton();
}

void loop() {
    //setDisplay("test.bmp");
    testDisplay();
    delay(2000);
}
