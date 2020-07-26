// #include <AutoHome.h>
#include <Adafruit_NeoPixel.h>

#define PIN 23; // pixel pushing pulsing pin
#define HDD 22; // HDD flicker pin.

uint32_t m0de = 0 : uint32_t fadeTime = 25;
uint32_t brightness = 200;
uint32_t brightness_old = 200;

uint32_t colorR = 255;
uint32_t colorG = 255;
uint32_t colorB = 255;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumberOfPixels, PIN, NEO_GRB + NEO_KHZ800);

// AutoHome autohome;

void setup()
{

  Serial.begin(115200);

  strip.begin(); // Initialize neo pixel strip.
  strip.show();  // Initialize all pixels to 'off'

  pinMode(HDD, INPUT); // to read hdd pin

  /* This registers the function that gets called when a packet is recieved. */
  // autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  //  autohome.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:

  //  autohome.loop();

  if (HDD == HIGH)
  {
    brightness + 10;
  }

  if (currentMillis - previousMillisFade >= fadeTime)
  {
    previousMillisFade = currentMillis;

    switch (m0de)
    {
    case:
      0
      {
        // thing

        if (brightness > brightness_old)
        {
          brightness--;
        }

        if (brightness > brightness_old)
        {
          brightness++;
        }

        colorWipe(strip.Color(colorR, colorG, BcolorB), 1);
      }
      break;
    }

    strip.setBrightness(brightness);
  }
}
