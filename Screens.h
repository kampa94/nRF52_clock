#pragma once

enum class Screens
{
   CLOCK,
   BATTERY
};

inline Screens &operator++(Screens &screen)
{
   if (screen == Screens::BATTERY)
   {
      screen = Screens::CLOCK; // Ricomincia da capo (opzionale)
      return screen;
   }
   screen = static_cast<Screens>(static_cast<int>(screen) + 1);
   return screen;
}