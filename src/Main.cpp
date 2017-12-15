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
    String time;
    bool rotate;
};

// Images to cycle through in the vault
static const Image vault [NUM_VAULT_IMAGES] = {
    {"20171231.bmp", "18:35", true}, {"20111120.bmp", "01:11", true},
    {"19940111.bmp", "12:00", true}, {"18870704.bmp", "00:01", false}
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
    Image current_image;

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
        } else if (prev_btn || next_btn) {
            updateModeSelection();
            delay(250);
            if (mode_selection == VAULT_SEL) mode_selection = BUCKET_LIST_SEL;
            else                             mode_selection = VAULT_SEL;
        }
    }
    else if (screen_mode == VAULT) {
        if (back_btn) {
            screen_mode = MAIN_MENU;
            displayMainMenu();
        } else if (prev_btn || next_btn || sel_btn) { 
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
                String name = current_image.filename;
                String date_from_file = name.substring(4,6) + '/' + name.substring(6,8) + '/' + name.substring(0,4) + 
                                        " @ " + current_image.time;
                showDate(date_from_file);
                sel_btn = digitalRead(SELECT);
            }
            displayImage(current_image.filename);
            loading_image = false;
        }
    }

}
