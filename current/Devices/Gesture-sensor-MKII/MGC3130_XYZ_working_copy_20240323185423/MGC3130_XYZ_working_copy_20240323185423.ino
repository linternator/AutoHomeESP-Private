#include <Wire.h>
#include <skywriter.h>

#define PIN_TRFD 18
#define PIN_REST 5

void poll(){
  /* It doesn't matter how heavy this interrupt
  handler is. It will *not* fire again until
  Skywriter has finished processing the new data
  and we have re-attached the interrupt. */
  
  /* Handle the new data */
Skywriter.poll();
 // Serial.println("warrrr!");
  /* Skywriter must twiddle PIN_TRFR to tell
  the hardware it's processing the new data.
  Interrupt needs re-attaching afterwards! */
 // attachInterrupt(18,poll,FALLING); // Arduino Due
      // PIN_TRFD
  //attachInterrupt(0,poll,FALLING); // Arduino Uno
  //attachInterrupt(1,poll,FALLING); // Arduino Leonardo
}


void setup() {
  Serial.begin(115200);
  while(!Serial){};  
  
  Skywriter.begin(PIN_TRFD, PIN_REST);
  Skywriter.onTouch(touch);
  
  Serial.println("Hello world!");

  delay(10);
  Serial.println(digitalRead(18));
  
  /* Set up the initial interrupt */
 // attachInterrupt(18,poll,FALLING); // Arduino Due
  //attachInterrupt(0,poll,FALLING); // Arduino Uno
  //attachInterrupt(1,poll,FALLING); // Arduino Leonardo
  
  
}



void loop() {
//
// poll();

if(digitalRead(18) == 0)
  {
    poll();
  }

delay(10);
//  Serial.println(digitalRead(18));
}

void touch(unsigned char type){
  Serial.println("Got touch ");
  Serial.print(type,DEC);
  Serial.print('\n');
}

