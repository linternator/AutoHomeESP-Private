#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>


#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12
 String packet = "";

 int mode = 1;
 int speed = 1;

 int NumberOfPixels = 33;

 int colorR = 0;
 int colorG = 255;
 int colorB = 0;

int   prevR = 0;
int   prevG = 0;
int   prevB = 0;
int   prevW = 0;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumberOfPixels, PIN, NEO_GRB + NEO_KHZ800);

// 11 pixels per "strip" 5 strips.

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

AutoHome autohome;

/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length){

  if(!autohome.mqtt_callback(topic, payload, length))
  {

      String packet = "";

      for (int i = 0; i < length; i++) 
      {
        packet = packet + (char)payload[i];
      }

      Serial.print(packet); // display packet on the serial for debug.

      if (getValue(packet, ':', 0).equals("mode"))  // set mode of light strip
        {
          mode = getValue(packet,':', 1).toInt();

            switch(mode)
            {
              case 0: // off
                {
                  String packet = "OFF:";
                } break;

              case 1: // solid coulor
                {
        //          autohome.sendPacket( "RGB:" + "," + colorR + "," + colorG + "," + colorB );
        //           String packet = "RGB:" + char(colorR) + "," + char(colorG) + "," + char(colorB);
                } break;

              case 2: // rainbow!
                {
          //        String packet = "rainbowCycle:" + "," + speed;
                } break;

              case 3: // lesser rainbow
                {
         //         String packet = "rainbow:" + "," + speed;
                } break;

              case 4: // theater chase rainbow
                {
         //         String packet = "theaterChaseRainbow:" + "," + speed;
                } break;

              case 5: // vue-screen
                {
         //         String packet = "theaterChaseRainbow:" + "," + speed;
                } break;
            }
        }
      
      if (getValue(packet, ':', 0).equals("speed")) // set speed of light strip
        {
          speed = getValue(packet,':', 1).toInt();

          String packet = "speed:" + String(speed);
        }

      if (getValue(packet, ':', 0).equals("set"))   // set color of light strip
        {
            colorR = getValue(packet,':', 1).toInt();
            colorG = getValue(packet,':', 2).toInt();
            colorB = getValue(packet,':', 3).toInt();

            String packet = "SET:" + String(colorR) + "," + String(colorG) + "," + String(colorB);
        }
          
  }

 //   autohome.sendPacket( packet.c_str() );          // report back what stat the lights are in
    Serial.print(packet);
  
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

   strip.begin();   // Initialize neo pixel strip.
  strip.show(); // Initialize all pixels to 'off'

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

   // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code



}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  switch(mode)
    {
      case 0: // off
        {
          colorWipe(strip.Color(0, 0, 0), 1); // get this to fade to clouor instead
        } break;

      case 1: // solid coulor
        {
          colorWipe(strip.Color(colorR, colorG , colorB), speed); // get this to fade to clouor instead
        } break;

      case 2: // rainbow!
        {
          rainbowCycle(speed);          // modifie to be non blocking code.
        } break;

      case 3: // lesser rainbow
        {
          rainbow(speed);
        } break;

      case 4: // theater chase rainbow
        {
          theaterChaseRainbow(speed);
        } break; 
    }
}

//void process_fade(long currentMillis) {
//
//  if (fade) {
//
//    SETRGB(prevR,prevG,prevB,prevW);
//
//    if (currentMillis - previousMillisFade >= fadeTime) {
//
//      previousMillisFade = currentMillis;
//
//      if (prevR < colorR) {
//        prevR++;
//      }
//
//      if (prevG < colorG) {
//        prevG++;
//      }
//
//      if (prevB < colorB) {
//        prevB++;
//      }
//
//      if (prevR > colorR) {
//        prevR--;
//      }
//
//      if (prevG > colorG) {
//        prevG--;
//      }
//
//      if (prevB > colorB) {
//        prevB--;
//      }
//
//      if (prevB > colorB) {
//        prevB--;
//      }
//
//      if (prevR == colorR && prevG == colorG && prevB == colorB) {
//
//        fade = false;
//        previousMillisFade = 0;
//
//        SETRGB(prevR,prevG,prevB);
//
//        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
//        autohome.sendPacket( packet.c_str() );
//      }
//    }
//  }
//}

//void SETRGB(int r, int g, int b, int w) {
//
//for(number of pixles, int X = 0, X++)
//{
//   // strip.setPixelColor(i, c);
//    strip.Color(r,g,b)
//    strip.show();
//}
//
//  prevR = r;
//  prevG = g;
//  prevB = b;
//  prevW = w;
//
//}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
