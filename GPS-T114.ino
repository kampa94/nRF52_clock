#include <Adafruit_TinyUSB.h>
#include "Config.h"
#include "DisplayManager.h"
#include "GpsManager.h"
#include "Screens.h"
#include "ButtonManager.h"
#include "GPS-T114.h"

unsigned long lastDisplayUpdate = 0;

// Debounce
bool stillPress = true;
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

  switch (getCurrentScreen())
  {
  case Screens::CLOCK:
    if (millis() - lastDisplayUpdate >= 1000)
    {
      lastDisplayUpdate = millis();

      if (!gps.time.isValid() || !gps.date.isValid())
      {
        drawWaitingGPS(gps.charsProcessed());
        return;
      }

      drawTime(gps);
    }
    break;

  case Screens::BATTERY:
     if (millis() - lastDisplayUpdate >= 1000)
    {
      lastDisplayUpdate = millis();
      drawBattery();
    }
  
    break;
  default:
    break;
  }
}
void changeScreenIfButtonIsPressed()
{
  if (isButtonPressed())
  {
    if (!stillPress)
    {
      cleanScreen();
      changeScreen();

      stillPress = true;
    }
  }
  else
  {
    stillPress = false;
  }
}