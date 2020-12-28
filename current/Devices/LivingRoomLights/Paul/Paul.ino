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

float heatstates[(NUM_LEDS*4) + 1];
float current;
float last;

volatile int BRIGHTNESS = 100;
volatile int SATURATION = 255;
volatile int HUE = 0;
volatile float SPREAD = 0.1;

volatile float RAINBOW_SCALE = 0.01;
volatile float RAINBOW_SPEED = 0.2;
#define FRAMES_PER_SECOND 120

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

    if (autohome.getValue(packet, ':', 0).equals("HUE"))
    {
      HUE = autohome.getValue(packet, ':', 1).toInt();
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("SPREAD"))
    {
      SPREAD = autohome.getValue(packet, ':', 1).toFloat();
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
  for (int i = 0; i < NUM_LEDS*4; i++)
  {
    heatstates[i] = random(1000)/1000.0;
  }
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
      Mode = random(1,6);      // select a random number between 1 and 3, to pick a rnadom animation on power on.
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

              CHSV color = CHSV(HUE, SATURATION, BRIGHTNESS);
              SetColor(ledstrip1, color);
              SetColor(ledstrip2, color);
              SetColor(ledstrip3, color);
              SetColor(ledstrip4, color);
          } break;

        case 4:   // the random case. 
          {
            Rando(ledstrip1);
            Rando(ledstrip2);
            Rando(ledstrip3);
            Rando(ledstrip4);
          } break;

        case 5:
          {
            heatblobs(ledstrip1, ledstrip2, ledstrip3, ledstrip4);
          } break;

          
        }


      State = 1;
    }
    else
    {
      SetColor(ledstrip1, CHSV(0, 0, 0));
      SetColor(ledstrip2, CHSV(0, 0, 0));
      SetColor(ledstrip3, CHSV(0, 0, 0));
      SetColor(ledstrip4, CHSV(0, 0, 0));

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

void SetColor(struct CRGB *leds, const struct CHSV &color)
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
  
  
  leds[i] = CHSV(random(HUE - SPREAD * 128, HUE + SPREAD * 128), SATURATION, random(0, BRIGHTNESS));

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

//Heat blobs
void heatblobs(struct CRGB *leds1, struct CRGB *leds2, struct CRGB *leds3, struct CRGB *leds4)
{
  int skip = 10;
  for (int i = 0; i < NUM_LEDS*4; i++)
  {
    if(skip != 0)
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
      float next = heatstates[i+1]*0.1;
      float curlast = last*0.1;
      if(i == 0)
      {
        curlast = current*0.1;
      }
      if(i == ((NUM_LEDS*4)-1))
      {
        next = current*0.1;
      }
      heatstates[i] = current*0.798 + next + curlast;
    }

    last = current;
    int red = convertdown(heatstates[i]*0.25);
    int green = convertdown(heatstates[i]*0.25);
    int blue = convertdown(heatstates[i]);
    int hue = 255-blue;
    int val = 255;
    if (hue > 192)
    {
      val = blue*4;
    }

    if (hue > 170)
    {
      hue = 170;
    }
    
    switch(i/NUM_LEDS)
    {
      case 0:
      {
        leds1[i%NUM_LEDS] = CHSV(hue, 255, val);
        //leds1[i%NUM_LEDS] = CRGB(red, green, blue);
      } break;
      case 1:
      {
        leds2[i%NUM_LEDS] = CHSV(hue, 255, val);
        //leds2[i%NUM_LEDS] = CRGB(red, green, blue);
      } break;
      case 2:
      {
        leds3[i%NUM_LEDS] = CHSV(hue, 255, val);
        //leds3[i%NUM_LEDS] = CRGB(red, green, blue);
      } break;
      case 3:
      {
        leds4[i%NUM_LEDS] = CHSV(hue, 255, val);
        //leds4[i%NUM_LEDS] = CRGB(red, green, blue);
      } break;
    }

  }
}
