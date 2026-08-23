#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>
#include <avr/dtostrf.h>
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



char buf[TFT_HEIGHT * TFT_WIDTH];
#include <string.h>  // Necessario per strcmp e strcpy

// Definiamo il numero massimo di righe che gestisci sullo schermo
#define MAX_ROWS 6
// Struttura per memorizzare lo stato dell'ultima stampa
char lastLines[MAX_ROWS][64];

// Funzione helper per ottenere l'indice del buffer in base alla coordinata Y
int getRowIndex(int16_t y) {
  if (y == 10) return 0;   // Ora
  if (y == 40) return 1;   // Data UTC
  if (y == 60) return 2;   // Satelliti
  if (y == 80) return 3;   // Latitudine
  if (y == 100) return 4;  // Longitudine
  if (y == 120) return 5;  // Altitudine
  return -1;               // Coordinata sconosciuta
}

void drawOnTft(int16_t size, int16_t cursorX, int16_t cursorY, const char* fmt, ...) {
  // 1. Genera la stringa formattata nel buffer temporaneo
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  // 2. Trova l'indice della riga basandoti sulla coordinata Y
  int rowIndex = getRowIndex(cursorY);

  if (rowIndex != -1) {
    // Se il testo è IDENTICO a quello già presente sullo schermo, non fare nulla
    if (strcmp(lastLines[rowIndex], buf) == 0) {
      return;
    }
    // Altrimenti, aggiorna il testo memorizzato per la prossima volta
    strcpy(lastLines[rowIndex], buf);
  }

  // 3. Cancella la vecchia riga sovrascrivendola con lo sfondo prima di stampare il nuovo testo
  // Nota: Usiamo setTextColor(TESTO, SFONDO) così la libreria cancella automaticamente i vecchi pixel
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(size);
  tft.setCursor(cursorX, cursorY);
  tft.print(buf);
}

void drawTime() {
  tft.startWrite();
  // NOTA: Abbiamo RIMOSSO tft.fillScreen(ST77XX_BLACK) per evitare lo sfarfallio totale dello schermo.
  // Lo sfondo viene aggiornato in modo mirato solo dove il testo cambia.

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




void setup() {
  serialSetup();
  setupDisplay();
  setupGps();
}

void loop() {
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }

  if (millis() - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = millis();

    if (!gps.time.isValid() || !gps.date.isValid()) {

      tft.setTextSize(2);
      tft.setCursor(0, 0);
      tft.print("In attesa GPS...");
      tft.setTextSize(1);
      tft.setCursor(0, 40);
      tft.print("Caratteri ricevuti: ");
      tft.print(gps.charsProcessed());
      tft.fillScreen(ST77XX_BLACK);

      return;
    }
    everGotFix = true;
    drawTime();
  }
}