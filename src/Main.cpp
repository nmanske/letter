#include <Arduino.h>

#include "Display.h"
#include "Button.h"

// SPI communication rate in bits per second
#define BAUD_RATE 9600

// Button
#define PREVIOUS 2
#define BACK 3
#define SELECT 4
#define NEXT 5

// Useful nums for array sizing and loops
#define NUM_VAULT_IMAGES 4

// State for correct screen display and button interpretation
enum ScreenState { MAIN_MENU, VAULT_MENU };

// Format of each image loaded from SD card
struct Image {
    String filename;
    bool rotate;
};

// Images to cycle through in the vault
static const Image vault [NUM_VAULT_IMAGES] = {
    {"1-1-1.bmp", true}, {"1-1-2.bmp", true},
    {"1-1-3.bmp", true}, {"parrot.bmp", false}
};

// ************************************************************************

// Current state of screen as defined in SCREEN_STATES
static ScreenState screen_state = MAIN_MENU;

// Current image displayed on screen and pulled from vault array
static int8_t vault_index = 0;

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);
    initDisplay();
    initButtons(PREVIOUS, BACK, SELECT, NEXT);
}

void loop() {

    uint8_t prev_btn, back_btn, sel_btn, next_btn;

    do {
        prev_btn = digitalRead(PREVIOUS);
        back_btn = digitalRead(BACK);
        sel_btn = digitalRead(SELECT);
        next_btn = digitalRead(NEXT);
    }
    while (buttonsNotPressed(prev_btn, back_btn, sel_btn, next_btn));

    if (screen_state == MAIN_MENU) {
        Image current_img;
        if (prev_btn) {
            if(--vault_index < 0) vault_index = NUM_VAULT_IMAGES - 1;
        }
        else if (next_btn) {
            if(++vault_index >= NUM_VAULT_IMAGES) vault_index = 0;
        }
        current_img = vault[vault_index];
        while (sel_btn) {
            showDate("12/06/2017 @ 18:31", current_img.rotate);
            sel_btn = digitalRead(SELECT);
        }
        displayImage(current_img.filename, current_img.rotate);
    }

}
