#include <Adafruit_TinyUSB.h>
#include "Config.h"
#include "DisplayManager.h"
#include "GpsManager.h"

unsigned long lastDisplayUpdate = 0;

void serialSetup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(200);
}

void setup() {
  serialSetup();
  setupDisplay();
  setupGps();
}

void loop() {
  processGpsData();

  if (millis() - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = millis();

    if (!gps.time.isValid() || !gps.date.isValid()) {
      drawWaitingGPS(gps.charsProcessed());
      return;
    }

    drawTime(gps);
  }
}