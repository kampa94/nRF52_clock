#include "Config.h"

bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;


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
