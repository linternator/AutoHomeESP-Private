
#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define PSU_PIN 0

uint8_t m0de = 0; // mode of operation
uint8_t wait = 50; // delay
uint8_t i = 0;    // pixen number
uint8_t color = 13;
uint8_t brightness = 255;
uint8_t prev_brightness = 255;
uint8_t next_brightness = 255;
uint8_t fade = 0;


#define LED_COUNT 200

AutoHome autohome;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void WriteVerables()
{
  LittleFS.begin();
  
  File verables = LittleFS.open("/verables", "w");    // open file verbales on SPIFF file system to write to. 
   if(!verables)
   {
    Serial.println("error opening verbles file D; ");
    return; 
   }

  char verableE[7] = {m0de,wait,color,brightness,prev_brightness,next_brightness,fade};                // creat char array
  if(verables.print(verableE))                          // write char array to file system
  {
    Serial.println("wrote verables");
  }
  else
     {
      Serial.println("failed");
     }

  verables.close();  
  LittleFS.end();
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{

    if (autohome.mqtt_callback(topic, payload, length) == 0)
    {
        String packet = "";

        for (int i = 0; i < length; i++)
        {
            packet = packet + (char)payload[i];
        }

        if (getValue(packet, ':', 0).equals("brightness"))
        { // brightness
            brightness = getValue(packet, ':', 1).toInt();
            prev_brightness = getValue(packet, ':', 1).toInt();
            String packet = "room-lights wait:" + String(wait) + ", col:" + String(color) + ", mode:" + String(m0de) + ", brightness:" + String(brightness);
            autohome.sendPacket(packet.c_str());
        }

        if (getValue(packet, ':', 0).equals("spEEd"))
        { // spEEd
            wait = getValue(packet, ':', 1).toInt();
            String packet = "room-lights wait:" + String(wait) + ", col:" + String(color) + ", mode:" + String(m0de);
            autohome.sendPacket(packet.c_str());
        }

        if (getValue(packet, ':', 0).equals("colour"))
        { // colour
            color = getValue(packet, ':', 1).toInt();
            String packet = "room-lights wait:" + String(wait) + ", col:" + String(color) + ", mode:" + String(m0de);
            autohome.sendPacket(packet.c_str());
        }

        if (getValue(packet, ':', 0).equals("mode"))
        { // mode
            m0de = getValue(packet, ':', 1).toInt();
            String packet = "room-lights wait:" + String(wait) + ", col:" + String(color) + ", mode:" + String(m0de);
            autohome.sendPacket(packet.c_str());
        }

        if (getValue(packet, ':', 0).equals("stat"))
        { // stat

            String packet = "room-lights wait:" + String(wait) + ", col:" + String(color) + ", mode:" + String(m0de )+ ", brightness:" + String(brightness)
            + ", prev_brightness:" + String(prev_brightness)+ ", fade:" + String(fade)+ ", next_brightness:" + String(next_brightness);
            autohome.sendPacket(packet.c_str());
        }

        Serial.print(packet);
         WriteVerables();  // srite new verables to flash chip.
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(PSU_PIN,OUTPUT);
    digitalWrite(PSU_PIN, HIGH);
    strip.begin();                     // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.setBrightness(255);          // Set BRIGHTNESS to about 1/5 (max = 255)

    autohome.setPacketHandler(mqtt_callback);
    autohome.begin();

LittleFS.begin();

  File verables = LittleFS.open("/verables", "r");   // opens the file on flash as "r" read

   
  size_t FileSize = verables.size();                             // get file size in bytes

    if(FileSize >= 3)                                                // check if file size is full or not.
        {
        std::unique_ptr<char[]> buf(new char[FileSize]);               // creat char arrray buffer
        verables.readBytes(buf.get(), FileSize);                       // read file verables and put in buffer

        m0de = buf[0];
        wait= buf[1];
        color = buf[2];
        brightness = buf[3];
        prev_brightness = buf[4];
        next_brightness = buf[5];
        fade = buf[6];
        Serial.println("done reading file system"); 
        }
  verables.close();
LittleFS.end();
Serial.println("loggling"); 
    // load verables  from rom.
}

void off(int wait)
{
    fade = 1;
 }

void bright()
{
    
    if(fade == 1)
        {       
                if(brightness != 0)
                    {
                        prev_brightness = brightness;
                    }
                
                brightness = 0; // set brighness to 0
        }
            else
            {
                    brightness = prev_brightness;
            }
            

    if(next_brightness  != brightness)      // if desired brightness dose not match the current brighness next_brightness.
    {
        if(next_brightness < brightness)
            {
                next_brightness++;
            }

        if(next_brightness > brightness)
            {
                next_brightness--;
            }
    }
        else
        {
            next_brightness = brightness;
        }
        

    strip.setBrightness(next_brightness);
}

void Rando(int wait)
{
    strip.setPixelColor((random(0, strip.numPixels())), Wheel((random(0, 255)) & 255)); // set random pixel to random coulour
    strip.show();                                                                       //  Update strip to match
   // delay(wait);
}

void RandoRing(int wait) // dosent work :/ upadte pixels mroe offten ?
{

    if (i < strip.numPixels())
    {
        i = 0;
    }
    else
    {
        i++;
    }
    strip.setPixelColor(i, Wheel((random(0, 255)) & 255)); // set random pixel to random coulour
    strip.show();                                          //  Update strip to match
   // delay(wait);
}

void colorWipe(uint32_t color)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {                                         // For each pixel in strip...
        strip.setPixelColor(i, Wheel(color)); //  Set pixel's color (in RAM)
    }
    strip.show(); //  Update strip to match
}

void rainbow(int wait)
{
    for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
    {
        for (int i = 0; i < strip.numPixels(); i++)
        {
            int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
            strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        strip.show(); // Update strip with new contents
        delay(wait);  // Pause for a moment
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void loop()
{
    autohome.loop();
    delay(wait);
    
    switch (m0de)
    {
    case 0: //set all pixels to off
    {
        off(wait);
        strip.show();                // Initialize all pixels to 'off'
        if(next_brightness == 0)
          { digitalWrite(PSU_PIN, HIGH); // turn off PSU
          }
        
    }
    break;

    case 1: //random 
    {
        strip.begin();
        fade = 0;
        digitalWrite(PSU_PIN, LOW);
        Rando(0);
    }
    break;

    case 2: // rotating ring of randoly updated coulour.
    {
        strip.begin();
        fade = 0;
        digitalWrite(PSU_PIN, LOW);
        RandoRing(0);
    }
    break;

    case 3: // set color to the color
    {
        strip.begin();
        fade = 0;
        digitalWrite(PSU_PIN, LOW);
        colorWipe(Wheel((color)&255));
        // strip.setPixelColor(R , Wheel((cOl) & 255)); //
    }
    break;

    case 4: // white
    {
        strip.begin();
        fade = 0;
        colorWipe(strip.Color(255, 255, 255)); // white
        strip.show();                          // Initialize all pixels to 'off'
        digitalWrite(PSU_PIN, HIGH);           // turn off PSU
    }
    break;
    }

    bright();
     
}
