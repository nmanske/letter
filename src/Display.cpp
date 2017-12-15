#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <SPI.h>
#include <SD.h>

#include "Display.h"

#define TFT_WIDTH 128
#define TFT_HEIGHT 160
#define CHAR_WIDTH 6

#define TFT_CS  10  // chip select for lcd
#define TFT_RST  9  // reset line for lcd
#define TFT_DC   8  // data/command line for lcd
#define SD_CS    6  // chip select for sd card

#define MAX_IMAGES 20

// Define screen using user-defined pins
static Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

static bool vault_selected = true;

// Rotate landscape style vault images using setter
static bool rotate_image;

void initDisplay() {
  Serial.println("Initializing ST7735...");
  tft.initR(INITR_BLACKTAB);
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("Failed to initialize SD card");
    return;
  }
  displayMainMenu();
}

void displayMainMenu() {
  tft.setRotation(3);
  tft.fillScreen(ST7735_CYAN);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.setCursor(6, 30);
  tft.println("Memory Box");
  tft.setFont();
  tft.setTextSize(2);
  tft.setCursor(25, 55);
  tft.println("The Vault");
  tft.setTextSize(2);
  tft.setCursor(15, 90);
  tft.println("Bucket List");
  tft.drawRect(17, 48, 122, 28, ST7735_YELLOW);
  tft.setTextSize(1);
}

void updateModeSelection() {
  if (vault_selected) {
    tft.drawRect(17, 48, 122, 28, ST7735_CYAN);
    tft.drawRect(8, 83, 144, 28, ST7735_YELLOW);
    vault_selected = false;
  } else {
    tft.drawRect(17, 48, 122, 28, ST7735_YELLOW);
    tft.drawRect(8, 83, 144, 28, ST7735_CYAN);
    vault_selected = true;
  }
}

void displayImage(String filename) {
  bmpDraw(filename.c_str(), 0, 0);
}

void displayImage(String filename, uint8_t x, uint8_t y) {
  bmpDraw(filename.c_str(), x, y);
}

void showDate(String date) {
  if (rotate_image) tft.setCursor((TFT_HEIGHT-(CHAR_WIDTH*date.length()))/2, 0);
  else              tft.setCursor((TFT_WIDTH-(CHAR_WIDTH*date.length()))/2, 0);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.setTextWrap(true);
  tft.print(date);
}

void setRotateImage(bool rotate) {
  rotate_image = rotate;
  if (rotate_image) tft.setRotation(3);
  else              tft.setRotation(0);
}

void displayTask(String task) {
  tft.setRotation(3);
  tft.fillScreen(ST7735_YELLOW);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.setCursor(0, 10);
  tft.print(task);
}

/*******************************************************************
 * The below code is written courtesy of Adafruit and is pulled from
 * Adafruit-ST7735-Library
 *******************************************************************/

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(const char *filename, uint8_t x, uint8_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}