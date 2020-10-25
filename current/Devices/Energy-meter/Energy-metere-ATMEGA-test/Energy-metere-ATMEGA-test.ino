// On ESP8266:
// At 80MHz runs up 57600ps, and at 160MHz CPU frequency up to 115200bps with only negligible errors.
// Connect pin 12 to 14.

#include <SoftwareSerial.h>

#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)
#define TX (1)

#define BAUD_RATE 19200  // 57600

 SoftwareSerial swSer;

void setup() {
  Serial.begin(115200);
  // Important: the buffer size optimizations here, in particular the isrBufSize (11) that is only sufficiently
  // large to hold a single 8N1 word, are on the basis that any char written to the loopback SoftwareSerial
  // adapter gets read before another write is performed.
  // Block writes with a size greater than 1 would usually fail. Do not copy this into your own project without
  // reading the documentation.
//   swSer.begin(BAUD_RATE, SWSERIAL_8N1, 4, 5, false, 95, 11);    // A // Voltage
  swSer.begin(BAUD_RATE, SWSERIAL_8N1, 5, 4, false, 95, 11);   // B // Current


  Serial.println(PSTR("\nSoftware serial test started"));

  for (char ch = ' '; ch <= 'z'; ch++) {
    swSer.write(ch);
  }
  swSer.println();
}

void loop() {
  while (swSer.available() > 0) {
    Serial.write(swSer.read());
    yield();
  }
  while (Serial.available() > 0) {
    int Byte = 0x00;
    Byte = Serial.read();
    swSer.write(Byte,sizeof(Byte));   // MUST SEND A 1 HEX BYTE ONLY.
    yield();
  }

}
