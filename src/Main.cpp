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

// States for correct screen display and button interpretation
enum Mode { MAIN_MENU, VAULT, BUCKET_LIST };
enum Selection { VAULT_SEL, BUCKET_LIST_SEL };

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
static Mode screen_mode = MAIN_MENU;

// First, second, third, etc button starting at index 0
static Selection mode_selection = VAULT_SEL;

// Current image displayed on screen and pulled from vault array
static int8_t vault_index = 0;

// Disable date info when vault pic loading
static bool loading_image = true;

void setup() {
    Serial.begin(BAUD_RATE);
    while (! Serial);
    initDisplay();
    initButtons(PREVIOUS, BACK, SELECT, NEXT);
}

void loop() {

    uint8_t prev_btn, back_btn, sel_btn, next_btn;

    while(! buttonsNotPressed(prev_btn, back_btn, sel_btn, next_btn));
    do {
        prev_btn = digitalRead(PREVIOUS);
        back_btn = digitalRead(BACK);
        sel_btn = digitalRead(SELECT);
        next_btn = digitalRead(NEXT);
    }
    while (buttonsNotPressed(prev_btn, back_btn, sel_btn, next_btn));

    if (screen_mode == MAIN_MENU) {
        if (sel_btn && mode_selection == VAULT_SEL) {
            screen_mode = VAULT;
            loading_image = true;
        }
        else if (prev_btn || next_btn) {
            updateModeSelection();
            if (mode_selection == VAULT_SEL) mode_selection = BUCKET_LIST_SEL;
            else                             mode_selection = VAULT_SEL;
        }
    }
    else if (screen_mode == VAULT) {
        Image current_image;
        if (back_btn) {
            screen_mode = MAIN_MENU;
            displayMainMenu();
        } else { 
            if (prev_btn) {
                if(--vault_index < 0) vault_index = NUM_VAULT_IMAGES - 1;
                loading_image = true;
            }
            else if (next_btn) {
                if(++vault_index >= NUM_VAULT_IMAGES) vault_index = 0;
                loading_image = true;
            }
            current_image = vault[vault_index];
            setRotateImage(current_image.rotate);
            while (sel_btn && !loading_image) {
                showDate("12/06/2017 @ 18:31");
                sel_btn = digitalRead(SELECT);
            }
            displayImage(current_image.filename);
            loading_image = false;
        }
    }

}
