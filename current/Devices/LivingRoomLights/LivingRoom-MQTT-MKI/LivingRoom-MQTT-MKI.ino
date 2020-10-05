/// Fast LED
/// - https://github.com/FastLED/FastLED
/// - http://fastled.io/docs/

#include <FastLED.h>
#include <AutoHome.h>

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

 if (autohome.getValue(packet, ':', 0).equals("stat"))
  {
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
} }

void mqtt_send_stats()
{
  String packet = "living room lights ... ,brightness = " + String(BRIGHTNESS) + ". switch stat = " + (digitalRead(ON_OFF_SWITCH_PIN)) + ".";
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
uint8_t time_counter = 0; // rotating "base color" used by many of the patterns

void loop()
{
  autohome.loop();
  unsigned long current_time = millis();

  if(abs(last_update_time - current_time) > 1000 / FRAMES_PER_SECOND)
  {
    last_update_time = current_time;
    time_counter += 1;
    if (digitalRead(ON_OFF_SWITCH_PIN) == LOW)
    {
      rainbow(time_counter);
      //CRGB color = CRGB(0x00,0x00,0xff);
      //SetColor(ledstrip1, color);
      //SetColor(ledstrip2, color);
      //SetColor(ledstrip3, color);
      //SetColor(ledstrip4, color);
    }
    else
    {
      SetColor(ledstrip1, CRGB::Black);
      SetColor(ledstrip2, CRGB::Black);
      SetColor(ledstrip3, CRGB::Black);
      SetColor(ledstrip4, CRGB::Black);
    }

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
  }
}

void rainbow(uint8_t value)
{
  // FastLED's built-in rainbow generator
  fill_rainbow(ledstrip1, NUM_LEDS, value, 7);
  fill_rainbow(ledstrip2, NUM_LEDS, value + 64, 7);
  fill_rainbow(ledstrip3, NUM_LEDS, value + 128, 7);
  fill_rainbow(ledstrip4, NUM_LEDS, value + 192, 7);
}

void SetColor(struct CRGB *leds, const struct CRGB &color)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
}
