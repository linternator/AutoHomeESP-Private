#ifndef LIVING_ROOM_H
#define LIVING_ROOM_H

#include <FastLED.h>
FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code." + FASTLED_VERSION
#endif
#include <AutoHome.h>

// Pin to turn the lights on/off
#define ON_OFF_SWITCH_PIN 4

// LED strip pins
#define LED_STRIP_1_PIN 12
#define LED_STRIP_2_PIN 14
#define LED_STRIP_3_PIN 27
#define LED_STRIP_4_PIN 26
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 180

// Animation
#define FRAMES_PER_SECOND 120

// MQTT
#define MQTT_DELIMITER ':'

// Autohome
extern AutoHome autohome;

struct LivingRoomLeds
{
    /* Max brightness allowed, value between 0 and 255 */
    uint8_t Brightness = 100;

    /* The current animation player*/
    int Mode = 1;

    /* The current time in milliseconds */
    unsigned long Time;

    /* The current frame that is being drawn */
    unsigned long Frame;

    /* The LED strips */
    CRGB Strip1[NUM_LEDS];
    CRGB Strip2[NUM_LEDS];
    CRGB Strip3[NUM_LEDS];
    CRGB Strip4[NUM_LEDS];
};

// Base class
class BaseAnimation
{
public:
    String Name = "<NotSet>";
    virtual void Update(LivingRoomLeds *leds){};
    virtual void HandleMqttMessage(String packet){};
    virtual String ToMqttMessage() { return ""; };
};

#endif