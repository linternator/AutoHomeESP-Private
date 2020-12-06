// On ESP8266:
// At 80MHz runs up 57600ps, and at 160MHz CPU frequency up to 115200bps with only negligible errors.
// Connect pin 12 to 14.

#include <SoftwareSerial.h>

#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)
#define TX (1)

#define relay 16

#define BAUD_RATE 19200 // 57600

 uint8_t coursSpeed = 0;

SoftwareSerial VSerial;
SoftwareSerial ASerial;

void setup()
{
  Serial.begin(115200);
  // Important: the buffer size optimizations here, in particular the isrBufSize (11) that is only sufficiently
  // large to hold a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial
  // adapter gets read before another write is performed.
  // Block writes with a size greater than 1 would usually fail. Do not copy this into your own project without
  // reading the documentation.
  VSerial.begin(BAUD_RATE, SWSERIAL_8N1, 10, 9, false, 95, 11); // A // Voltage
  ASerial.begin(BAUD_RATE, SWSERIAL_8N1, 5, 4, false, 95, 11); // B // Current  // fix pins, wrong pins :C 

  Serial.println(PSTR("\nSoftware serial test started"));
}

void loop()
{
//  uint8_t coursSpeed = random(0, 255);
if(coursSpeed == 255)
  {
    coursSpeed = 0;
  }
  coursSpeed ++;
  ASerial.write(coursSpeed);
    Serial.println(coursSpeed);
  VSerial.write(coursSpeed);

  delay(500);
}
