#include <Adafruit_TinyUSB.h>
#include "Config.h"
#include "DisplayManager.h"
#include "GpsManager.h"
#include "Screens.h"
#include "ButtonManager.h"
#include "GPS-T114.h"

unsigned long lastDisplayUpdate = 0;

void serialSetup()
{
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && (millis() - start < 3000))
  {
    delay(10);
  }
  delay(200);
}

void setup()
{
  serialSetup();
  setupDisplay();
  setupGps();
  pinMode(BUTTON_PIN, INPUT_PULLUP); // pulsante attivo basso
}

void loop()
{
  processGpsData();

  changeScreenIfButtonIsPressed();

  static Screens lastScreen = getCurrentScreen();

  if (getCurrentScreen() != lastScreen || (millis() - lastDisplayUpdate >= 1000))
  {
    lastDisplayUpdate = millis();
    lastScreen = getCurrentScreen();

    switch (getCurrentScreen())
    {
    case Screens::CLOCK:
      if (!gps.time.isValid() || !gps.date.isValid())
      {
        drawWaitingGPS(gps.charsProcessed());
      }
      else
      {
        drawTime();
      }
      break;

    case Screens::BATTERY:
      drawBattery();
      break;

    default:
      break;
    }
  }
}

void changeScreenIfButtonIsPressed()
{
  if (wasButtonJustPressed())
  {
    cleanScreen();
    nextScreen();
    Serial.println("Screen changed to: " + String(static_cast<int>(getCurrentScreen())));
    lastDisplayUpdate = 0;
  }
}