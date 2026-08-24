#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <TinyGPSPlus.h>
#include "Config.h"

void setupDisplay();
void drawOnTft(int16_t size, int16_t cursorX, int16_t cursorY, String str);
void drawTime(TinyGPSPlus &gps);
void drawWaitingGPS(uint32_t charsProcessed);

#endif