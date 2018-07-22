#include <AutoHome.h>

char const* host_name = "white-bulbRGB";
char const* mqtt_topic = "/home/white-bulbRGB";


AutoHome autohome;

/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */


#define RED_PIN 13 //11; // CORRECT
#define GREEN_PIN 12  // CORRECT
#define BLUE_PIN 14 // CORRECT
#define WHITE_PIN 2 // CORRECT
#define ENABLE 15 // PSU on off pin

char incomingByte;

// #define SCL_PIN 4
// #define SDO_PIN 5

int currR = 0;
int currG = 0;
int currB = 0;
int currW = 0;

int prevR = 0;
int prevG = 0;
int prevB = 0;
int prevW = 0;

unsigned long previousMillisKeypad = 0;
unsigned long previousMillisFade = 0;
int fadeTime = 0;

// int lastButton = 0;

boolean fade = false;
boolean buttonPress = false;

void SETRGB(int r, int g, int b, int w);
// byte Read_Keypad(void);
void process_fade(long currentMillis);
// void processKeypad();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);

  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);

//  pinMode(SCL_PIN, OUTPUT);
//  pinMode(SDO_PIN, INPUT);

  analogWriteRange(255);

  // check for last state and set to that

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  unsigned long currentMillis = millis();

  process_fade(currentMillis);

if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
if(incomingByte == 'D')
    {
      autohome.resetSettings();
      Serial.println("eraseing flash ...");
      }
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

/* Gets called when get message from any topic*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  if (getValue(packet, ':', 0).equals("RGB")) {

    if (getValue(packet, ':', 1).equals("OFF")) {

      digitalWrite(ENABLE, HIGH);
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      analogWrite(WHITE_PIN, 0);

    }

    if (getValue(packet, ':', 1).equals("STATIC")) {

      String colors = getValue(packet, ':', 2);
      SETRGB(getValue(colors, ',' , 0).toInt(), getValue(colors, ',' , 1).toInt(), getValue(colors, ',' , 2).toInt(), getValue(colors, ',' , 3).toInt());

      String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB) + "," + String(currW) ;
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

}

void SETRGB(int r, int g, int b, int w) {

  if (r == 0 && g == 0 && b == 0 && w == 0) {

    digitalWrite(ENABLE, HIGH);

  } else {

    digitalWrite(ENABLE, LOW);

  }

  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
  analogWrite(WHITE_PIN, w);

  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  Serial.println(w);

  currR = r;
  currG = g;
  currB = b;
  currW = w;

}


void process_fade(long currentMillis) {

  if (fade) {

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

            if (prevW > currW) {

        prevW--;

      }


      if (prevR == currR && prevG == currG && prevB == currB && prevW == currW) {

        fade = false;
        previousMillisFade = 0;

        currR = prevR;
        currG = prevG;
        currB = prevB;
        currW = prevW;

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB) + "," + String(currW);
        autohome.sendPacket( packet.c_str() );

      }

      analogWrite(RED_PIN, prevR);
      analogWrite(GREEN_PIN, prevG);
      analogWrite(BLUE_PIN, prevB);
      analogWrite(WHITE_PIN, prevW);

      if (prevR == 0 && prevG == 0 && prevB == 0 && prevW == 0) {

        digitalWrite(ENABLE, HIGH);

      } else {

        digitalWrite(ENABLE, LOW);

      }

    }

  }

}

