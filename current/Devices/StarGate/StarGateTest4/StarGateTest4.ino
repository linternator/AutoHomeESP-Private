#include "StarGateTest4.h"

CRGB Gate_strip[GATE_LED_COUNT];
CRGB Stair_strip[STAIRS_LED_COUNT];

int i = 0;
int m0de = 1;

int ChveronsLocks[] = {3, 10, 17, 24, 31,
                       38, 45, 52, 59}; // chevron middle bits
int ChveronsSides[] = {
    2, 4, 9, 11, 16, 18, 23, 25, 30,
    32, 37, 39, 44, 46, 51, 53, 58, 60}; // chevron side bits

int Symbols[] = {0, 1, 5, 6, 7, 8, 12, 13, 14, 15, 19, 20,
                 21, 22, 26, 27, 28, 29, 33, 34, 35, 36, 40, 41,
                 42, 43, 47, 48, 49, 50, 54, 55, 56, 57, 61, 62}; // symoblosl

int Back[] = {41, 42, 43, 44, 45, 46, 47, 48, 49, 50}; // the back

int Stairs[] = {         // The stairs
    12, 13,              // Step 1 (top step)
    14, 15, 16,          // Step 2
    17, 18, 19,          // Step 3
    20, 21, 22,          // Step 4
    23, 24, 25, 26,      // Step 5
    27, 28, 29, 30,      // Step 6
    31, 32, 33, 34, 35,  // Step 7
    40, 39, 38, 37, 36}; // Step 8 (bottom step)

int Sides[] = {7, 8, 6, 9, 5, 10, 4, 11}; // the sides, in light order
int StairLights[] = {0, 1, 2, 3};         // side ligitng on stairs

struct BackAnimationState
{
  int update_interval = 50;
  int last_update_time = 0;
  int index = 0;
};
BackAnimationState backState;

struct SideAnimationState
{
  int update_interval = 500;
  unsigned long last_update_time = 0;
  int index = 0;
};
SideAnimationState sideState;

struct StairsAnimationState
{
  int update_interval = 100;
  unsigned long last_update_time = 0;
  int counter = 0;
};
StairsAnimationState stairsState;

struct StairLightsAnimationState
{
  int reset_interval = 350;
  unsigned long next_update_time_normal = 0;
  unsigned long next_update_time_flicker = 0;
  int random_flash = 600;
};
StairLightsAnimationState stairLightsState;

void setup()
{
  delay(500);
  FastLED
      .addLeds<LED_TYPE, GATE_LED_PIN, COLOR_ORDER>(Gate_strip, GATE_LED_COUNT)
      .setCorrection(TypicalLEDStrip);
  FastLED
      .addLeds<LED_TYPE, STAIRS_LED_PIN, COLOR_ORDER>(Stair_strip,
                                                      STAIRS_LED_COUNT)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
  FastLED.clear();
  delay(200);
  Serial.begin(115200);

  // Ensures the next update will be from this time
  backState.last_update_time = millis();
  sideState.last_update_time = millis();
  stairsState.last_update_time = millis();
}

void loop()
{
  unsigned long time = millis();
  UpdateBack(time);
  UpdateSides(time);
  UpdateStairs(time);
  UpdateStairLights(time);
  DialUpAnimationUpdate(time);

  //Gate_strip[ChveronsLocks[4]] = CRGB(255, 255, 255);
  // CRGB color = CRGB(0, 0, 255);
  //StepThroughArray(Stair_strip, StairLights, 4, color);
  FastLED.show();
  delay(16);
}

void StepThroughArray(CRGB *leds, int *array, int array_size, CRGB &color)
{
  FastLED.clear();
  if (i == array_size)
  {
    i = 0;
  }
  Serial.println(i);
  leds[array[i]] = color;

  i++;
}

void UpdateBack(unsigned long Time)
{
  if (Time > backState.last_update_time + backState.update_interval)
  {
    backState.last_update_time = Time;

    // Turn off previus pixel
    Stair_strip[Back[backState.index]] = CRGB(0, 0, 0);

    // Moves to the next pixel
    backState.index++;

    // So we don't run over the end of the array
    if (backState.index == 10)
    {
      backState.index = 0;
    }

    // Light the next pixel
    Stair_strip[Back[backState.index]] = CRGB(255, 255, 255);
  }
}

void UpdateSides(unsigned long Time)
{
  if (Time > sideState.last_update_time + sideState.update_interval && sideState.index < 8)
  {
    sideState.last_update_time = Time;

    Stair_strip[Sides[sideState.index]] = CRGB(0, 255, 0);
    Stair_strip[Sides[sideState.index + 1]] = CRGB(0, 255, 0);

    sideState.index = sideState.index + 2;
  }
}

void UpdateStairs(unsigned long Time)
{
  if (Time > stairsState.last_update_time + stairsState.update_interval && stairsState.counter < 8)
  {
    stairsState.last_update_time = Time;

    int numberOfStairsToTurnOn = 0;
    switch (stairsState.counter)
    {
    case 0:
      numberOfStairsToTurnOn = 5;
      break;
    case 1:
      numberOfStairsToTurnOn = 10;
      break;
    case 2:
      numberOfStairsToTurnOn = 14;
      break;
    case 3:
      numberOfStairsToTurnOn = 18;
      break;
    case 4:
      numberOfStairsToTurnOn = 21;
      break;
    case 5:
      numberOfStairsToTurnOn = 24;
      break;
    case 6:
      numberOfStairsToTurnOn = 27;
      break;
    case 7:
      numberOfStairsToTurnOn = 29;
      break;
    default:
      break;
    }
    for (int index = 28; index > 28 - numberOfStairsToTurnOn; index--)
    {
      Stair_strip[Stairs[index]] = CRGB(0, 0, 255);
    }
    stairsState.counter++;
  }
}

void UpdateStairLights(unsigned long Time)
{

  if (Time > stairLightsState.next_update_time_flicker)
  {
    stairLightsState.next_update_time_flicker = Time + random(1000, 5000);

    // picks left/right
    int led1, led2 = 0;
    if (random(0, 2))
    {
      led1 = 0;
      led2 = 1;
    }
    else
    {
      led1 = 2;
      led2 = 3;
    }

    // Picks up/down
    if (random(0, 2))
    {
      Stair_strip[StairLights[led1]] = CRGB(255, 255, 255);
      Stair_strip[StairLights[led2]] = CRGB(255, 255, 255);
    }
    else
    {
      Stair_strip[StairLights[led1]] = CRGB(80, 80, 80);
      Stair_strip[StairLights[led2]] = CRGB(80, 80, 80);
    }
  }

  if (Time > stairLightsState.next_update_time_normal)
  {
    stairLightsState.next_update_time_normal = Time + stairLightsState.reset_interval;
    for (int index = 0; index < 4; index++)
    {
      Stair_strip[StairLights[index]] = CRGB(128, 128, 0);
    }
  }
}