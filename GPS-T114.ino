#include <Adafruit_TinyUSB.h>
#include "Config.h"
#include "DisplayManager.h"
#include "GpsManager.h"
#include "Screens.h"

unsigned long lastDisplayUpdate = 0;

// Debounce
bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
Screens currentScreen = Screens::CLOCK;
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

bool isButtonPressed()
{
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    buttonState = reading;
  }

  lastButtonState = reading;

  return buttonState == LOW;
}
void changeScreen()
{
  ++currentScreen;
}

void loop()
{
  processGpsData();

  if (isButtonPressed())
  {
    if (!stillPress)
    {
      changeScreen();
      stillPress = true;
    }
  }
  else
  {
    stillPress = false;
  }

  switch (currentScreen)
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