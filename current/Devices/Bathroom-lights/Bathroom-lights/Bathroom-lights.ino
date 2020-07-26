/* This example is a simple setup example to show you how to setup the AutoHome library. */

#include <AutoHome.h>
#include <Adafruit_NeoPixel.h>

// Pin to turn the lights on/off
#define ON_OFF_BUTTON_PIN 12

// Wether the on/off button should be used.
// If set to false, you can send MQTT packages: "STATE:0" (Turn off) and "STATE:1" (Turn on)
#define USE_ON_OFF_BUTTON true

// On Trinket or Gemma, suggest changing this to 1
#define NEO_PIXEL_PIN 13

// Popular NeoPixel ring size
#define NUMBER_OF_PIXELS 122

enum LightState
{
  TurnedOff = 1,
  TurningOff = 2,
  TurnedOn = 4,
  TurningOn = 8,
  ColourChangeRequest = 16
};

int default_WW = 128;
int default_CW = 128;
int default_AM = 128;

int WW = 128;
int CW = 128;
int AM = 128;
bool changeColourRequested = false;

// Keep track of state
LightState state = TurnedOff;
unsigned long state_time;
//int current_neopixel = 0;
//int updateSpeed = 1;
//int lightsPerUpdate = 1;

Adafruit_NeoPixel pixels(NUMBER_OF_PIXELS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

AutoHome autohome;


void set_state(LightState new_state)
{
  state = new_state;
  state_time = millis();
}


void mqtt_send_stats()
{
  String packet = "Bathroom is, WW:" + String(WW) +
                  ", CW:" + String(CW) +
                  ", AM:" + String(AM) +
                  ", State:" + String(state) +
                  ", State time:" + String(state_time);
  autohome.sendPacket(packet.c_str());
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  String packet = "";
  for (int i = 0; i < length; i++)
  {
    packet = packet + (char)payload[i];
  }

  if (autohome.getValue(packet, ':', 0).equals("SET"))
  {
    WW = autohome.getValue(packet, ',', 1).toInt();
    CW = autohome.getValue(packet, ',', 2).toInt();
    AM = autohome.getValue(packet, ',', 3).toInt();

    mqtt_send_stats();
    changeColourRequested = true;
  }

 if (autohome.getValue(packet, ':', 0).equals("SET_DEFAULT"))
  {
    default_WW = autohome.getValue(packet, ',', 1).toInt();
    default_CW = autohome.getValue(packet, ',', 2).toInt();
    default_AM = autohome.getValue(packet, ',', 3).toInt();

    mqtt_send_stats();
    changeColourRequested = true;
  }


  if (autohome.getValue(packet, ':', 0).equals("WW"))
  {
    // colour
    WW = autohome.getValue(packet, ':', 1).toInt();
    mqtt_send_stats();
    changeColourRequested = true;
  }

  if (autohome.getValue(packet, ':', 0).equals("CW"))
  {
    // colour
    CW = autohome.getValue(packet, ':', 1).toInt();
    mqtt_send_stats();
    changeColourRequested = true;
  }

  if (autohome.getValue(packet, ':', 0).equals("AM"))
  {
    // colour
    AM = autohome.getValue(packet, ':', 1).toInt();
    mqtt_send_stats();
    changeColourRequested = true;
  }

  if (autohome.getValue(packet, ':', 0).equals("STATE") && !USE_ON_OFF_BUTTON)
  {
    // colour
    int new_state = autohome.getValue(packet, ':', 1).toInt();
    if (new_state == 0)
    {
      set_state(TurningOff);
    }
    else if (new_state == 1)
    {
      set_state(TurningOn);
    }
    mqtt_send_stats();
    changeColourRequested = true;
  }

  if (autohome.getValue(packet, ':', 0).equals("stat"))
  {
    // stats
    mqtt_send_stats();
  }
}

void setup()
{

  Serial.begin(115200);

  // On/Off button
  pinMode(ON_OFF_BUTTON_PIN, INPUT);

  // Initialize NeoPixel strip object
  pixels.begin();

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
  set_state(TurningOff);
}



void loop()
{
  autohome.loop();
   
  // Update state
  if (changeColourRequested)
  {
    // Can only update the color if we are turned on
    if (state == TurningOn ||
        state == TurnedOn ||
        state == ColourChangeRequest)
    {
      set_state(ColourChangeRequest);
    }
    changeColourRequested = false;
  }

  if (USE_ON_OFF_BUTTON)
  {
    if (digitalRead(ON_OFF_BUTTON_PIN) == HIGH && (state == TurnedOff || state == TurningOff))
    {
      // The switch have been turned on
      WW = default_WW;
      AM = default_AM;
      CW = default_CW;
      set_state(TurningOn);
      Serial.println("Turn on");
    }
    else if (
        digitalRead(ON_OFF_BUTTON_PIN) == LOW && state != TurnedOff && state != TurningOff)
    {
      // The switch have been turned off
      set_state(TurningOff);
      Serial.println("Turn off");
    }
  }

  if (state == TurningOn ||
      state == ColourChangeRequest)
  {
    // Set the color
    for (int i = 0; i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(AM, CW, WW));
    }
    pixels.show(); // Send the updated pixel colors to the hardware.

    set_state(TurnedOn);
  }
  else if (state == TurningOff)
  {
    // Turn off
    for (int i = 0; i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show(); // Send the updated pixel colors to the hardware.
    set_state(TurnedOff);
  }
}
