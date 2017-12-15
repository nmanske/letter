#include <SD.h>

#ifndef DISPLAY_H
#define DISPLAY_H

void initDisplay(void);

void displayMainMenu(void);
void updateModeSelection(void);
void displayImage(String filename);
void displayImage(String filename, uint8_t x, uint8_t y);
void showDate(String date);
void setRotateImage(bool rotate);
void displayTask(String task);

void bmpDraw(const char *filename, uint8_t x, uint8_t y);
uint16_t read16(File f);
uint32_t read32(File f);

#endif
