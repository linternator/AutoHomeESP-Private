/// Fast LED
/// - https://github.com/FastLED/FastLED
/// - http://fastled.io/docs/

#include <FastLED.h>

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN_STRIP1 12
#define DATA_PIN_STRIP2 14
#define DATA_PIN_STRIP3 27
#define DATA_PIN_STRIP4 26
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 179
CRGB ledstrip1[NUM_LEDS];
CRGB ledstrip2[NUM_LEDS];
CRGB ledstrip3[NUM_LEDS];
CRGB ledstrip4[NUM_LEDS];

#define BRIGHTNESS 255
#define FRAMES_PER_SECOND 120

void setup()
{
  delay(500);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN_STRIP1, COLOR_ORDER>(ledstrip1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_STRIP2, COLOR_ORDER>(ledstrip2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_STRIP3, COLOR_ORDER>(ledstrip3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_STRIP4, COLOR_ORDER>(ledstrip4, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

uint8_t time_counter = 0; // rotating "base color" used by many of the patterns

void loop()
{
  
  //experiment(ledstrip1, CRGB(150,0,200));
  //experiment(ledstrip2, CRGB(0,255,150));
  //experiment(ledstrip3, CRGB(50,90,100));
  //experiment(ledstrip4, CRGB(0,135,50));
  
  rainbow(time_counter);
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(20) { time_counter += 10; } // slowly cycle the "base color" through the rainbow
}

void rainbow(uint8_t value)
{
  // FastLED's built-in rainbow generator
  fill_rainbow(ledstrip1, NUM_LEDS, value, 7);
  fill_rainbow(ledstrip2, NUM_LEDS, value + 64, 7);
  fill_rainbow(ledstrip3, NUM_LEDS, value + 128, 7);
  fill_rainbow(ledstrip4, NUM_LEDS, value + 192, 7);
}

void SetColor(struct CRGB * leds, const struct CRGB& color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
}
