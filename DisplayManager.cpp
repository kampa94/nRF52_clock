#include "DisplayManager.h"
#include <avr/dtostrf.h>
#include <stdio.h>
#include <string.h>
#include "GpsManager.h"
#include "Battery.h"
#include "Screens.h"

Adafruit_ST7789 tft(&SPI1, SOC_GPIO_PIN_T114_TFT_SS, SOC_GPIO_PIN_T114_TFT_DC, SOC_GPIO_PIN_T114_TFT_RST);

#define MAX_ROWS 6
static char lastLines[MAX_ROWS][64];
static char buf[256];
Screens currentScreen = Screens::CLOCK;
Screens getCurrentScreen(){
  return currentScreen;
}

void changeScreen()
{
  ++currentScreen;
}
void setupDisplay()
{
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

void drawOnTft(int16_t size, int16_t cursorX, int16_t cursorY, String str)
{
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(size);
  tft.setCursor(cursorX, cursorY);
  tft.print(str);
}

void drawTime(TinyGPSPlus &gps)
{
  tft.startWrite();

  drawOnTft(3, 10, 10, getTimeStr());
  drawOnTft(2, 10, 40, getDateStr());
  drawOnTft(2, 10, 60, getSatStr());
  drawOnTft(2, 10, 80, getLatStr());
  drawOnTft(2, 10, 100, getLonStr());
  drawOnTft(2, 10, 120, getAltStr());

  tft.endWrite();
}

void drawWaitingGPS(uint32_t charsProcessed)
{
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("In attesa GPS...");
  tft.setTextSize(1);
  tft.setCursor(0, 40);
  tft.print("Caratteri ricevuti: ");
  tft.print(charsProcessed);
  tft.fillScreen(ST77XX_BLACK);
}

void drawBattery()
{
  tft.startWrite();

  drawOnTft(3, 10, 10, getBatteryStr());

  tft.endWrite();
}
void cleanScreen()
{
  tft.fillScreen(ST77XX_BLACK);
}
