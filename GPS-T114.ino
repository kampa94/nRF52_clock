#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>

/* Alimentazione periferiche */
#define SOC_GPIO_PIN_T114_ADC_EN 6    // P0.06
#define SOC_GPIO_PIN_T114_VEXT_EN 21  // P0.21  -- ATTENZIONE: vedi nota sotto

/* TFT */
#define TFT_WIDTH 135
#define TFT_HEIGHT 240
#define SOC_GPIO_PIN_T114_TFT_SS 11    // P0.11
#define SOC_GPIO_PIN_T114_TFT_DC 12    // P0.12
#define SOC_GPIO_PIN_T114_TFT_RST 2    // P0.02
#define SOC_GPIO_PIN_T114_TFT_EN 3     // P0.03
#define SOC_GPIO_PIN_T114_TFT_BLGT 15  // P0.15

/* GPS - alimentazione e reset dedicati */
#define PIN_GPS_EN 21  // ATTENZIONE: stesso numero di VEXT_EN sopra, vedi nota
#define PIN_GPS_RESET 38
#define GPS_BAUD 9600

Adafruit_ST7789 tft(&SPI1, SOC_GPIO_PIN_T114_TFT_SS, SOC_GPIO_PIN_T114_TFT_DC, SOC_GPIO_PIN_T114_TFT_RST);
TinyGPSPlus gps;

unsigned long lastDisplayUpdate = 0;
bool everGotFix = false;

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

void setupGps() {
  pinMode(PIN_GPS_EN, OUTPUT);
  digitalWrite(PIN_GPS_EN, HIGH);

  pinMode(PIN_GPS_RESET, OUTPUT);
  digitalWrite(PIN_GPS_RESET, HIGH);

  delay(200);

  Serial2.begin(GPS_BAUD);
}

void serialSetup() {
  Serial.begin(115200);           // USB, per il Serial Monitor
  while (!Serial) { delay(10); }  // aspetta che il monitor si apra (utile su nRF52)

  delay(200);
}

void setup() {
  serialSetup();
  setupDisplay();
  setupGps();
}


void drawOnTft(int16_t size, int16_t cursorX, int16_t cursorY, const char* fmt, ...) {
  char buf[64];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  tft.setTextSize(size);
  tft.setCursor(cursorX, cursorY);
  tft.print(buf);
}

void drawTime() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  drawOnTft(3, 10, 10, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
  drawOnTft(2, 10, 40,  "UTC: %02d/%02d/%04d", gps.date.day(), gps.date.month(), gps.date.year());
  drawOnTft(2, 10, 60,  "Satelliti: %d", gps.satellites.value()); 
  drawOnTft(2, 10, 80,  "Location: %d, %d", gps.location.lat(), gps.location.lng());
  drawOnTft(2, 10, 100,  "Alt.: %d slm", gps.altitude.meters());
}

void loop() {
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }

  if (millis() - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = millis();
    Serial.print("chars: ");
    Serial.print(gps.charsProcessed());
    Serial.print("  sentences: ");
    Serial.print(gps.sentencesWithFix());
    Serial.print("  fail checksum: ");
    Serial.println(gps.failedChecksum());
    if (gps.time.isValid() && gps.date.isValid()) {
      everGotFix = true;
      drawTime();
    } else if (!everGotFix) {
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(0, 0);
      tft.print("In attesa GPS...");
      tft.setTextSize(1);
      tft.setCursor(0, 40);
      tft.print("Caratteri ricevuti: ");
      tft.print(gps.charsProcessed());
    }
  }
}