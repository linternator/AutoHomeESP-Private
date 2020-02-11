/* This example is a simple setup example to show you how to setup the AutoHome library. */

#include <AutoHome.h>

AutoHome autohome;

#define RED_PIN 13
#define GREEN_PIN 12
#define BLUE_PIN 14
#define WHITE_PIN 2
#define ENABLE 15

uint32_t currR = 0;
uint32_t currG = 0;
uint32_t currB = 0;
uint32_t currW = 0;

int prevR = 0;
int prevG = 0;
int prevB = 0;
int prevW = 0;

unsigned long previousMillisKeypad = 0;
unsigned long previousMillisFade = 0;
int fadeTime = 0;

boolean fade = false;

void SETRGB(int r, int g, int b, int w);
void process_fade(long currentMillis);
// void process_fade(long currentMillis);


/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  if (autohome.mqtt_callback(topic, payload, length) == 0) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }


    if (autohome.getValue(packet, ':', 0).equals("RGB")) {

      if (autohome.getValue(packet, ':', 1).equals("STATIC")) {

        String colors = autohome.getValue(packet, ':', 2);
        SETRGB(autohome.getValue(colors, ',' , 0).toInt(), autohome.getValue(colors, ',' , 1).toInt(), autohome.getValue(colors, ',' , 2).toInt(), autohome.getValue(colors, ',' , 3).toInt());
//
        currR = prevR;
        currG = prevG;
        currB = prevB;
        currW = prevW;

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB) + "," + String(currW);
        autohome.sendPacket( packet.c_str() );

      }

      if (getValue(packet, ':', 1).equals("DYNAMIC")) {
  
        String colors = getValue(packet, ':', 2);
  
        prevR = currR;
        prevG = currG;
        prevB = currB;
        prevW = currW;
  
        currR = getValue(colors, ',' , 0).toInt();
        currG = getValue(colors, ',' , 1).toInt();
        currB = getValue(colors, ',' , 2).toInt();
        currW = getValue(colors, ',' , 3).toInt();
  
        fadeTime = getValue(colors, ',' , 4).toInt();
        fade = true;
  
      }
    }

    Serial.print(packet);

  }

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


void setup() {

  // put your setup code here, to run once:

  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);

  pinMode(WHITE_PIN, OUTPUT);
  digitalWrite(WHITE_PIN, HIGH);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

  analogWriteFreq(500);
  analogWriteRange(255);

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  unsigned long currentMillis = millis();

  process_fade(currentMillis);

delay(5);

// Serial.println("testing");
}

void SETRGB(int r, int g, int b, int w) {

  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
  analogWrite(WHITE_PIN, w);

  prevR = r;
  prevG = g;
  prevB = b;
  prevW = w;
//
//        currR = prevR;
//        currG = prevG;
//        currB = prevB;
//        currW = prevW;

}

void process_fade(long currentMillis) {

  if (fade) {

    SETRGB(prevR,prevG,prevB,prevW);

    if (currentMillis - previousMillisFade >= fadeTime) {

      previousMillisFade = currentMillis;

      if (prevR < currR) {
        prevR++;
      }

      if (prevG < currG) {
        prevG++;
      }

      if (prevB < currB) {
        prevB++;
      }

      if (prevW < currW) {
        prevW++;
      }

      if (prevR > currR) {
        prevR--;
      }

      if (prevG > currG) {
        prevG--;
      }

      if (prevB > currB) {
        prevB--;
      }

      if (prevB > currB) {
        prevB--;
      }

      if (prevW > currW) {
        prevW--;
      }

      if (prevR == currR && prevG == currG && prevB == currB && prevW == currW) {

        fade = false;
        previousMillisFade = 0;

        SETRGB(prevR,prevG,prevB,prevW);

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB) + "," + String(currW);
        autohome.sendPacket( packet.c_str() );

      }

    }

  }

}
