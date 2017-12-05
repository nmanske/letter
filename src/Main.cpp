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
typedef struct Image Image;
struct Image {
    String filename;
    bool rotate;
};

// Images to cycle through in the vault
static const Image vault [NUM_VAULT_IMAGES] = {
    {"1-1-1.bmp", true}, {"1-1-2.bmp", true},
    {"1-1-3.bmp", true}, {"parrot.bmp", false}
};

// Current state of screen as defined in SCREEN_STATES
ScreenState screen_state = MAIN_MENU;

// Current image displayed on screen and pulled from vault array
static int8_t vault_index = 0;

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial);
    initDisplay();
    initButtons(PREVIOUS, BACK, SELECT, NEXT);
}

void loop() {

    uint8_t previous_btn, back_btn, select_btn, next_btn;

    do {
        previous_btn = digitalRead(PREVIOUS);
        back_btn = digitalRead(BACK);
        select_btn = digitalRead(SELECT);
        next_btn = digitalRead(NEXT);
    }
    while (buttonsNotPressed(previous_btn, back_btn, select_btn, next_btn));

    if (screen_state == MAIN_MENU) {
        if (previous_btn == HIGH) {
            if(--vault_index < 0) { vault_index = NUM_VAULT_IMAGES - 1; }
            setDisplay(vault[vault_index].filename, vault[vault_index].rotate);
        }
        else if (next_btn == HIGH) {
            if(++vault_index >= NUM_VAULT_IMAGES) { vault_index = 0; }
            setDisplay(vault[vault_index].filename, vault[vault_index].rotate);
        }
    }

}
