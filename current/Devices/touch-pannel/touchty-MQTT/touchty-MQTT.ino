#include <AutoHome.h>

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

#define LED1 18
#define LED2 19
#define LED3 4

unsigned long myTime;

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


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  if (!autohome.mqtt_callback(topic, payload, length)) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    Serial.print(packet);

    // to set leds, send MQTT
    // LED1:1 // turn on LED 1.
    // LED2:0 // turn off LED 2.

    if (autohome.getValue(packet, ':', 1).equals("LED1")) {
      // do the thing
      int state1 = autohome.getValue(packet, ':', 2).toInt();
      digitalWrite(LED1, state1);
      Serial.println(state1);
    }

    if (autohome.getValue(packet, ':', 1).equals("LED2")) {
      // do the thing
      int state2 = autohome.getValue(packet, ':', 2).toInt();
      digitalWrite(LED2, state2);
      Serial.println(state2);
    }

    if (autohome.getValue(packet, ':', 1).equals("LED3")) {
      // do the thing
      int state3 = autohome.getValue(packet, ':', 2).toInt();
      digitalWrite(LED3, state3);
      Serial.println(state3);
    }

  }

}


void SendMQTTStatus(uint16_t currtouched, uint16_t lasttouched, String chipName)
{
  // Get the currently touched pads

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched " + chipName);

      String packet = chipName + ":" + String(i) + ":touched";
      autohome.sendPacket( packet.c_str() );
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released " + chipName);

      String packet = chipName + ":" + String(i) + ":released";
      autohome.sendPacket( packet.c_str() );
    }
  }
}



void setup() {
  Serial.begin(115200);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
  }

  autohome.setPacketHandler(mqtt_callback);



  pinMode(intruptPin, INPUT_PULLUP);

  Serial.println("Adafruit MPR121 TouchControllerDacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!TouchControllerA.begin(0x5A)) {
    Serial.println("MPR121 A not found, check wiring?");
    //  while (1);
  }
  else
  {
    Serial.println("MPR121 A found!");
  }

  if (!TouchControllerB.begin(0x5B)) {
    Serial.println("MPR121 B not found, check wiring?");
    //   while (1);
  }
  else
  {
    Serial.println("MPR121 B found!");
  }

  if (!TouchControllerC.begin(0x5C)) {
    Serial.println("MPR121 C not found, check wiring?");
    //   while (1);
  }
  else
  {
    Serial.println("MPR121 C found!");
  }

  if (!TouchControllerD.begin(0x5D)) {
    Serial.println("MPR121 D not found, check wiring?");
    //   while (1);
  }
  else
  {
    Serial.println("MPR121 D found!");
  }

  autohome.begin();

}

void loop() {

  autohome.loop();

  if (digitalRead(intruptPin) == LOW)
  {

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
