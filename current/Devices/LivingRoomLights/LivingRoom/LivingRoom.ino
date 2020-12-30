/// Fast LED
/// - https://github.com/FastLED/FastLED
/// - http://fastled.io/docs/

#include "LivingRoom.h"
#include <WiFiManager.h>
#include "LionRandomAnimation.cpp"
#include "HeatBlobAnimation.cpp"
#include "RainbowAnimaion.cpp"
#include "SetColorAnimation.cpp"

volatile int rando = 0;
volatile int UseRando = 1;
volatile int State = 0;
volatile int NState = 0;
volatile unsigned long last_update_time = 0;

LivingRoomLeds *Leds = new LivingRoomLeds;
volatile int numberOfAnimations = 5;
BaseAnimation **Animations = new BaseAnimation *[numberOfAnimations];
AutoHome autohome;

/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  if (!autohome.mqtt_callback(topic, payload, length))
  {
    String packet = "";
    for (int i = 0; i < length; i++)
    {
      packet = packet + (char)payload[i];
    }
    Serial.print(packet);

    Serial.println("]");
    String propertyName = autohome.getValue(packet, MQTT_DELIMITER, 0);
    if (propertyName.equals("Brightness"))
    {
      Leds->Brightness = autohome.getValue(packet, MQTT_DELIMITER, 1).toInt();
      FastLED.setBrightness(Leds->Brightness);
      mqtt_send_stats();
    }
    else if (propertyName.equals("UseRando")) // 0 1, if lights use random animation
    {
      UseRando = autohome.getValue(packet, MQTT_DELIMITER, 1).toInt();
      mqtt_send_stats();
    }
    else if (propertyName.equals("stat")) // report stats
    {
      mqtt_send_stats();
    }
    else if (propertyName.equals("mode")) // lighting mode
    {
      Leds->Mode = autohome.getValue(packet, MQTT_DELIMITER, 1).toInt();
      mqtt_send_stats();
    }
    else
    {
      for (int i = 0; i < numberOfAnimations; i++)
      {
        if (propertyName.equals(Animations[i]->Name))
        {
          Animations[i]->HandleMqttMessage(packet);
          mqtt_send_stats();
          break;
        }
      }
    }
  }
}

void mqtt_send_stats()
{
  String packet = "Living room lights: "
                  "Brightness: " +
                  String(Leds->Brightness) +
                  ", Switch stat: " + String(digitalRead(ON_OFF_SWITCH_PIN)) +
                  ", Mode: " + String(Leds->Mode) +
                  ", Frame: " + String(Leds->Frame) +
                  ", Time: " + String(Leds->Time);

  for (int i = 0; i < numberOfAnimations; i++)
  {
   packet = packet + ", " + Animations[i]->Name + "(" + String(i) + "): <" + Animations[i]->ToMqttMessage() + ">";
  }
  autohome.sendPacket(packet.c_str());
}

void setup()
{

  delay(500);

  Serial.begin(115200);
  Serial.println("setup()...");
  Serial.println("Autohome init");
  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
  Serial.println("Autohome Done");

  // Create the animations objects
  // Raw color animation
  Animations[0] = new SetColorAnimation;
  Animations[0]->Name = "SetColor";

  //Normal rainbow animation
  Animations[1] = new RainbowAnimaion;
  Animations[1]->Name = "Rainbow";

  // Rain bow with glitter
  RainbowAnimaion *rainbowWithGlitterAnimaion = new RainbowAnimaion;
  rainbowWithGlitterAnimaion->EnableGlitter = true;
  Animations[2] = rainbowWithGlitterAnimaion;
  Animations[2]->Name = "Glitter Rainbow";

  // Lion rando animation
  Animations[3] = new LionRandomAnimation;
  Animations[3]->Name = "Lion Rando";

  // Heatblob animation
  Animations[4] = new HeatBlobAnimation();
  Animations[4]->Name = "Heat Blobs";

  Serial.println("Animations initialized");

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, LED_STRIP_1_PIN, COLOR_ORDER>(Leds->Strip1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_2_PIN, COLOR_ORDER>(Leds->Strip2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_3_PIN, COLOR_ORDER>(Leds->Strip3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_4_PIN, COLOR_ORDER>(Leds->Strip4, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(Leds->Brightness);
  // On/Off button
  pinMode(ON_OFF_SWITCH_PIN, INPUT_PULLUP);
}

void SetColor(CRGB *leds, CRGB &color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
}

void loop()
{
  autohome.loop();

  Leds->Time = millis();
  if ((State == 1) & (NState == 0) && (UseRando == 1)) // only triguer once on lights turning on.
  {
    NState = 1;
    Leds->Mode = random(0, numberOfAnimations); // select a random number between 0 and numberOfAnimations, to pick a random animation on power on.
  }

  if (State == 0)
  {
    NState = 0;
  }
  if (abs(last_update_time - Leds->Time) > 1000 / FRAMES_PER_SECOND)
  {
    last_update_time = Leds->Time;
    Leds->Frame += 1;
    if (digitalRead(ON_OFF_SWITCH_PIN) == LOW)
    {
      // Ensure we do not run a non-existing animation
      if (Leds->Mode < numberOfAnimations)
      {
        Animations[Leds->Mode]->Update(Leds);
      }
      State = 1;
    }
    else
    {
      CRGB black = CRGB(0x000000);
      SetColor(Leds->Strip1, black);
      SetColor(Leds->Strip2, black);
      SetColor(Leds->Strip3, black);
      SetColor(Leds->Strip4, black);

      FastLED.clear(); // clear buffer to make random animaiton work better.
      State = 0;
    }

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
  }
}
