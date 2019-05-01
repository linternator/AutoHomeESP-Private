/* This example is a simple setup example to show you how to setup the AutoHome library. */

#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 14

int speed = 25;
int col = 450;
int m0de = 1;

AutoHome autohome;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(45, PIN, NEO_GRB + NEO_KHZ800);

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

/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {


  String packet = "";
  
  if (!autohome.mqtt_callback(topic, payload, length)) {

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    if (getValue(packet, ':', 0).equals("spEEd")) {   // speed
      speed = getValue(packet, ':', 1).toInt();
    }


        if (getValue(packet, ':', 0).equals("colour")) { // colour
      col = getValue(packet, ':', 1).toInt();
    }


            if (getValue(packet, ':', 0).equals("mode")) { // mode
      m0de = getValue(packet, ':', 1).toInt();
    }

            if (getValue(packet, ':', 0).equals("stat")) { // stat
              
          String packet = "warp-core speed:" + String(speed) + ", col:" + String(col) + ", mode:" + String(m0de);
          autohome.sendPacket( packet.c_str() );      
    }



    Serial.print(packet);

  }

}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);


  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  strip.begin();
  colorWipe(strip.Color(0, 255, 0), 50); // Red
  strip.show(); // Initialize all pixels to 'off'

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

switch( m0de)
  {
    case 0:
      {
        //set all pixels to off
         strip.begin();
         colorWipe(strip.Color(0, 0, 0), 50); // Red
         strip.show(); // Initialize all pixels to 'off'
      } break;

   case 1:
      {
         Warp( speed ); // warp speed! 
      } break;

   case 2:
      {
        // strobe
         strip.begin();
         colorWipe(strip.Color(255, 255, 189), 50); // Red
         strip.show(); // Initialize all pixels to 'off'
      } break;

    case 3:   // no dosent work 
      {
        // some othere paturn
      //   strip.begin();
         theaterChaseRainbow(speed);
     //   strip.show(); // Initialize all pixels to 'off'
      } break;

    case 4:
      {
        // some othere paturn
      //   strip.begin();
        rainbowCycle(speed);
     //   strip.show(); // Initialize all pixels to 'off'
      } break;



  }

  delay(0);


}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
      autohome.loop();
    delay(wait);
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
  autohome.loop();
      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void Warp(uint8_t facotr) {

  strip.clear();
  
  for (int i = -5; i <= 20; i++)
  {
    strip.setPixelColor(i,  0, 0, 0);                     // top part of the warp core
    strip.setPixelColor(i + 1, Wheel((col) & 255)); //

    strip.setPixelColor(43 -  i, Wheel((col) & 255)); //  // bottom part of the warp core
    strip.setPixelColor(44 -  i,  0, 0, 0);

    for (int L = 20; L <= 24; L++)                    // intermix chmaber glows birhgter.
    {
              strip.setPixelColor(L, 255,255,140);
    }

    delay(facotr);
    strip.show();

  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
