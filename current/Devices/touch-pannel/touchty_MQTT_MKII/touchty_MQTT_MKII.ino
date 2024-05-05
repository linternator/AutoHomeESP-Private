#include <AutoHome.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 2

AutoHome autohome;

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 TouchControllerA = Adafruit_MPR121();
Adafruit_MPR121 TouchControllerB = Adafruit_MPR121();
Adafruit_MPR121 TouchControllerC = Adafruit_MPR121();
Adafruit_MPR121 TouchControllerD = Adafruit_MPR121();

#define intruptPin 35

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouchedA = 0;
uint16_t currtouchedA = 0;

uint16_t lasttouchedB = 0;
uint16_t currtouchedB = 0;

uint16_t lasttouchedC = 0;
uint16_t currtouchedC = 0;

uint16_t lasttouchedD = 0;
uint16_t currtouchedD = 0;

///////// LED stuff things

struct LEDState {
  int mode;          // LED mode, 0 = off, 1 = solid, 2 = flash, fade = 3
  float speed;       // flash or fade speed.
  float brightness;  // float, replaces anny.
  uint8_t R;         // red value (0-255)
  uint8_t G;         // green value (0-255)
  uint8_t B;         // blue value (0-255)
};

int Rplus = 0;
int Gplus = 0;
int Bplus = 0;
const int number_of_leds = 3;

LEDState leds[number_of_leds];
// leds [3] = { LEDState = led1, LEDState = led2, LEDState = led3 };


unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long updateInterval = 25;

void SetColour(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(led, r, g, b);
  strip.show();
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  if (!autohome.mqtt_callback(topic, payload, length)) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    Serial.print(packet);

    //    string e.g.  "led" : LED : MODE :  R  :  G  :  B  : BRIGHT  : SPEED
    // led number      "led" : int : int  : int : int : int : float(1): float

    if (autohome.getValue(packet, ':', 0).equals("led")) {
      leds[autohome.getValue(packet, ':', 1).toInt()].mode = autohome.getValue(packet, ':', 2).toInt();
      leds[autohome.getValue(packet, ':', 1).toInt()].R = autohome.getValue(packet, ':', 3).toInt();
      leds[autohome.getValue(packet, ':', 1).toInt()].G = autohome.getValue(packet, ':', 4).toInt();
      leds[autohome.getValue(packet, ':', 1).toInt()].B = autohome.getValue(packet, ':', 5).toInt();
      leds[autohome.getValue(packet, ':', 1).toInt()].brightness = autohome.getValue(packet, ':', 6).toFloat();
      leds[autohome.getValue(packet, ':', 1).toInt()].speed = autohome.getValue(packet, ':', 7).toFloat();


      // if (autohome.getValue(packet, ':', 0).equals("SOLID")) {  // SET an LED to a specific colour.
      //   // set LED to solid,        e.g. SOLID:LED:R:G:B:brightness
      //   //                                  0 : 1: 2:3:4

      //   switch ( autohome.getValue(packet, ':', 1).toInt() )
      //     {
      //         case 1:
      //           {
      //             led1.mode = 1;  // mode is solid
      //             led1.R = autohome.getValue(packet, ':', 2).toInt();
      //             led1.G = autohome.getValue(packet, ':', 3).toInt();
      //             led1.B = autohome.getValue(packet, ':', 4).toInt();
      //           } break;
      //     }
      // SetColour(autohome.getValue(packet, ':', 1).toInt(), autohome.getValue(packet, ':', 2).toInt(), autohome.getValue(packet, ':', 3).toInt(), autohome.getValue(packet, ':', 4).toInt());
    }
  }
}
// // Fill the dots one after the other with a color
// void colorWipe(uint32_t c, uint8_t wait) {
//   for (uint16_t i = 0; i < strip.numPixels(); i++) {
//     strip.setPixelColor(i, c);
//     strip.show();
//     delay(wait);
//   }
// }

void SendMQTTStatus(uint16_t currtouched, uint16_t lasttouched, String chipName) {
  // Get the currently touched pads

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" touched " + chipName);

      String packet = chipName + ":" + String(i) + ":touched";
      autohome.sendPacket(packet.c_str());
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" released " + chipName);

      String packet = chipName + ":" + String(i) + ":released";
      autohome.sendPacket(packet.c_str());
    }
  }
}



void setup() {
  Serial.begin(115200);

  while (!Serial) {  // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  strip.begin();
  strip.setBrightness(50);
  strip.show();  // Initialize all pixels to 'off'

  autohome.setPacketHandler(mqtt_callback);

  // colorWipe(strip.Color(0, 255, 0), 1);  // Green

  pinMode(intruptPin, INPUT_PULLUP);

  Serial.println("Adafruit MPR121 TouchControllerDacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!TouchControllerA.begin(0x5A)) {
    Serial.println("MPR121 A not found, check wiring?");
    //  while (1);
  } else {
    Serial.println("MPR121 A found!");
  }

  if (!TouchControllerB.begin(0x5B)) {
    Serial.println("MPR121 B not found, check wiring?");
    //   while (1);
  } else {
    Serial.println("MPR121 B found!");
  }

  if (!TouchControllerC.begin(0x5C)) {
    Serial.println("MPR121 C not found, check wiring?");
    //   while (1);
  } else {
    Serial.println("MPR121 C found!");
  }

  if (!TouchControllerD.begin(0x5D)) {
    Serial.println("MPR121 D not found, check wiring?");
    //   while (1);
  } else {
    Serial.println("MPR121 D found!");
  }

  autohome.begin();

  // colorWipe(strip.Color(255, 100, 0), 1);  // een
}

void loop() {

  autohome.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    for (int i = 0; i < number_of_leds; i++)  // incremnt LED.
    {

      switch (leds[i].mode) {
        case 0:  // off
          {
            SetColour(i, 0, 0, 0);
          }
          break;

        case 1:  // set colour
          {
            SetColour(i, leds[i].R, leds[i].G, leds[i].B);
          }
          break;

        case 2:  // flash
          {
            if (leds[i].brightness > 1)  // interat the animation
            {
              leds[i].brightness = 0;
            } else {
              leds[i].brightness += leds[i].speed;
            }

            SetColour(i,
                      (int)(leds[i].R * leds[i].brightness),
                      (int)(leds[i].G * leds[i].brightness),
                      (int)(leds[i].B * leds[i].brightness));
            // Serial.println((leds[i].R * leds[i].brightness));
          }
          break;

        case 3:  // fade
          {
            leds[i].brightness += leds[i].speed;

            if (leds[i].brightness > 2)  // interat the animation
            {
              leds[i].brightness = 0;
            }

            if (leds[i].brightness < 1) {  // ramping up
              SetColour(i,
                        (int)(leds[i].R * leds[i].brightness),
                        (int)(leds[i].G * leds[i].brightness),
                        (int)(leds[i].B * leds[i].brightness));
            //  Serial.println((leds[i].R * leds[i].brightness));
            } else  // ramping down
            {
              SetColour(i,
                        (int)(leds[i].R * (2 - leds[i].brightness)),
                        (int)(leds[i].G * (2 - leds[i].brightness)),
                        (int)(leds[i].B * (2 - leds[i].brightness)));
            //  Serial.println((leds[i].R * (2 - leds[i].brightness)));
            }
          }
          break;
      }
      // SetColour(i ,leds[i].R,leds[i].G,leds[i].B);
      strip.show();  // Initialize all pixels to 'off'
    }
  }


  // SetColour(led,r,g,b)

  // update LED animation state.


  if (digitalRead(intruptPin) == LOW) {  // handle touche buttons being pushed.

    currtouchedA = TouchControllerA.touched();
    SendMQTTStatus(currtouchedA, lasttouchedA, "A");
    lasttouchedA = currtouchedA;

    currtouchedB = TouchControllerB.touched();
    SendMQTTStatus(currtouchedB, lasttouchedB, "B");
    lasttouchedB = currtouchedB;

    currtouchedC = TouchControllerC.touched();
    SendMQTTStatus(currtouchedC, lasttouchedC, "C");
    lasttouchedC = currtouchedC;

    currtouchedD = TouchControllerD.touched();
    SendMQTTStatus(currtouchedD, lasttouchedD, "D");
    lasttouchedD = currtouchedD;

    // comment out this line for detailed data from the sensor!
    return;
  }
}
