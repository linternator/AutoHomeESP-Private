/* This example is a simple setup example to show you how to setup the AutoHome library. */
// 1M (512 SPIFFS)
#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 14

uint8_t spEEd = 25;
uint8_t cOl= 200;
uint8_t m0de = 1;

// define RTC memory blocks
// 4 bytes per block. 


AutoHome autohome;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(45, PIN, NEO_GRB + NEO_KHZ800);


void WriteVerables()
{
  File verables = SPIFFS.open("/verables", "w");    // open file verbales on SPIFF file system to write to. 
   if(!verables)
   {
    Serial.println("error opening verbles file :C ");
    return; 
   }
  char verableE[3] = {spEEd,cOl,m0de};                // creat char array
  if(verables.print(verableE))                          // write char array to file system
  {
    Serial.print("wrote verables");
  }
  else
     {
      Serial.println("failed");
     }

  verables.close();  
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {


  String packet = "";
  
  if (!autohome.mqtt_callback(topic, payload, length)) {

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    if (getValue(packet, ':', 0).equals("spEEd")) {   // spEEd
      spEEd = getValue(packet, ':', 1).toInt();
      EEPROM.put(42,spEEd);
      EEPROM.commit();

      int tesum;
      EEPROM.get(42,tesum);
      Serial.println(tesum);
    //  ESP.rtcUserMemoryWrite(34,  &spEEd, sizeof(spEEd));   // if value changes, write to RTC memorty. 
    }


    if (getValue(packet, ':', 0).equals("colour")) { // colour
      cOl= getValue(packet, ':', 1).toInt();
    }


            if (getValue(packet, ':', 0).equals("mode")) { // mode
      m0de = getValue(packet, ':', 1).toInt();
    }

            if (getValue(packet, ':', 0).equals("stat")) { // stat
              
          String packet = "warp-core spEEd:" + String(spEEd) + ", col:" + String(cOl) + ", mode:" + String(m0de);
          autohome.sendPacket( packet.c_str() );      
    }
    

          if (getValue(packet, ':', 0).equals("reset")) { // reset CPU
              ESP.restart();
    }
   WriteVerables();  // srite new verables to flash chip.
  }

}


void setup() {
  
  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  strip.begin();
 // colorWipe(strip.Color(0, 255, 0), 50); // Red
 // strip.show(); // Initialize all pixels to 'off'

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

  File verables = SPIFFS.open("/verables", "r");                 // opens the file on flash as "r" read
  size_t FileSize = verables.size();                             // get file size in bytes

if(FileSize >= 3)                                                // check if file size is full or not.
{
  std::unique_ptr<char[]> buf(new char[FileSize]);               // creat char arrray buffer
  verables.readBytes(buf.get(), FileSize);                       // read file verables and put in buffer

  spEEd = buf[0];
  cOl= buf[1];
  m0de = buf[2];
}
  verables.close();                                              // close file.

}

void loop() {

switch( m0de)
  {
    case 0:
      {
        //set all pixels to off
         strip.begin();
         colorWipe(strip.Color(0, 0, 0), 50); // Red
         strip.show(); // Initialize all pixels to 'off'
      } break;

   case 1:
      {
         Warp( spEEd ); // warp spEEd! 
      } break;

   case 2:
      {
        // on
         strip.begin();
          for(int R=0; R<50; R++)
          {
         strip.setPixelColor(R , Wheel((cOl) & 255)); //
         strip.show(); // Initialize all pixels to 'off'
          }
      } break;

    case 3:   // no dosent work 
      {
        // some othere paturn
      //   strip.begin();
         theaterChaseRainbow(spEEd);
     //   strip.show(); // Initialize all pixels to 'off'
      } break;

    case 4:
      {
        // some othere paturn
      //   strip.begin();
        rainbowCycle(spEEd);
     //   strip.show(); // Initialize all pixels to 'off'
      } break;



  }

  delay(0);

  autohome.loop();

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
      autohome.loop();
    delay(wait);
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
  autohome.loop();
      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void Warp(uint8_t facotr) {

  strip.clear();
  
  for (int i = -5; i <= 20; i++)
  {
    strip.setPixelColor(i,  0, 0, 0);                     // top part of the warp core
    strip.setPixelColor(i + 1, Wheel((cOl) & 255)); //

    strip.setPixelColor(43 -  i, Wheel((cOl) & 255)); //  // bottom part of the warp core
    strip.setPixelColor(44 -  i,  0, 0, 0);

    for (int L = 20; L <= 24; L++)                    // intermix chmaber glows birhgter.
    {
              strip.setPixelColor(L, 255,255,255);  // add pulsing in the middle

//              switch(i)
//                {
//                  case :>0<5   // pulsing up
//                    {
//                        strip.setPixelColor(L, 255,255,(50*i));  // add pulsing in the middle
//                    }break;
//                  
//                  case :>5<10   // pulsing down
//                    {
//                      strip.setPixelColor(L, 255,255,(250/i));  // add pulsing in the middle
//                    }break;
//
//                  case :>10<19  //  constant
//                    {
//                      strip.setPixelColor(L, 255,255,140);  // add pulsing in the middle
//
//                    }break;
//
//                  case :20  // flash up.
//                    {
//                      strip.setPixelColor(L, 255,100,200);  // add pulsing in the middle
//
//                    }break;
//                }
    }

    delay(facotr);
    strip.show();

  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
