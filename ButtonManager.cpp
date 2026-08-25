#include "Config.h"

static bool lastButtonState = HIGH;
static bool buttonState = HIGH;
static unsigned long lastDebounceTime = 0;
static const unsigned long debounceDelay = 50;


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
