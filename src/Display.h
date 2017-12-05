#include <SD.h>

#ifndef DISPLAY_H
#define DISPLAY_H

void initDisplay(void);
void showDate(String date, bool rotate);
void displayImage(String filename, bool rotate);
void displayImage(String filename, bool rotate, uint8_t x, uint8_t y);

void bmpDraw(const char *filename, uint8_t x, uint8_t y);
uint16_t read16(File f);
uint32_t read32(File f);

#endif
