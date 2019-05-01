#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 13

Adafruit_NeoPixel strip = Adafruit_NeoPixel(45, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
//  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
////colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
//  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue


// set intermix chamber to white.   
for (int i = 20; i <= 25; i++)
{
  strip.setPixelColor(i, 255,255,255);
}
Serial.println("warp ready");
Warp(50);

//  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);
}

        // speed // 
void Warp(uint8_t facotr) {
  for (int i = -5; i <= 15; i++)
    {
       strip.setPixelColor(i,  Wheel((0) & 255));
       strip.setPixelColor(i+1, Wheel((10) & 255)); //
       strip.setPixelColor(i+2, Wheel((25) & 255)); ///
       strip.setPixelColor(i+3, Wheel((50) & 255)); ////
       strip.setPixelColor(i+4, Wheel((25) & 255)); ///
       strip.setPixelColor(i+5, Wheel((10) & 255)); //
        
               for (int L = 20; L <= 25; L++)   // intermix chmaber glows birhgter. 
        {
          strip.setPixelColor(L, i+200,i+200,i+200);
        }

       strip.setPixelColor(i-30,  Wheel((0) & 255));
       strip.setPixelColor(i-29, Wheel((10) & 255)); //
       strip.setPixelColor(i-28, Wheel((25) & 255)); ///
       strip.setPixelColor(i-27, Wheel((50) & 255)); ////
       strip.setPixelColor(i-26, Wheel((25) & 255)); ///
       strip.setPixelColor(i-25, Wheel((10) & 255)); //

        delay(facotr);
Serial.println("warpin");
       strip.show();
       
    }
}






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
