#include "LivingRoom.h"

class LionRandomAnimation : public BaseAnimation
{
public:
    int SATURATION = 255;
    int HUE = 0;
    float SPREAD = 0.1;

    void Update(LivingRoomLeds *leds)
    {
        Rando(leds->Strip1, leds->Brightness);
        Rando(leds->Strip2, leds->Brightness);
        Rando(leds->Strip3, leds->Brightness);
        Rando(leds->Strip4, leds->Brightness);
    }

    // Lions Random Mode
    void Rando(struct CRGB *leds, uint8_t brightness)
    {
        int Strippy = random(0, 4); // prick a random strip to pester
        int i = random(0, NUM_LEDS);
        leds[i] = CHSV(random(HUE - SPREAD * 128, HUE + SPREAD * 128), SATURATION, random(0, brightness));
    }
};
