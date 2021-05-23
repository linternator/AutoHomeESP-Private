#ifndef STARGATE_H
#include "StarGateTest4.h"
#endif
struct DialUpAdressAnimationState
{
  // Overall state
  int currentChveron = 0; // 7, 8, 9 . adress
  int address[9] = {24, 3, 10, 17, 31, 21, 27, 6, 33};
  int addressCount = 9;

  // Search algorithm (Animation)
  int currentSearchSymbol = 0;
  int currentDialAnimationPart = 0;
  bool currentChveronLocked = false;
  unsigned long dialAnimationPartTime = 0;
};
DialUpAdressAnimationState dialUpAdressState;


void DialAdress(int start_symbol, bool left, int symbol, int chveron, unsigned long Time)
{

  switch (dialUpAdressState.currentDialAnimationPart)
  {
  case 0:
    dialUpAdressState.dialAnimationPartTime = Time;
    dialUpAdressState.currentDialAnimationPart = 1;
    break;

  // 1. Light sides of chveron sides
  case 1:
    Gate_strip[ChveronsSides[chveron * 2]] = CRGB(255, 255, 255);
    Gate_strip[ChveronsSides[chveron * 2 + 1]] = CRGB(255, 255, 255);
    if (Time > dialUpAdressState.dialAnimationPartTime + 500)
    {
      dialUpAdressState.currentDialAnimationPart = 2;
      dialUpAdressState.dialAnimationPartTime = Time;
    }
    break;

  // 2. Light the first symbol
  case 2:
    Gate_strip[Symbols[start_symbol]] = CRGB(255, 255, 255);
    if (Time > dialUpAdressState.dialAnimationPartTime + 250)
    {
      dialUpAdressState.currentDialAnimationPart = 3;
      dialUpAdressState.dialAnimationPartTime = Time;
      dialUpAdressState.currentSearchSymbol = start_symbol;
    }
    break;

  // 3. Is the symbol we are on, the correct symbol?
  //  3a: It is not the correct symbol, we go to the symbol to the left and repeat 3
  //  We do this by turning off current symbol and turn on the left symbol
  case 3:
    if (dialUpAdressState.currentSearchSymbol == symbol)
    {
      // We have found the correct symbol
      dialUpAdressState.currentDialAnimationPart = 4;
      dialUpAdressState.dialAnimationPartTime = Time;
    }
    else if (Time > dialUpAdressState.dialAnimationPartTime + 100)
    {
      //Goes to the next symbol
      dialUpAdressState.dialAnimationPartTime = Time;
      Gate_strip[Symbols[dialUpAdressState.currentSearchSymbol]] = CRGB(0, 0, 0);
      if (left)
      {
        dialUpAdressState.currentSearchSymbol = dialUpAdressState.currentSearchSymbol + 1;
      }
      else
      {
        dialUpAdressState.currentSearchSymbol = dialUpAdressState.currentSearchSymbol - 1;
      }
      // Goes to the first symbol
      if (dialUpAdressState.currentSearchSymbol >= 36)
      {
        dialUpAdressState.currentSearchSymbol = 0;
      }
      // Goes to the last symbol
      if (dialUpAdressState.currentSearchSymbol < 0)
      {
        dialUpAdressState.currentSearchSymbol = 35;
      }
      Gate_strip[Symbols[dialUpAdressState.currentSearchSymbol]] = CRGB(255, 255, 255);
    }
    break;

    // 4. When it reaches the correct one we lights the chveron
  case 4:
    if (Time > dialUpAdressState.dialAnimationPartTime + 250)
    {
      Gate_strip[ChveronsLocks[chveron]] = CRGB(255, 255, 255);
      dialUpAdressState.currentDialAnimationPart = 5;
      dialUpAdressState.dialAnimationPartTime = Time;
    }
    break;

    // 5. We turn off the chveron sides
  case 5:
    if (Time > dialUpAdressState.dialAnimationPartTime + 500)
    {
      Gate_strip[ChveronsSides[chveron * 2]] = CRGB(0, 0, 0);
      Gate_strip[ChveronsSides[chveron * 2 + 1]] = CRGB(0, 0, 0);
      dialUpAdressState.currentDialAnimationPart = 6;
      dialUpAdressState.dialAnimationPartTime = Time;
    }
    break;

    // 6. Change the color of the symbol (to show it have been chosen)
  case 6:
    Gate_strip[Symbols[symbol]] = CRGB(0, 0, 255);
    dialUpAdressState.currentDialAnimationPart = 7;
    dialUpAdressState.dialAnimationPartTime = Time;
    break;

    // 7. Add a delay
  case 7:
    if (Time > dialUpAdressState.dialAnimationPartTime + 1500)
    {
      dialUpAdressState.currentDialAnimationPart = 0;
      dialUpAdressState.dialAnimationPartTime = Time;
      dialUpAdressState.currentChveronLocked = true;
    }
    break;

  default:
    break;
  }
}

void DialUpAnimationUpdate(unsigned long Time)
{
  if (dialUpAdressState.currentChveronLocked)
  {
    dialUpAdressState.currentChveron++;
    dialUpAdressState.currentChveronLocked = false;
  }

  // Checks if we all dialed up
  if (dialUpAdressState.currentChveron == dialUpAdressState.addressCount)
  {

    if (Time > dialUpAdressState.dialAnimationPartTime + 5000)
    {
      //Resets the gate and starts a new dial up
      dialUpAdressState.currentChveron = 0;
      dialUpAdressState.addressCount = random(7, 10);
      for (int index = 0; index < dialUpAdressState.addressCount; index++)
      {
        dialUpAdressState.address[index] = random(0, 36);
      }

      for (int index = 0; index < 36; index++)
      {
        Gate_strip[Symbols[index]] = CRGB(0, 0, 0);
      }

      for (int index = 0; index < 18; index++)
      {
        Gate_strip[ChveronsSides[index]] = CRGB(0, 0, 0);
      }

      for (int index = 0; index < 9; index++)
      {
        Gate_strip[ChveronsLocks[index]] = CRGB(0, 0, 0);
      }
    }
    else
    {
      return;
    }
  }

  int start_symbol = 18;
  if (dialUpAdressState.currentChveron > 0)
  {
    start_symbol = dialUpAdressState.address[dialUpAdressState.currentChveron - 1];
  }

  DialAdress(
      start_symbol,
      dialUpAdressState.currentChveron % 2 == 0,
      dialUpAdressState.address[dialUpAdressState.currentChveron],
      dialUpAdressState.currentChveron,
      Time);

  // Ensures the previous locked symbols stay turned on
  for (int index = 0; index < dialUpAdressState.currentChveron; index++)
  {
    if (dialUpAdressState.currentSearchSymbol != dialUpAdressState.address[index])
    {
      Gate_strip[Symbols[dialUpAdressState.address[index]]] = CRGB(0, 0, 255);
    }
  }
}
