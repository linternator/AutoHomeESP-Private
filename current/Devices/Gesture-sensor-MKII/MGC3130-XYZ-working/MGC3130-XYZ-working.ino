#include <Wire.h>
#include <skywriter.h>

#define PIN_TRFD 18
#define PIN_REST 5

void setup() {
  Serial.begin(115200);
  while(!Serial){};
  Serial.println("Hello World!");

  Skywriter.begin(PIN_TRFD, PIN_REST);
  Skywriter.onXYZ(handle_xyz);
}

void loop() {
  Skywriter.poll();
}

void handle_xyz(unsigned int x, unsigned int y, unsigned int z){
 char buf[256];
 sprintf(buf, "%05u:%05u:%05u", x, y, z);
 Serial.println(buf);
}