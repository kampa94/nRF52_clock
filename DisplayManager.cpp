#include "DisplayManager.h"
#include <avr/dtostrf.h>
#include <stdio.h>
#include <string.h>

Adafruit_ST7789 tft(&SPI1, SOC_GPIO_PIN_T114_TFT_SS, SOC_GPIO_PIN_T114_TFT_DC, SOC_GPIO_PIN_T114_TFT_RST);

#define MAX_ROWS 6
static char lastLines[MAX_ROWS][64];
static char buf[256]; // Ridotto a dimensione ragionevole invece di TFT_HEIGHT * TFT_WIDTH

static int getRowIndex(int16_t y) {
  if (y == 10) return 0;
  if (y == 40) return 1;
  if (y == 60) return 2;
  if (y == 80) return 3;
  if (y == 100) return 4;
  if (y == 120) return 5;
  return -1;
}

void setupDisplay() {
  digitalWrite(SOC_GPIO_PIN_T114_TFT_EN, LOW);
  pinMode(SOC_GPIO_PIN_T114_TFT_EN, OUTPUT);

  digitalWrite(SOC_GPIO_PIN_T114_TFT_BLGT, LOW);
  pinMode(SOC_GPIO_PIN_T114_TFT_BLGT, OUTPUT);

  digitalWrite(SOC_GPIO_PIN_T114_ADC_EN, HIGH);
  pinMode(SOC_GPIO_PIN_T114_ADC_EN, OUTPUT);

  tft.init(TFT_WIDTH, TFT_HEIGHT);
  tft.setRotation(3);
  tft.setSPISpeed(40000000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.print("In attesa GPS...");
}

void drawOnTft(int16_t size, int16_t cursorX, int16_t cursorY, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  int rowIndex = getRowIndex(cursorY);

  if (rowIndex != -1) {
    if (strcmp(lastLines[rowIndex], buf) == 0) {
      return;
    }
    strcpy(lastLines[rowIndex], buf);
  }

  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(size);
  tft.setCursor(cursorX, cursorY);
  tft.print(buf);
}

void drawTime(TinyGPSPlus &gps) {
  tft.startWrite();

  drawOnTft(3, 10, 10, "%02d:%02d:%02d", gps.time.hour() + 2, gps.time.minute(), gps.time.second());
  drawOnTft(2, 10, 40, "UTC: %02d/%02d/%04d", gps.date.day(), gps.date.month(), gps.date.year());
  drawOnTft(2, 10, 60, "Satelliti: %d", gps.satellites.value());

  char latBuf[12], lngBuf[12];
  dtostrf(gps.location.lat(), 4, 5, latBuf);
  dtostrf(gps.location.lng(), 4, 5, lngBuf);

  drawOnTft(2, 10, 80, "Lat:%s", latBuf);
  drawOnTft(2, 10, 100, "Lon:%s", lngBuf);
  drawOnTft(2, 10, 120, "Alt.: %d slm", gps.altitude.meters());

  tft.endWrite();
}

void drawWaitingGPS(uint32_t charsProcessed) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("In attesa GPS...");
  tft.setTextSize(1);
  tft.setCursor(0, 40);
  tft.print("Caratteri ricevuti: ");
  tft.print(charsProcessed);
}