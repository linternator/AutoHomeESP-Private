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

struct TurnOnAnimation
{
  // How long the turn on animation should be in ms
  float AnimationTimeMs = 200;

  // How wide the highlight should be in number of Leds
  int HighlightWidth = 5;

  // The color of the highlight
  int HighlightWW = 255;
  int HighlightCW = 255;
  int HighlightAM = 255;
};

struct TurnOffAnimation
{
  // How long the turn off animation should be in ms
  float AnimationTimeMs = 200;
};

TurnOnAnimation turnOnAnimation;
TurnOffAnimation turnOffAnimation;

int default_WW = 128;
int default_CW = 128;
int default_AM = 128;

int WW = 128;
int CW = 128;
int AM = 128;
bool changeColourRequested = false;

// Keep track of state
LightState state = TurnedOff;
unsigned long stateStartTime;

Adafruit_NeoPixel pixels(NUMBER_OF_PIXELS, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
AutoHome autohome;

void set_state(LightState new_state)
{
  state = new_state;
  stateStartTime = millis();
}

void mqtt_send_stats()
{
  String packet = "Bathroom is, WW:" + String(WW) +
                  ", CW:" + String(CW) +
                  ", AM:" + String(AM) +
                  ", State:" + String(state) +
                  ", State Start time:" + String(stateStartTime);
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
    TurnOnAnimation_Loop();
  }
  else if (state == TurningOff)
  {
    TurnOffAnimation_Loop();
  }
}

void TurnOnAnimation_Loop()
{
  unsigned long currentTime = millis();
  unsigned long timeSinceStart = currentTime - stateStartTime;
  if (timeSinceStart < turnOnAnimation.AnimationTimeMs)
  {
    // Do the turn on animation
    float ratio = timeSinceStart / turnOnAnimation.AnimationTimeMs;
    int pixelsTurnedOn = NUMBER_OF_PIXELS * ratio;

    //Sets the first leds to the wanted AM, CW, WW color
    for (int i = 0; i < pixelsTurnedOn && i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(AM, CW, WW));
    }

    // Sets the highlight colors infront of the normal wanted colors
    for (int i = pixelsTurnedOn; i < pixelsTurnedOn + turnOnAnimation.HighlightWidth && i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(
                                  turnOnAnimation.HighlightAM,
                                  turnOnAnimation.HighlightCW,
                                  turnOnAnimation.HighlightWW));
    }

    // Send the updated pixel colors to the hardware.
    pixels.show();
  }
  else
  {
    // The turn on animation is done, just set all the leds to the same colour
    for (int i = 0; i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(AM, CW, WW));
    }
    // Send the updated pixel colors to the hardware.
    pixels.show();
    set_state(TurnedOn);
  }
}

void TurnOffAnimation_Loop()
{

  unsigned long currentTime = millis();
  unsigned long timeSinceStart = currentTime - stateStartTime;
  if (timeSinceStart < turnOffAnimation.AnimationTimeMs)
  {
    // Do the turn on animation
    float ratio = timeSinceStart / turnOnAnimation.AnimationTimeMs;
    int pixelsTurnedOff = NUMBER_OF_PIXELS * ratio / 2;
    int halfPoint = NUMBER_OF_PIXELS / 2;

    // Turn on from the middle and out

    // Upper half
    for (int i = halfPoint; i < halfPoint + pixelsTurnedOff && i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }

    // Lower half
    for (int i = halfPoint; i > halfPoint - pixelsTurnedOff && i >= 0; i--)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }

    // Send the updated pixel colors to the hardware.
    pixels.show();
  }
  else
  {

    for (int i = 0; i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show(); // Send the updated pixel colors to the hardware.
    set_state(TurnedOff);
  }
}
