#ifndef STARGATE_H
#include "StarGateTest4.h"
#endif
struct DialInAdressAnimationState
{
  // Overall state
  int start_symbol = 18;
  int index = 18;
};
DialInAdressAnimationState dialInAdressState;

void DialInAnimation(unsigned long Time)
{
  Serial.println(dialInAdressState.index);

  Gate_strip[Symbols[dialInAdressState.index]] = CRGB(0, 0, 0);
  if (dialInAdressState.index == 35)
  {
    dialInAdressState.index = 0;
  }
  else
  {
    dialInAdressState.index++;
  }
  Gate_strip[Symbols[dialInAdressState.index]] = CRGB(255, 255, 255);

  Gate_strip[ChveronsLocks[((dialInAdressState.index -2 ) / 4)]] = CRGB(255, 255, 255);
  Serial.println(((dialInAdressState.index + 3) / 4));
  delay(500);
}