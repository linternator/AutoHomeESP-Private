#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 2
#define PSU_PIN 0

uint8_t m0de = 0; // mode of operation
uint8_t wait = 1; // delay
uint8_t i = 0;    // pixen number
uint8_t color = 13;

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 200

AutoHome autohome;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // NEO_KHZ400

//    NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)

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

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{

  if (autohome.mqtt_callback(topic, payload, length) == 0)
  {
    String packet = "";

    for (int i = 0; i < length; i++)
    {
      packet = packet + (char)payload[i];
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

      String packet = "room-lights wait:" + String(wait) + ", col:" + String(color) + ", mode:" + String(m0de);
      autohome.sendPacket(packet.c_str());
    }

    Serial.print(packet);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  digitalWrite(PSU_PIN, HIGH);
  strip.begin();                     // INITIALIZE NeoPixel strip object (REQUIRED)
                                     //  strip.show();                      // Turn OFF all pixels ASAP
  strip.setBrightness(255);          // Set BRIGHTNESS to about 1/5 (max = 255)
  colorWipe(strip.Color(0, 255, 0)); // Red
  strip.show();                      // Initialize all pixels to 'off'

  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
}

void off(int wait)
{
  strip.begin();
  //  for(int i=0; i<strip.numPixels(); i++)
  {                                // For each pixel in strip...
      colorWipe(strip.Color(0, 0, 0)); // Red
    delay(wait); //  Pause for a moment
  }
}

void Rando(int wait)
{
  strip.setPixelColor((random(0, strip.numPixels())), Wheel((random(0, 255)) & 255)); // set random pixel to random coulour
  strip.show();                                                                       //  Update strip to match
  delay(wait);
}

void RandoRing(int wait) // dosent work :/ upadte pixels mroe offten ?
{

  if (i <= strip.numPixels())
  {
    i = 0;
  }
  else
  {
    i++;
  }
  strip.setPixelColor(i, Wheel((random(0, 255)) & 255)); // set random pixel to random coulour
  strip.show();                                          //  Update strip to match
  delay(wait);
}

void colorWipe(uint32_t color)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {                                       // For each pixel in strip...
    strip.setPixelColor(i, Wheel(color)); //  Set pixel's color (in RAM)
  }
  strip.show();                         //  Update strip to match
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
  // put your main code here, to run repeatedly:
  autohome.loop();
  switch (m0de)
  {
  case 0:
  {
    //set all pixels to off
    strip.begin();
    //colorWipe(strip.Color(0, 0, 0)); // off
    strip.clear();
    strip.show();                    // Initialize all pixels to 'off'
    digitalWrite(PSU_PIN, HIGH);     // turn off PSU
  }
  break;

  case 1:
  {
    strip.begin();
    digitalWrite(PSU_PIN, LOW);
    Rando(wait);
  }
  break;

  case 2:
  {
    strip.begin();
    digitalWrite(PSU_PIN, LOW);
    RandoRing(wait);
  }
  break;

  case 3: // set color to the color
  {
    strip.begin();
    digitalWrite(PSU_PIN, LOW);
    colorWipe(Wheel((color)&255));
    // strip.setPixelColor(R , Wheel((cOl) & 255)); //
  }
  break;
  

  case 4:
  {
    //set all pixels to off
    strip.begin();
    colorWipe( strip.Color(255, 255, 255) ); // off
    strip.show();                    // Initialize all pixels to 'off'
    digitalWrite(PSU_PIN, HIGH);     // turn off PSU
  }
  break;
}}
