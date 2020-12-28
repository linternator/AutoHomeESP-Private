#include "LivingRoom.h"

class HeatBlobAnimation : public BaseAnimation
{
public:
    float heatstates[(NUM_LEDS * 4) + 1];
    float current;
    float last;

    void Initialize()
    {
        for (int i = 0; i < NUM_LEDS * 4; i++)
        {
            heatstates[i] = random(1000) / 1000.0;
        }
    }

    void Update(LivingRoomLeds *leds)
    {
        heatblobs(leds->Strip1, leds->Strip2, leds->Strip3, leds->Strip4);
    }

    //Heat blobs
    void heatblobs(struct CRGB *leds1, struct CRGB *leds2, struct CRGB *leds3, struct CRGB *leds4)
    {
        int skip = 10;
        for (int i = 0; i < NUM_LEDS * 4; i++)
        {
            if (skip != 0)
            {
                skip--;
            }

            current = heatstates[i];

            if ((current < 0.02) && skip == 0)
            {
                heatstates[i] = 1;
                skip = 50;
            }
            else
            {
                float next = heatstates[i + 1] * 0.1;
                float curlast = last * 0.1;
                if (i == 0)
                {
                    curlast = current * 0.1;
                }
                if (i == ((NUM_LEDS * 4) - 1))
                {
                    next = current * 0.1;
                }
                heatstates[i] = current * 0.798 + next + curlast;
            }

            last = current;
            int red = convertdown(heatstates[i] * 0.25);
            int green = convertdown(heatstates[i] * 0.25);
            int blue = convertdown(heatstates[i]);
            int hue = 255 - blue;
            int val = 255;
            if (hue > 192)
            {
                val = blue * 4;
            }

            if (hue > 170)
            {
                hue = 170;
            }

            switch (i / NUM_LEDS)
            {
            case 0:
            {
                leds1[i % NUM_LEDS] = CHSV(hue, 255, val);
                //leds1[i%NUM_LEDS] = CRGB(red, green, blue);
            }
            break;
            case 1:
            {
                leds2[i % NUM_LEDS] = CHSV(hue, 255, val);
                //leds2[i%NUM_LEDS] = CRGB(red, green, blue);
            }
            break;
            case 2:
            {
                leds3[i % NUM_LEDS] = CHSV(hue, 255, val);
                //leds3[i%NUM_LEDS] = CRGB(red, green, blue);
            }
            break;
            case 3:
            {
                leds4[i % NUM_LEDS] = CHSV(hue, 255, val);
                //leds4[i%NUM_LEDS] = CRGB(red, green, blue);
            }
            break;
            }
        }
    }

    int convertdown(float value)
    {
        value = value * 2048;
        int result = int(value);
        if (result > 255)
        {
            result = 255;
        }
        return result;
    }
};