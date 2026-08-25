#pragma once

enum class Screens
{
   CLOCK = 0,
   BATTERY = 1
};

inline Screens &operator++(Screens &screen)
{
   if (screen == Screens::BATTERY)
   {
      screen = Screens::CLOCK;
      return screen;
   }
   screen = static_cast<Screens>(static_cast<int>(screen) + 1);
   return screen;
}