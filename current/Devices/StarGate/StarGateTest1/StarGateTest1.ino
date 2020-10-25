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


void DialAdress(int A, int B, int C, int D, int E, int F, int G,    int H,    int I)    // consider passing in an array.
{
  char system = M;  // M = milky way, 7. G = galaxy = 8. D = Destiny = 9.
  
//  if(I != 0)
//  {
//    // all 9 chevrons used.   // destiny
//    system = D;
//  }
//    else
//      {
//        if( H != 0)         // sort out if statments. 
//          {
//            system = G;
//          }
//        else 
//          {
//            system = M;
//          }
//      }

int i = 1;
int Dir = 1;
int Target =1;

array Adress[A,B,C,D,E,F,G,H,I];


if(Adress[i] != 0)    // if theres no more symbols entered.
{

// turn on shevron sides A
pixels.setPixelColor((ChveronsLocks[( Adress[i] )] -1 ), pixels.Color(255,255,255));
pixels.setPixelColor((ChveronsLocks[( Adress[i] )] +1 ), pixels.Color(255,255,255));

if( Dir == 1) // swich direction after each syboml is found and locked in. 
  {
    for(int X; X == A; X++)    // find symbol // convert to non blocking.   // clockwise.
      {
          pixels.setPixelColor(Symbols[X-1], pixels.Color(0,0,0));  // turn of last symbol
          pixels.setPixelColor(Symbols[X], pixels.Color(0,0,255));  // turn on symbol
      }
    Dir = 0
  }
    else
      {
        for(int X = 36; X == A; X--)    // find symbol // convert to non blocking.  // anti cockwise. 
          {
              pixels.setPixelColor(Symbols[X-1], pixels.Color(0,0,0));  // turn of last symbol
              pixels.setPixelColor(Symbols[X], pixels.Color(0,0,255));  // turn on symbol
          }
    Dir = 1;
      }

pixels.setPixelColor(ChveronsLocks[ Adress[i] ], pixels.Color(255,255,255));  // activeate shevron

pixels.setPixelColor((ChveronsLocks[( Adress[i] )] -1 ), pixels.Color(0,0,0));    // turn of side shevrons. 
pixels.setPixelColor((ChveronsLocks[( Adress[i] )] +1 ), pixels.Color(0,0,0));

i++;    // next adress
}




// loop
  
 // light sides 1
 // dial clockwise till A reached. 
 // lock chevoron 1
 // un light sides 1

 // light sides 2
 // dial anti clockwise till B reached.
 // lock Chevron 2
 // un light sides 2

 // loop

  
}
