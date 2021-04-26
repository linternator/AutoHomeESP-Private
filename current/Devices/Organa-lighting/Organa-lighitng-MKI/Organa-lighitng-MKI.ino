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

int m0de = 2;
int fadeTime = 25;
int brightness = 200;
int brightness_old = 200;

int colorR = 255;
int colorG = 255;
int colorB = 255;

Adafruit_NeoPixel strip(NumberOfPixels, PIN, NEO_GRB + NEO_KHZ400);

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
if (autohome.getValue(packet, ':', 0).equals("brightness"))
    {
      brightness = autohome.getValue(packet, ':', 1).toInt();
    //  FastLED.setBrightness(BRIGHTNESS);
      strip.setBrightness(brightness);
      mqtt_send_stats();
    }

    if (autohome.getValue(packet, ':', 0).equals("mode"))
    {
      m0de = autohome.getValue(packet, ':', 1).toInt();
      mqtt_send_stats();
    }


    if (autohome.getValue(packet, ':', 0).equals("stat"))
    {
      mqtt_send_stats();
    }
  }
}

void mqtt_send_stats()
{
  String packet = "Organa-lighitng: "
                  "m0de = " + String(m0de) + ", " + 
                  "brightness = " + String(brightness) + ", "
                    ;
  autohome.sendPacket(packet.c_str());
  Serial.println(packet);
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i< NumberOfPixels; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(1);                           //  Pause for a moment
  }
}

void setup()
{
  Serial.begin(115200);

  strip.begin(); // Initialize neo pixel strip.
  strip.show();  // Initialize all pixels to 'off'

  pinMode(HDD, INPUT_PULLUP); // to read hdd pin
  pinMode(pwLed, INPUT_PULLUP);
  pinMode(pwSw, OUTPUT); // SET defult stat to not turn off the server ? 

  /* This registers the function that gets called when a packet is recieved. */
   autohome.setPacketHandler(mqtt_callback);    // do i need ? 

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
    autohome.begin();
}


void loop()
{

    autohome.loop();  // check for MQTT messages

   delay(16);  // service things like wifi

//    touchValue = touchRead(touchPin); // reach touch buttons, returns analog value
//  if(touchRead(ooff) > 50 ) // touch button pushed. 
//  {
//    m0de = 0; // OFF
//  }
//
// if(touchRead(act) > 50 ) // touch button pushed. 
//  {
//    m0de = 1; // activity
//  }
//
//  if(touchRead(FULL) > 50 ) // touch button pushed. 
//  {
//    m0de = 2; // full on
//  }
//
//  if(touchRead(Fade) > 50 ) // touch button pushed. 
//  {
//    m0de = 3; // fade
//  }
//
//  if(touchRead(Live) > 50 ) // touch button pushed. 
//  {
//    m0de = 4; // live (heart beat)
//  }

switch(m0de)    // switch the mode for the LEDs
  {
    case 0: // turn LEDs off
      {
          colorWipe(strip.Color(  0, 0,   0), 0);
          //  strip.Color(0, 0, 0);
          //  strip.clear();
          //  strip.show();  // show updated pixels
      } break;

    case 1: // normal HDD activieyt flashing
      {
        brightness = 255;
        if( digitalRead(HDD) )
          {
            strip.Color(colorR, colorG, colorB);   // turn LEDS full on
              strip.show();  // show updated pixels
          }
            else
            {
              strip.Color(0,0,0);                   // turn LEDS full off
               strip.show();  // show updated pixels
              
            }
      } break;

    case 2:   // full on couloru
    {
       brightness = 255;
      // strip.Color(colorR, colorG, colorB);   // turn LEDS full on
      colorWipe(strip.Color(  0, 255,   0), 0); // Green
      //  strip.show();  // show updated pixels
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
      strip.clear();
      strip.show();
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
