/// Fast LED
/// - https://github.com/FastLED/FastLED
/// - http://fastled.io/docs/

#include <FastLED.h>
#include <AutoHome.h>

int Mode = 1;
int rando = 0;
int UseRando = 1;
int State = 0;
int NState = 0;

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

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
CRGB ledstrip1[NUM_LEDS];
CRGB ledstrip2[NUM_LEDS];
CRGB ledstrip3[NUM_LEDS];
CRGB ledstrip4[NUM_LEDS];

// #define BRIGHTNESS 255
volatile int BRIGHTNESS = 255;
volatile int SATURATION = 255;
volatile float RAINBOW_SCALE = 0.5;
volatile float RAINBOW_SPEED = 1;
#define FRAMES_PER_SECOND 20

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
      BRIGHTNESS = autohome.getValue(packet, ':', 1).toInt();
      FastLED.setBrightness(BRIGHTNESS);
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("SATURATION"))
    {
      SATURATION = autohome.getValue(packet, ':', 1).toInt();
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("RAINBOW_SCALE"))
    {
      RAINBOW_SCALE = autohome.getValue(packet, ':', 1).toFloat();
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("RAINBOW_SPEED"))
    {
      RAINBOW_SPEED = autohome.getValue(packet, ':', 1).toFloat();
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("stat"))
    {
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("UseRando")) // 0 1, if lights use random animation
    {
      UseRando = autohome.getValue(packet, ':', 1).toInt();
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("mode"))   // lighting mode
    {
      Mode = autohome.getValue(packet, ':', 1).toFloat();
      mqtt_send_stats();
    }
    
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
}

void mqtt_send_stats()
{
  String packet = "living room lights: "
                  "brightness = " +
                  String(BRIGHTNESS) + ", "
                                       "SATURATION = " +
                  String(SATURATION) + ", "
                                       "RAINBOW_SCALE = " +
                  String(RAINBOW_SCALE) + ", "
                                          "switch stat = " +
                  (digitalRead(ON_OFF_SWITCH_PIN)) + ", "
                  + "Mode = " + String(Mode) + "";
  autohome.sendPacket(packet.c_str());
}

void setup()
{
  delay(500);

  Serial.begin(115200);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, LED_STRIP_1_PIN, COLOR_ORDER>(ledstrip1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_2_PIN, COLOR_ORDER>(ledstrip2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_3_PIN, COLOR_ORDER>(ledstrip3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_STRIP_4_PIN, COLOR_ORDER>(ledstrip4, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // On/Off button
  pinMode(ON_OFF_SWITCH_PIN, INPUT_PULLUP);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
}

unsigned long last_update_time = 0;
unsigned long time_counter = 0; // rotating "base color" used by many of the patterns

void loop()
{
  autohome.loop();
  unsigned long current_time = millis();

  if((State == 1) & (NState == 0) && (UseRando == 1))  // only triguer once on lights turning on.
    {
      NState = 1;
      Mode = random(1,5);      // select a random number between 1 and 3, to pick a rnadom animation on power on.
    }

  if(State == 0)
    {
      NState = 0;
    }


      
if (abs(last_update_time - current_time) > 1000 / FRAMES_PER_SECOND)
  {
    last_update_time = current_time;
    time_counter += 1;
    
    if (digitalRead(ON_OFF_SWITCH_PIN) == LOW)
    {

      switch(Mode)      // diffrent on animations.
        {
          case 1:
            {
              rainbow(time_counter);
            } break;

          case 2:
            {
              rainbow(time_counter);
              addGlitter(ledstrip1, 75);
              addGlitter(ledstrip2, 75);
              addGlitter(ledstrip3, 75);
              addGlitter(ledstrip4, 75);    
            } break;

         case 3:
          {
              CRGB color = CRGB(0x00,0x00,0xff);
              SetColor(ledstrip1, CRGB(0xFF,0x00,0x00));
              SetColor(ledstrip2, CRGB(0x00,0xFF,0x00));
              SetColor(ledstrip3, CRGB(0x00,0x00,0xFF));
              SetColor(ledstrip4, CRGB(0xFF,0xFF,0xFF));
          } break;

        case 4:   // the random case. 
          {
            Rando(ledstrip1);
            Rando(ledstrip2);
            Rando(ledstrip3);
            Rando(ledstrip4);
          } break;

          
        }


      State = 1;
    }
    else
    {
      SetColor(ledstrip1, CRGB::Black);
      SetColor(ledstrip2, CRGB::Black);
      SetColor(ledstrip3, CRGB::Black);
      SetColor(ledstrip4, CRGB::Black);

      FastLED.clear(); // clear buffer to make MY random animaiton work better. 

      State = 0;
    }

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
  }
}

void rainbow(unsigned long time_counter)
{
  fill_my_rainbow(ledstrip1, time_counter, 0);
  fill_my_rainbow(ledstrip2, time_counter, 64);
  fill_my_rainbow(ledstrip3, time_counter, 128);
  fill_my_rainbow(ledstrip4, time_counter, 192);
}

void SetColor(struct CRGB *leds, const struct CRGB &color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
}

void fill_my_rainbow(struct CRGB *leds, unsigned long time_counter, int offset)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    /* The higher the value 4 the less fade there is and vice versa */
    uint8_t hue = (uint8_t)(i * RAINBOW_SCALE + time_counter * RAINBOW_SPEED);
    leds[i] = CHSV(hue + offset, SATURATION, BRIGHTNESS);
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

// Lions Random Mode
void Rando(struct CRGB *leds)
{
  int Strippy = random(0,4);    // prick a random strip to pester
  int i = random(0, NUM_LEDS); 

leds[i] = CRGB( random(0,255), random(0,255), random(0,255));

}
