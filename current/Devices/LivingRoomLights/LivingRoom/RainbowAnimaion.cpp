#include "LivingRoom.h"

class RainbowAnimaion : public BaseAnimation
{
public:
    float RAINBOW_SCALE = 0.2;
    float RAINBOW_SPEED = 0.02;
    int SATURATION = 255;
    int HUE = 0;
    bool EnableGlitter = false;

    void Update(LivingRoomLeds *leds)
    {
        fill_my_rainbow(leds->Strip1, 0, leds->Time, leds->Brightness);
        fill_my_rainbow(leds->Strip2, 64, leds->Time, leds->Brightness);
        fill_my_rainbow(leds->Strip3, 128, leds->Time, leds->Brightness);
        fill_my_rainbow(leds->Strip4, 192, leds->Time, leds->Brightness);
        if (EnableGlitter)
        {
            addGlitter(leds->Strip1, 75);
            addGlitter(leds->Strip2, 75);
            addGlitter(leds->Strip3, 75);
            addGlitter(leds->Strip4, 75);
        }
    }

    void fill_my_rainbow(struct CRGB *strip, int offset, unsigned long Time, uint8_t Brightness)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            /* The higher the value 4 the less fade there is and vice versa */
            uint8_t hue = (uint8_t)(i * RAINBOW_SCALE + Time * RAINBOW_SPEED);
            strip[i] = CHSV(hue + offset, SATURATION, Brightness);
        }
    }

    //glitter effect  (addGlitter(ledstrip1, 75);)
    void addGlitter(struct CRGB *leds, fract8 chanceOfGlitter)
    {
        if (random8() < chanceOfGlitter)
        {
            leds[random16(NUM_LEDS)] += CRGB::White;
        }
    }
};
