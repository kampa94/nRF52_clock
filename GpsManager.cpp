#include "GpsManager.h"

TinyGPSPlus gps;

void setupGps() {
  pinMode(PIN_GPS_EN, OUTPUT);
  digitalWrite(PIN_GPS_EN, HIGH);

  pinMode(PIN_GPS_RESET, OUTPUT);
  digitalWrite(PIN_GPS_RESET, HIGH);

  delay(200);

  Serial2.begin(GPS_BAUD);
}

void processGpsData() {
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }
}