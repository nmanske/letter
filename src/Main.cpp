#include <Arduino.h>

#include "Display.h"
#include "Button.h"

#define BAUD_RATE 9600

#define NUM_SLIDES 4

#define PREVIOUS 2
#define BACK 3
#define SELECT 4
#define NEXT 5

static int8_t current_slide = 0;

typedef struct Image Image;
struct Image {
    String filename;
    bool rotate;
};

static const Image images [NUM_SLIDES] = {
    {"1-1-1.bmp", true}, {"1-1-2.bmp", true},
    {"1-1-3.bmp", true}, {"parrot.bmp", false}
};

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
        Serial.println("Before: " + current_slide);
        if(--current_slide < 0) {
            current_slide = NUM_SLIDES - 1;
        }
        Serial.println("After: " + current_slide + '\n');
        setDisplay(images[current_slide].filename, images[current_slide].rotate);
    }
    else if (next_state == HIGH) {
        if(++current_slide >= NUM_SLIDES) {
            current_slide = 0;
        }
        setDisplay(images[current_slide].filename, images[current_slide].rotate);
    }
    
}
