#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2     // pixel pushing pulsing pin
#define HDD 4     // HDD flicker pin.
#define pwLed 16 // power on led pin
#define pwSw  17 // power on off button

// touch buttons
#define OnOff 13 // on off

#define ooff 12  // leds off touch button
#define act 14  // normal hdd activity lighting

#define FULL 27 // full on
#define Fade 33 // smoothed hdd lighting
#define Live 32 // heart beat

#define NumberOfPixels 55

int m0de = 1;
int fadeTime = 25;
int brightness = 200;
int brightness_old = 200;

int colorR = 255;
int colorG = 255;
int colorB = 255;

Adafruit_NeoPixel strip(NumberOfPixels, PIN, NEO_GRB + NEO_KHZ800);

// AutoHome autohome;

void setup()
{
  Serial.begin(115200);

  strip.begin(); // Initialize neo pixel strip.
  strip.show();  // Initialize all pixels to 'off'

  pinMode(HDD, INPUT); // to read hdd pin
  pinMode(pwLed, INPUT);
  pinMode(pwSw, OUTPUT); // SET defult stat to not turn off the server ? 

  /* This registers the function that gets called when a packet is recieved. */
//   autohome.setPacketHandler(mqtt_callback);    // do i need ? 

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
//    autohome.begin();
}

void loop()
{
  strip.show();  // show updated pixels
  //  autohome.loop();  // check for MQTT messages

  // delay(1);  // service things like wifi

//    touchValue = touchRead(touchPin); // reach touch buttons, returns analog value
  if(touchRead(ooff) > 50 ) // touch button pushed. 
  {
    m0de = 0; // OFF
  }

  if(touchRead(act) > 50 ) // touch button pushed. 
  {
    m0de = 1; // activity
  }

  if(touchRead(FULL) > 50 ) // touch button pushed. 
  {
    m0de = 2; // full on
  }

  if(touchRead(Fade) > 50 ) // touch button pushed. 
  {
    m0de = 3; // fade
  }

  if(touchRead(Live) > 50 ) // touch button pushed. 
  {
    m0de = 4; // live (heart beat)
  }

// consider putting this in a milis timed thing of fadeTime

switch(m0de)    // switch the mode for the LEDs
  {
    case 0: // turn LEDs off
      {
        brightness = brightness - 1;
          strip.setBrightness(brightness);
      } break;

    case 1: // normal HDD activieyt flashing
      {
        brightness = 255;
        if( digitalRead(HDD) )
          {
            strip.Color(colorR, colorG, colorB);   // turn LEDS full on
          }
            else
            {
              strip.Color(0,0,0);                   // turn LEDS full off
            }
      } break;

    case 2:   // full on couloru
    {
       brightness = 255;
       strip.Color(colorR, colorG, colorB);   // turn LEDS full on
    } break;


    case 3: // HDD activty smooth
    {
      if( digitalRead(HDD))
        {
          if(brightness <= 255) // prevent over flow
              {
                brightness = brightness + 30; // 30 is the brightness jump it makes
              }
          
        } 
          else 
          {
            brightness = brightness -1;   // fade down
          }
          
    } break;

    case 4: // live // heart beat
    {
      // code
    } break;
  }


}






// old code 

//  if (HDD == HIGH)
//  {
//    brightness + 10;
//  }
//
//  if (currentMillis - previousMillisFade >= fadeTime)
//  {
//    previousMillisFade = currentMillis;
//
//    switch (m0de)
//    {
//    case:
//      0
//      {
//        // thing
//
//        if (brightness > brightness_old)
//        {
//          brightness--;
//        }
//
//        if (brightness > brightness_old)
//        {
//          brightness++;
//        }
//
//        colorWipe(strip.Color(colorR, colorG, BcolorB), 1);
//      }
//      break;
//    }
//
//    strip.setBrightness(brightness);
//  }
