/* This example is a simple setup example to show you how to setup the AutoHome library. */

#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 14

int speed = 25;

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

  if (!autohome.mqtt_callback(topic, payload, length)) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    if (getValue(packet, ':', 0).equals("spEEd")) {
      speed = getValue(packet, ':', 1).toInt();
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

  Warp( speed );

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void Warp(uint8_t facotr) {
  for (int i = -5; i <= 20; i++)
  {
    strip.setPixelColor(i,  0, 0, 0);
    strip.setPixelColor(i + 1, Wheel((10) & 255)); //
 //   strip.setPixelColor(i,  0, 0, 0);
    
    //    strip.setPixelColor(i+2, Wheel((25) & 255)); ///
    //    strip.setPixelColor(i+3, Wheel((50) & 255)); ////
    //    strip.setPixelColor(i+4, Wheel((25) & 255)); ///
    //    strip.setPixelColor(i+5, Wheel((10) & 255)); //

    for (int L = 20; L <= 24; L++)   // intermix chmaber glows birhgter.
    {
              strip.setPixelColor(L, 255,255,140);
    }


    strip.setPixelColor(44 -  i, Wheel((10) & 255)); //
    strip.setPixelColor(45 -  i,  0, 0, 0);
 //   strip.setPixelColor(44 -  i,  0, 0, 0);



    //     strip.setPixelColor(38 -  i, Wheel((25) & 255)); ///
    //     strip.setPixelColor(37 -  i, Wheel((50) & 255)); ////
    //     strip.setPixelColor(36 -  i, Wheel((25) & 255)); ///
    //     strip.setPixelColor(35 -  i, Wheel((10) & 255)); //

    delay(facotr);
    //Serial.println("warpin");
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
