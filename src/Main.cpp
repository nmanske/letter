#include <Arduino.h>
#include <avr/pgmspace.h>

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
#define NUM_VAULT_IMAGES 6
#define NUM_BUCKETLIST_ITEMS 10

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
const Image vault [NUM_VAULT_IMAGES] PROGMEM = {
    {"01.bmp", "11/01/15 @ 02:07:38", true}, {"02.bmp", "05/30/16 @ 19:15:01", true},
    {"03.bmp", "07/04/16 @ 21:02:11", true}, {"04.bmp", "04/09/17 @ 16:46:10", true},
    {"05.bmp", "07/09/17 @ 10:07:17", true}, {"06.bmp", "08/26/17 @ 20:45:49", true},
};

// Tasks to cycle through in the bucketlist
const String bucketlist [NUM_BUCKETLIST_ITEMS] PROGMEM = {
    "Traveling to Spain", "Attending NYC march",
    "Completing Overcooked", "Going primitive camping" 
};

// ************************************************************************

// Current state of screen as defined in SCREEN_STATES
static Mode screen_mode = MAIN_MENU;

// First, second, third, etc button starting at index 0
static Selection mode_selection = VAULT_SEL;

// Current image displayed on screen and pulled from vault array
static uint8_t vault_index = 0;

// Current task displayed on screen and pulled from bucketlist array
static uint8_t bucketlist_index = 0;

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
        } else if (sel_btn && mode_selection == BUCKET_LIST_SEL) {
            screen_mode = BUCKET_LIST;
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
                showDate(current_image.time);
                sel_btn = digitalRead(SELECT);
            }
            displayImage(current_image.filename);
            loading_image = false;
        }
    }
    else if (screen_mode == BUCKET_LIST) {
        if (back_btn) {
            screen_mode = MAIN_MENU;
            displayMainMenu();
        } else if (prev_btn || next_btn || sel_btn) {
            if (prev_btn) {
                if(--bucketlist_index < 0) bucketlist_index = NUM_BUCKETLIST_ITEMS - 1;
            }
            else if (next_btn) {
                if(++bucketlist_index >= NUM_VAULT_IMAGES) bucketlist_index = 0;
            }
            Serial.println(bucketlist[bucketlist_index]);
            displayTask(bucketlist[bucketlist_index]);
        }
    }

}
