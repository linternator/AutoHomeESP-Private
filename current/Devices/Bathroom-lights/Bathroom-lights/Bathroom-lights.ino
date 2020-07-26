/* This example is a simple setup example to show you how to setup the AutoHome library. */

#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        0 // On Trinket or Gemma, suggest changing this to 1
#define OnOFF       2 // PSU on off pin

int WW  = 0;
int CW  = 0;
int AM  = 0;

bool onOFF = 0;

#define NUMPIXELS 120 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

AutoHome autohome;


void mqtt_callback(char *topic, byte *payload, unsigned int length)
{

  String packet = "";

  for (int i = 0; i < length; i++)
  {
    packet = packet + (char)payload[i];
  }

  if (autohome.getValue(packet, ':', 0).equals("SET"))
  {
    WW = autohome.getValue(packet, ',' , 1).toInt();
    CW = autohome.getValue(packet, ',' , 2).toInt();
    AM = autohome.getValue(packet, ',' , 3).toInt();

    String packet = "bathroom is, WW:" + String(WW) + ", CW:" + String(CW) + ", AM:" + String(AM);
    autohome.sendPacket(packet.c_str());
  }

  if (autohome.getValue(packet, ':', 0).equals("WW"))
  { // colour
    WW = autohome.getValue(packet, ':', 1).toInt();
    String packet = "bathroom is, WW:" + String(WW) + ", CW:" + String(CW) + ", AM:" + String(AM);
    autohome.sendPacket(packet.c_str());
  }

  if (autohome.getValue(packet, ':', 0).equals("CW"))
  { // colour
    CW = autohome.getValue(packet, ':', 1).toInt();
    String packet = "bathroom is, WW:" + String(WW) + ", CW:" + String(CW) + ", AM:" + String(AM);
    autohome.sendPacket(packet.c_str());
  }

  if (autohome.getValue(packet, ':', 0).equals("AM"))
  { // colour
    AM = autohome.getValue(packet, ':', 1).toInt();
    String packet = "bathroom is, WW:" + String(WW) + ", CW:" + String(CW) + ", AM:" + String(AM);
    autohome.sendPacket(packet.c_str());
  }



  if (autohome.getValue(packet, ':', 0).equals("stat"))
  { // stat

    String packet = "bathroom is, WW:" + String(WW) + ", CW:" + String(CW) + ", AM:" + String(AM);
    autohome.sendPacket(packet.c_str());
  }
}


void setup() {
  // put your setup code here, to run once:

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pinMode(OnOFF, OUTPUT);
  // digitalWrite(OnOFF, LOW);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(115200);
  Serial.println("hello worms");

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
}


void loop() {

  autohome.loop();
  
  for (int i = 0; i < NUMPIXELS; i++)
  {  
    pixels.setPixelColor(i, pixels.Color(AM, CW, WW));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }

  if ( WW == 0 && CW == 0 && AM == 0)
  {
    onOFF = true;
    digitalWrite(OnOFF, HIGH);
  }
  else
  {
    onOFF = true;
    digitalWrite(OnOFF, LOW);
  }

}
