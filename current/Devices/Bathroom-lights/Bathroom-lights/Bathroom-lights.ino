/* This example is a simple setup example to show you how to setup the AutoHome library. */

// Bathroom lights are running on a ESP 8622
#define MQTT_VERSION MQTT_VERSION_3_1

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
#define NUMBER_OF_PIXELS 149

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

enum TurnOffAnimationType
{
  Across = 1,
  MiddleAndOut = 2,
};

struct TurnOffAnimation
{
  // How long the turn off animation should be in ms
  float AnimationTimeMs = 200;

  // Which animation type it should play
  TurnOffAnimationType AnimationType = Across;
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
  if (TurningOff == new_state)
  {
    turnOffAnimation.AnimationType = (TurnOffAnimationType)random(1, 3);
  }
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
  pinMode(ON_OFF_BUTTON_PIN, INPUT_PULLUP);

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
    if (digitalRead(ON_OFF_BUTTON_PIN) == LOW && (state == TurnedOff || state == TurningOff))
    {
      // The switch have been turned on
      WW = default_WW;
      AM = default_AM;
      CW = default_CW;
      set_state(TurningOn);
      Serial.println("Turn on");
    }
    else if (
        digitalRead(ON_OFF_BUTTON_PIN) == HIGH && state != TurnedOff && state != TurningOff)
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
    if (turnOffAnimation.AnimationType == Across)
    {
      TurnOffAnimation1_Loop();
    }
    else
    {
      TurnOffAnimation2_Loop();
    }
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
    int pixelsTurnedOn = NUMBER_OF_PIXELS * easeOutCubic(ratio);

    //Sets the first leds to the wanted AM, CW, WW color
    for (int i = 0; i < pixelsTurnedOn && i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(AM, CW, WW));
    }

    for (int i = pixelsTurnedOn; i < NUMBER_OF_PIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
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

void TurnOffAnimation1_Loop()
{

  unsigned long currentTime = millis();
  unsigned long timeSinceStart = currentTime - stateStartTime;
  if (timeSinceStart < turnOffAnimation.AnimationTimeMs)
  {
    // Do the turn on animation
    float ratio = timeSinceStart / turnOffAnimation.AnimationTimeMs;
    int pixelsTurnedOff = NUMBER_OF_PIXELS * ratio;

    // Turn off
    for (int i = 0; i < pixelsTurnedOff && i < NUMBER_OF_PIXELS; i++)
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

void TurnOffAnimation2_Loop()
{
  unsigned long currentTime = millis();
  unsigned long timeSinceStart = currentTime - stateStartTime;
  if (timeSinceStart < turnOffAnimation.AnimationTimeMs)
  {
    // Do the turn off animation
    float ratio = timeSinceStart / turnOffAnimation.AnimationTimeMs;
    int pixelsTurnedOff = NUMBER_OF_PIXELS * ratio / 2;
    int halfPoint = NUMBER_OF_PIXELS / 2;

    // Turn off from the middle and out

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


// https://github.com/nicolausYes/easing-functions/blob/master/src/easing.cpp
float easeInSine(float t)
{
  return sin(1.5707963 * t);
}

float easeOutSine(float t)
{
  return 1 + sin(1.5707963 * (--t));
}

float easeInOutSine(float t)
{
  return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
}

float easeInQuad(float t)
{
  return t * t;
}

float easeOutQuad(float t)
{
  return t * (2 - t);
}

float easeInOutQuad(float t)
{
  return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

float easeInCubic(float t)
{
  return t * t * t;
}

float easeOutCubic(float t)
{
  return 1 + (--t) * t * t;
}

float easeInOutCubic(float t)
{
  return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
}

float easeInQuart(float t)
{
  t *= t;
  return t * t;
}

float easeOutQuart(float t)
{
  t = (--t) * t;
  return 1 - t * t;
}

float easeInOutQuart(float t)
{
  if (t < 0.5)
  {
    t *= t;
    return 8 * t * t;
  }
  else
  {
    t = (--t) * t;
    return 1 - 8 * t * t;
  }
}

float easeInQuint(float t)
{
  float t2 = t * t;
  return t * t2 * t2;
}

float easeOutQuint(float t)
{
  float t2 = (--t) * t;
  return 1 + t * t2 * t2;
}

float easeInOutQuint(float t)
{
  float t2;
  if (t < 0.5)
  {
    t2 = t * t;
    return 16 * t * t2 * t2;
  }
  else
  {
    t2 = (--t) * t;
    return 1 + 16 * t * t2 * t2;
  }
}

float easeInExpo(float t)
{
  return (pow(2, 8 * t) - 1) / 255;
}

float easeOutExpo(float t)
{
  return 1 - pow(2, -8 * t);
}

float easeInOutExpo(float t)
{
  if (t < 0.5)
  {
    return (pow(2, 16 * t) - 1) / 510;
  }
  else
  {
    return 1 - 0.5 * pow(2, -16 * (t - 0.5));
  }
}

float easeInCirc(float t)
{
  return 1 - sqrt(1 - t);
}

float easeOutCirc(float t)
{
  return sqrt(t);
}

float easeInOutCirc(float t)
{
  if (t < 0.5)
  {
    return (1 - sqrt(1 - 2 * t)) * 0.5;
  }
  else
  {
    return (1 + sqrt(2 * t - 1)) * 0.5;
  }
}

float easeInBack(float t)
{
  return t * t * (2.70158 * t - 1.70158);
}

float easeOutBack(float t)
{
  return 1 + (--t) * t * (2.70158 * t + 1.70158);
}

float easeInOutBack(float t)
{
  if (t < 0.5)
  {
    return t * t * (7 * t - 2.5) * 2;
  }
  else
  {
    return 1 + (--t) * t * 2 * (7 * t + 2.5);
  }
}

float easeInElastic(float t)
{
  float t2 = t * t;
  return t2 * t2 * sin(t * PI * 4.5);
}

float easeOutElastic(float t)
{
  float t2 = (t - 1) * (t - 1);
  return 1 - t2 * t2 * cos(t * PI * 4.5);
}

float easeInOutElastic(float t)
{
  float t2;
  if (t < 0.45)
  {
    t2 = t * t;
    return 8 * t2 * t2 * sin(t * PI * 9);
  }
  else if (t < 0.55)
  {
    return 0.5 + 0.75 * sin(t * PI * 4);
  }
  else
  {
    t2 = (t - 1) * (t - 1);
    return 1 - 8 * t2 * t2 * sin(t * PI * 9);
  }
}

float easeInBounce(float t)
{
  return pow(2, 6 * (t - 1)) * abs(sin(t * PI * 3.5));
}

float easeOutBounce(float t)
{
  return 1 - pow(2, -6 * t) * abs(cos(t * PI * 3.5));
}

float easeInOutBounce(float t)
{
  if (t < 0.5)
  {
    return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * PI * 7));
  }
  else
  {
    return 1 - 8 * pow(2, -8 * t) * abs(sin(t * PI * 7));
  }
}