#ifndef STARGATE_H
#include "StarGateTest4.h"
#endif
struct DialInAdressAnimationState {
  // Overall state
  int start_symbol = 18;
  int index = 18;
  int dailInComplete = 0;
  int state = 0;
};
DialInAdressAnimationState dialInAdressState;

void DialInAnimation(unsigned long Time) {

  switch (dialInAdressState.state) {
    case 0:
      {

        Gate_strip[Symbols[dialInAdressState.index]] = CRGB(0, 0, 0);  // set prviouse syombol to off.
        if (dialInAdressState.index == 35) {
          dialInAdressState.index = 0;
        } else {
          dialInAdressState.index++;
        }

        if (dialInAdressState.index == 18) {  // this should only trigure when 1 full loop has happened.
          dialInAdressState.state++;          // move to next state.
        }

        Gate_strip[Symbols[dialInAdressState.index]] = CRGB(255, 255, 255);  // set syombol on.

        Gate_strip[ChveronsLocks[((dialInAdressState.index - 2) / 4)]] = CRGB(255, 255, 255);  // set center chevron on, as symbols pass it.
      }

    case 1:
      {
        for (int Sym = 0; Sym < 35; Sym ++) {
          Gate_strip[Symbols[Sym]] = CRGB(255, 255, 255);  // set all symbols to on.
        }

        // add delay ? 
        delay(200);

        for (int side = 0; side < 18; side ++) {
          Gate_strip[ChveronsSides[side]] = CRGB(255,255,255);  // set all chevron sides to on.
        }

        // add delay ? 
        delay(200);

        for (int chev = 0; chev < 9; chev ++) {
          Gate_strip[ChveronsSides[chev]] = CRGB(255,255,255);  // set all chevrons to on.
        }

      }
  }



  delay(500);
}