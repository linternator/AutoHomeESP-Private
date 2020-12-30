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

    void HandleMqttMessage(String packet)
    {
        String property = autohome.getValue(packet, MQTT_DELIMITER, 1);
        String value = autohome.getValue(packet, MQTT_DELIMITER, 2);

        if (property.equals("SATURATION"))
        {
            SATURATION = value.toInt();
        }
        else if (property.equals("HUE"))
        {
            HUE = value.toInt();
        }
        else if (property.equals("SPREAD"))
        {
            SPREAD = value.toFloat();
        }
    }

    String ToMqttMessage()
    {
        String msg = "SATURATION: " + String(SATURATION) +
                     ", HUE: " + String(HUE) +
                     ", SPREAD: " + String(SPREAD);
        return msg;
    }
};
