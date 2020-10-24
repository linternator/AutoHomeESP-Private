#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    6
#define LED_COUNT 63

int i = 0;

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


int Brightness = 50;

int ChveronsLocks[] = {3, 10, 17, 24, 31, 38, 45, 52, 59 }; // chevron middle bits
int ChveronsSides[] = {3,5, 10,12, 17,19, 24,26, 31,33, 38,40, 45,47, 52,54, 59,61 };  // chevron side bits // dont realy need
int Symbols[] = {0,1, 5,6,7,8, 12,13,14,15, 19,20,21,22, 26,27,28,29, 33,34,35,36, 40,41,42,43, 47,48,49,50, 54,55,56,57, 61,62 };  // symoblosl


void setup() {
  // put your setup code here, to run once:
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(Brightness); // Set BRIGHTNESS to about 1/5 (max = 255)

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(i == 36)
    {i = 0;}
      else
      {
       pixels.clear();

    
      // light pixel I 
       pixels.setPixelColor(Symbols[i], pixels.Color(255,255,255));
      // turn off pixel I - 1.
      pixels.setPixelColor( (Symbols[i-1]) , pixels.Color(0,0,0));
    
    

  if(i < 9)
      {
      // light pixel I 
       pixels.setPixelColor(ChveronsLocks[i], pixels.Color(255,255,255));
      // turn off pixel I - 1.
      pixels.setPixelColor( (ChveronsLocks[i-1]) , pixels.Color(0,0,0));
      }

   if(i < 28)
      {
      // light pixel I 
       pixels.setPixelColor((ChveronsLocks[(i/3)] -1 ), pixels.Color(255,255,255));
       pixels.setPixelColor((ChveronsLocks[(i/3)] +1 ), pixels.Color(255,255,255));
      // turn off pixel I - 1.
  //     pixels.setPixelColor((ChveronsLocks[(i/3)] -2 ), pixels.Color(0,0,0));
  //     pixels.setPixelColor((ChveronsLocks[(i/3)] -4 ), pixels.Color(0,0,0));
      }

    i++;
      delay(500);
      pixels.show();
      Serial.println(i);
      }
}
