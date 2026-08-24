#include <Adafruit_TinyUSB.h>
#include "Config.h"
#include "DisplayManager.h"
#include "GpsManager.h"

unsigned long lastDisplayUpdate = 0;
// Debounce
bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void serialSetup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && (millis() - start < 3000)) { 
    delay(10);
  }
  delay(200);
}

void setup() {
  serialSetup();
  setupDisplay();
  setupGps();
    pinMode(BUTTON_PIN, INPUT_PULLUP); // pulsante attivo basso

}

bool isButtonPressed() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }

  lastButtonState = reading;

  return buttonState == LOW; // LOW = premuto (con INPUT_PULLUP)
}
void showBlack(){
  return;
}

void loop() {
  processGpsData();

  if (isButtonPressed()) {
    showBlack();
    return; // salta l'aggiornamento normale finché il pulsante è premuto
  }

  if (millis() - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = millis();

    if (!gps.time.isValid() || !gps.date.isValid()) {
      drawWaitingGPS(gps.charsProcessed());
      return;
    }

    drawTime(gps);
  }
}