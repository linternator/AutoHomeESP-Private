#include "LivingRoom.h"

class SetColorAnimation : public BaseAnimation
{
public:
    int SATURATION = 255;
    int HUE = 0;

    void Update(LivingRoomLeds *leds)
    {
        CHSV color = CHSV(HUE, SATURATION, leds->Brightness);
        SetColor(leds->Strip1, color);
        SetColor(leds->Strip2, color);
        SetColor(leds->Strip3, color);
        SetColor(leds->Strip4, color);
    }

    void SetColor(CRGB *leds, const struct CHSV &color)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = color;
        }
    }
};
