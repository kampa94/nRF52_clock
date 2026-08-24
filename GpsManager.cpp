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

String getDateStr() {
  char buf[TFT_WIDTH];
  snprintf(buf, sizeof(buf), "%02d/%02d/%04d",
           gps.date.day(), gps.date.month(), gps.date.year());
  return String(buf);
}

String getTimeStr() { 
  char buf[TFT_WIDTH];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second()); 
  return String(buf);
}

String getLatStr() { 
  return "Lat:" + String(gps.location.lat(), 6);
}

String getLonStr() {
  return "Lon:" + String(gps.location.lng(), 6);
}

String getAltStr() {
  return "Alt.: " + String((int)gps.altitude.meters()) + " slm";
}

String getSatStr() {
  return "Sat: " + String(gps.satellites.value());
}