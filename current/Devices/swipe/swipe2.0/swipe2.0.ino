/****************************************************************
GestureTest.ino
APDS-9960 RGB and Gesture Sensor
Shawn Hymel @ SparkFun Electronics
May 30, 2014
https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor

Tests the gesture sensing abilities of the APDS-9960. Configures
APDS-9960 over I2C and waits for gesture events. Calculates the
direction of the swipe (up, down, left, right) and displays it
on a serial console. 

To perform a NEAR gesture, hold your hand
far above the sensor and move it close to the sensor (within 2
inches). Hold your hand there for at least 1 second and move it
away.

To perform a FAR gesture, hold your hand within 2 inches of the
sensor for at least 1 second and then move it above (out of
range) of the sensor.

Hardware Connections:

IMPORTANT: The APDS-9960 can only accept 3.3V!
 
 Arduino Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
 2            INT              Interrupt

Resources:
Include Wire.h and SparkFun_APDS-9960.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with SparkFun Arduino Pro Mini 3.3V

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
****************************************************************/

// MQTT DETAILS
// /sens
//  swipe



#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <AutoHome.h>
 
 AutoHome autohome;
// Pins
#define APDS9960_INT    14 // Needs to be an interrupt pin
#define LED 2

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void mqtt_callback(char* topic, byte* payload, unsigned int length){

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  Serial.print(packet);
  
}

void setup() {

  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  /* This starts the library and connects the esp to the wifi and the mqtt broker */

  autohome.setPacketHandler(mqtt_callback);
  autohome.begin();
  
  // Initialize interrupt service routine
  attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
    
}


void loop() {

 // delay(10);
   autohome.loop();
 //  delay(10);

  if(  isr_flag == 1  )
//  if( APDS9960_INT == 1 )
   {
  digitalWrite(LED,LOW);
//    isr_flag = 1; 
    detachInterrupt(APDS9960_INT);
    handleGesture();
    isr_flag = 0;
    delay(10);
    attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);
    Serial.println("ding");
  digitalWrite(LED, HIGH);
  }
}

ICACHE_RAM_ATTR void interruptRoutine() {
  isr_flag = 1;
  Serial.println("blep");
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("1,UP");
        autohome.sendPacket( "1,UP" );
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        autohome.sendPacket( "1,DOWN" );
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        autohome.sendPacket( "1,LEFT" );
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        autohome.sendPacket( "1,RIGHT" );
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        autohome.sendPacket( "1,NEAR" );
        break;
      case DIR_FAR:
        Serial.println("FAR");
        autohome.sendPacket( "1,FAR" );
        break;
      default:
        Serial.println("NONE");
        autohome.sendPacket( "1,NONE" );
    }
  }
}
