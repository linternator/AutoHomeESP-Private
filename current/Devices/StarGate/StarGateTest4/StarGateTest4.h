#ifndef STARGATE4_H
#define STARGATE4_H
// #include <AutoHome.h>
#include <FastLED.h>

#define GATE_LED_PIN 0
#define STAIRS_LED_PIN 2

#define GATE_LED_COUNT 63
#define STAIRS_LED_COUNT 51 // plus actual sairs

#define LED_TYPE WS2812
#define COLOR_ORDER GRB

extern CRGB Gate_strip[];
extern CRGB Stair_strip[];

extern int i;
extern int m0de;
extern int ChveronsLocks[];
extern int ChveronsSides[];
extern int Symbols[];
extern int Back[];
extern int Stairs[];
extern int Sides[];
extern int StairLights[];

void DialUpAnimationUpdate(unsigned long Time);
void DialInAnimation(unsigned long Time);

#endif