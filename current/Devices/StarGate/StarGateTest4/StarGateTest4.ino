// #include <AutoHome.h>
#include <FastLED.h>

#define GATE_LED_PIN 0
#define STAIRS_LED_PIN 5

#define GATE_LED_COUNT 63
#define STAIRS_LED_COUNT 51 // plus actual sairs

#define LED_TYPE WS2812
#define COLOR_ORDER GRB

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

struct SymbolsAnimationState
{
  unsigned long last_update_time = 0;
  int update_interval = 50;
  int index = 0;
};
SymbolsAnimationState symbolsState;

struct DialAdressAnimationState
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
DialAdressAnimationState dialAdressState;

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
  UpdateSymbols(time);

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

void UpdateSymbols(unsigned long Time)
{
  // if (Time > symbolsState.last_update_time + symbolsState.update_interval)
  // {
  //   symbolsState.last_update_time = Time;

  //   Gate_strip[Symbols[symbolsState.index]] = CRGB(0, 0, 0);
  //   symbolsState.index++;

  //   if (symbolsState.index > 35)
  //   {
  //     symbolsState.index = 0;
  //   }

  //   Gate_strip[Symbols[symbolsState.index]] = CRGB(255, 128, 0);
  // }

  if (dialAdressState.currentChveronLocked)
  {
    dialAdressState.currentChveron++;
    dialAdressState.currentChveronLocked = false;
  }

  // Checks if we all dialed up
  if (dialAdressState.currentChveron == dialAdressState.addressCount)
  {

    if (Time > dialAdressState.dialAnimationPartTime + 5000)
    {
      //Resets the gate and starts a new dial up
      dialAdressState.currentChveron = 0;
      dialAdressState.addressCount = random(7, 10);
      for (int index = 0; index < dialAdressState.addressCount; index++)
      {
        dialAdressState.address[index] = random(0, 36);
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
  if (dialAdressState.currentChveron > 0)
  {
    start_symbol = dialAdressState.address[dialAdressState.currentChveron - 1];
  }

  DialAdress(
      start_symbol,
      dialAdressState.currentChveron % 2 == 0,
      dialAdressState.address[dialAdressState.currentChveron],
      dialAdressState.currentChveron,
      Time);

  // Ensures the previous locked symbols stay turned on
  for (int index = 0; index < dialAdressState.currentChveron; index++)
  {
    if (dialAdressState.currentSearchSymbol != dialAdressState.address[index])
    {
      Gate_strip[Symbols[dialAdressState.address[index]]] = CRGB(0, 0, 255);
    }
  }
}

void DialAdress(int start_symbol, bool left, int symbol, int chveron, unsigned long Time)
{

  switch (dialAdressState.currentDialAnimationPart)
  {
  case 0:
    dialAdressState.dialAnimationPartTime = Time;
    dialAdressState.currentDialAnimationPart = 1;
    break;

  // 1. Light sides of chveron sides
  case 1:
    Gate_strip[ChveronsSides[chveron * 2]] = CRGB(255, 255, 255);
    Gate_strip[ChveronsSides[chveron * 2 + 1]] = CRGB(255, 255, 255);
    if (Time > dialAdressState.dialAnimationPartTime + 500)
    {
      dialAdressState.currentDialAnimationPart = 2;
      dialAdressState.dialAnimationPartTime = Time;
    }
    break;

  // 2. Light the first symbol
  case 2:
    Gate_strip[Symbols[start_symbol]] = CRGB(255, 255, 255);
    if (Time > dialAdressState.dialAnimationPartTime + 250)
    {
      dialAdressState.currentDialAnimationPart = 3;
      dialAdressState.dialAnimationPartTime = Time;
      dialAdressState.currentSearchSymbol = start_symbol;
    }
    break;

  // 3. Is the symbol we are on, the correct symbol?
  //  3a: It is not the correct symbol, we go to the symbol to the left and repeat 3
  //  We do this by turning off current symbol and turn on the left symbol
  case 3:
    if (dialAdressState.currentSearchSymbol == symbol)
    {
      // We have found the correct symbol
      dialAdressState.currentDialAnimationPart = 4;
      dialAdressState.dialAnimationPartTime = Time;
    }
    else if (Time > dialAdressState.dialAnimationPartTime + 100)
    {
      //Goes to the next symbol
      dialAdressState.dialAnimationPartTime = Time;
      Gate_strip[Symbols[dialAdressState.currentSearchSymbol]] = CRGB(0, 0, 0);
      if (left)
      {
        dialAdressState.currentSearchSymbol = dialAdressState.currentSearchSymbol + 1;
      }
      else
      {
        dialAdressState.currentSearchSymbol = dialAdressState.currentSearchSymbol - 1;
      }
      // Goes to the first symbol
      if (dialAdressState.currentSearchSymbol >= 36)
      {
        dialAdressState.currentSearchSymbol = 0;
      }
      // Goes to the last symbol
      if (dialAdressState.currentSearchSymbol < 0)
      {
        dialAdressState.currentSearchSymbol = 35;
      }
      Gate_strip[Symbols[dialAdressState.currentSearchSymbol]] = CRGB(255, 255, 255);
    }
    break;

    // 4. When it reaches the correct one we lights the chveron
  case 4:
    if (Time > dialAdressState.dialAnimationPartTime + 250)
    {
      Gate_strip[ChveronsLocks[chveron]] = CRGB(255, 255, 255);
      dialAdressState.currentDialAnimationPart = 5;
      dialAdressState.dialAnimationPartTime = Time;
    }
    break;

    // 5. We turn off the chveron sides
  case 5:
    if (Time > dialAdressState.dialAnimationPartTime + 500)
    {
      Gate_strip[ChveronsSides[chveron * 2]] = CRGB(0, 0, 0);
      Gate_strip[ChveronsSides[chveron * 2 + 1]] = CRGB(0, 0, 0);
      dialAdressState.currentDialAnimationPart = 6;
      dialAdressState.dialAnimationPartTime = Time;
    }
    break;

    // 6. Change the color of the symbol (to show it have been chosen)
  case 6:
    Gate_strip[Symbols[symbol]] = CRGB(0, 0, 255);
    dialAdressState.currentDialAnimationPart = 7;
    dialAdressState.dialAnimationPartTime = Time;
    break;

    // 7. Add a delay
  case 7:
    if (Time > dialAdressState.dialAnimationPartTime + 1500)
    {
      dialAdressState.currentDialAnimationPart = 0;
      dialAdressState.dialAnimationPartTime = Time;
      dialAdressState.currentChveronLocked = true;
    }
    break;

  default:
    break;
  }
}