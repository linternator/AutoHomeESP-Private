/// Fast LED
/// - https://github.com/FastLED/FastLED
/// - http://fastled.io/docs/

#include <FastLED.h>
#include <AutoHome.h>

int Mode = 1;


 unsigned long current_time;

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


// LED strip pins

#define GATE_LED_PIN    0
#define STAIRS_LED_PIN  5

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define GATE_LED_COUNT 63
#define STAIRS_LED_COUNR  12 // plus actual sairs

CRGB ledstrip1[GATE_LED_COUNT];
CRGB ledstrip2[STAIRS_LED_COUNR];

#define FRAMES_PER_SECOND 60

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

    if (autohome.getValue(packet, ':', 0).equals("Brightness"))
    {
    //  BRIGHTNESS = autohome.getValue(packet, ':', 1).toInt();
    //  FastLED.setBrightness(BRIGHTNESS);
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("SATURATION"))
    {
    //  SATURATION = autohome.getValue(packet, ':', 1).toInt();
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("HUE"))
    {
    //  HUE = autohome.getValue(packet, ':', 1).toInt();
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("SPREAD"))
    {
    //  SPREAD = autohome.getValue(packet, ':', 1).toFloat();
    //  mqtt_send_stats();
    }


    if (autohome.getValue(packet, ':', 0).equals("RAINBOW_SCALE"))
    {
    //  RAINBOW_SCALE = autohome.getValue(packet, ':', 1).toFloat();
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("RAINBOW_SPEED"))
    {
    //  RAINBOW_SPEED = autohome.getValue(packet, ':', 1).toFloat();
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("stat"))
    {
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("UseRando")) // 0 1, if lights use random animation
    {
    //  UseRando = autohome.getValue(packet, ':', 1).toInt();
    //  mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("mode"))   // lighting mode
    {
    //  Mode = autohome.getValue(packet, ':', 1).toFloat();
    //  mqtt_send_stats();
    }

        if (autohome.getValue(packet, ':', 0).equals("buzz"))
    {

    
    // if (autohome.getValue(packet, ':', 0).equals("time_counter"))
    //  {
    //    time_counter = autohome.getValue(packet, ',', 1).toInt();
    //    mqtt_send_stats();
    //  }

    // if (autohome.getValue(packet, ':', 0).equals("SET"))
    //  {
    //   WW = autohome.getValue(packet, ',', 1).toInt();
    //   CW = autohome.getValue(packet, ',', 2).toInt();
    //   AM = autohome.getValue(packet, ',', 3).toInt();

    //    mqtt_send_stats();
    //    changeColourRequested = true;
    //  }
  }
}}

void mqtt_send_stats()
{
  String packet = "living room lights: "
                  "brightness = " +
                  String("BRIGHTNESS") + ", "
                                       "SATURATION = " +
                  String("SATURATION") + ", "
                                       "RAINBOW_SCALE = " +
                  String("RAINBOW_SCALE") + ", "
                                          "switch stat = " +
                //  (digitalRead("ON_OFF_SWITCH_PIN")) + ", "
                  + "Mode = " + String(Mode) + "";
  autohome.sendPacket(packet.c_str());
}


void rainbow(unsigned long time_counter)
{
  fill_my_rainbow(ledstrip1, time_counter, 0);
  fill_my_rainbow(ledstrip2, time_counter, 64);
}

void SetColor(struct CRGB *leds, const struct CHSV &color)
{
  for (int i = 0; i < GATE_LED_COUNT; i++)
  {
    leds[i] = color;
  }
}

void fill_my_rainbow(struct CRGB *leds, unsigned long time_counter, int offset)
{
  for (int i = 0; i < GATE_LED_COUNT; i++)
  {
    /* The higher the value 4 the less fade there is and vice versa */
    uint8_t hue = (uint8_t)(i * 100 + time_counter * 80);
    leds[i] = CHSV(hue + 12, 255, 255);
  }
}

// Lions Random Mode
void Rando(struct CRGB *leds)
{
  int Strippy = random(0,4);    // prick a random strip to pester
  int i = random(0, GATE_LED_COUNT); 
  leds[i] = CHSV(random(200 - 100 * 128, 200 + 100 * 128), 200, random(0, 255));

}



void setup()
{
  delay(500);

  Serial.begin(115200);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, GATE_LED_PIN, COLOR_ORDER>(ledstrip1, GATE_LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, STAIRS_LED_PIN, COLOR_ORDER>(ledstrip2, GATE_LED_COUNT).setCorrection(TypicalLEDStrip);

  // set master brightness control
  // FastLED.setBrightness(BRIGHTNESS);
  FastLED.setBrightness(200);


  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
}

unsigned long last_update_time = 0;
unsigned long time_counter = 255; // rotating "base color" used by many of the patterns

void loop()
{
  autohome.loop();
  current_time = millis();
      
if (abs(last_update_time - current_time) > 1000 / FRAMES_PER_SECOND)
  {
    last_update_time = current_time;
    time_counter += 1;

      switch(Mode)      // diffrent on animations.
        {
          case 1:
            {
              rainbow(time_counter);
            } break;

          case 2:
            {
//              rainbow(time_counter);
            } break;

         case 3:
          {

              CHSV color = CHSV(200, 200, 200);
              SetColor(ledstrip1, color);
              SetColor(ledstrip2, color);
          } break;

         case 4:   // the random case. 
          {
            Rando(ledstrip1);
            Rando(ledstrip2);
          } break;

        }

    }

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
  }
