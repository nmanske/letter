#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

static Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 9, 10, 8);

void initDisplay(void) {
    display.begin();
    display.setContrast(50);
}

void setDisplay(void) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.println("Hello Test");
    display.display();
}
