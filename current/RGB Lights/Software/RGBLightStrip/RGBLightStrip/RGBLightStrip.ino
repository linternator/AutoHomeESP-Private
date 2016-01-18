#include <AutoHome.h>

AutoHome autohome;

/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */
char const* wifi_ssid = "RG-Wifi";
char const* wifi_password = "STBCrg123";
char const* mqtt_broker_ip = "192.168.1.10";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "Bedroom-Lights";
char const* mqtt_topic = "/home/bedroom/lights/RGB";

#define RED_PIN 12 //11; // CORRECT  
#define GREEN_PIN 13  // CORRECT
#define BLUE_PIN 14 // CORRECT
#define ENABLE 2 // PSU on off pin

#define SCL_PIN 4
#define SDO_PIN 5

int currR = 0;
int currG = 0;
int currB = 0;

int prevR = 0;
int prevG = 0;
int prevB = 0;

unsigned long previousMillisKeypad = 0;
unsigned long previousMillisFade = 0;
int fadeTime = 0;

boolean fade = false;
boolean buttonPress = false;

void SETRGB(int r, int g, int b);
byte Read_Keypad(void);
void process_fade(long currentMillis);
void processKeypad();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {
  
  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);

  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);

}

void loop() {
 
  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  unsigned long currentMillis = millis();

  process_fade(currentMillis);

  if (currentMillis - previousMillisKeypad >= 150) {

    previousMillisKeypad = currentMillis;

    if (Read_Keypad() == 0 ) {

      if (buttonPress) {

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
        autohome.sendPacket( packet.c_str() );

        buttonPress = false;

      }

    } else {

      buttonPress = true;

      processKeypad();

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

    }

    if (getValue(packet, ':', 1).equals("STATIC")) {

      String colors = getValue(packet, ':', 2);
      SETRGB(getValue(colors, ',' , 0).toInt(), getValue(colors, ',' , 1).toInt(), getValue(colors, ',' , 2).toInt());

      String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
      autohome.sendPacket( packet.c_str() );


    }

    if (getValue(packet, ':', 1).equals("DYNAMIC")) {

      String colors = getValue(packet, ':', 2);

      prevR = currR;
      prevG = currG;
      prevB = currB;

      currR = getValue(colors, ',' , 0).toInt();
      currG = getValue(colors, ',' , 1).toInt();
      currB = getValue(colors, ',' , 2).toInt();

      fadeTime = getValue(colors, ',' , 3).toInt();
      fade = true;

    }

  }

}

void SETRGB(int r, int g, int b) {

  if (r == 0 && g == 0 && b == 0 ) {

    digitalWrite(ENABLE, HIGH);

  } else {

    digitalWrite(ENABLE, LOW);

  }

  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);

  Serial.println(r);
  Serial.println(g);
  Serial.println(b);

  currR = r;
  currG = g;
  currB = b;

}

/* Read the state of the keypad */
byte Read_Keypad(void)
{
  byte Count;
  byte Key_State = 0;

  /* Pulse the clock pin 16 times (one for each key of the keypad)
     and read the state of the data pin on each pulse */
  for (Count = 1; Count <= 16; Count++)
  {
    digitalWrite(SCL_PIN, LOW);

    /* If the data pin is low (active low mode) then store the
       current key number */
    if (!digitalRead(SDO_PIN))
    { Key_State = Count;
      Serial.write(Key_State);
    }
    digitalWrite(SCL_PIN, HIGH);
  }

  return Key_State;
  //  delay(10);
}

void processKeypad() {

  switch ( Read_Keypad() ) {

    case 1: {

        if (currR <= 247) {

          SETRGB((currR + 8) , currG, currB);

        } else {

          SETRGB((currR = 255) , currG, currB);

        }

        break;

      }

    case 5: {

        if (currR >= 8) {

          SETRGB((currR - 8) , currG, currB);

        } else {

          SETRGB((currR = 0) , currG, currB);

        }

        break;

      }

    case 2: {

        if (currG <= 247) {

          SETRGB(currR , (currG + 8), currB);

        }

        break;

      }

    case 6: {

        if (currG >= 8) {

          SETRGB(currR, (currG - 8), currB);

        }

        break;

      }

    case 3: {

        if (currB <= 247) {

          SETRGB(currR , currG, (currB + 8));

        }

        break;

      }

    case 7:
      {
        if (currB >= 8) {

          SETRGB(currR , currG, (currB - 8));

        }

        break;

      }

    case 13: {

        SETRGB(255, 255, 0);

        break;

      }

    case 14: {

        SETRGB(50, 50, 254);

        break;

      }

    case 16: {

        SETRGB(255 , 255, 255);

        break;

      }

    case 15: {

        SETRGB(0 , 0, 0);

        break;

      }

  }

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

      if (prevR > currR) {

        prevR--;

      }

      if (prevG > currG) {

        prevG--;

      }

      if (prevB > currB) {

        prevB--;

      }

      if (prevR == currR && prevG == currG && prevB == currB) {

        fade = false;
        previousMillisFade = 0;

        currR = prevR;
        currG = prevG;
        currB = prevB;

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
        autohome.sendPacket( packet.c_str() );

      }

      analogWrite(RED_PIN, prevR);
      analogWrite(GREEN_PIN, prevG);
      analogWrite(BLUE_PIN, prevB);

      if (prevR == 0 && prevG == 0 && prevB == 0 ) {

        digitalWrite(ENABLE, HIGH);

      } else {

        digitalWrite(ENABLE, LOW);

      }

    }

  }

}

