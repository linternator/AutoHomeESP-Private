#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* Network Settings that can be changed */
const char* ssid     = "";
const char* password = "";
const char* mqtt_server = "192.168.1.10";
const char* mqtt_user = "";
const char* mqtt_password = "";

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

WiFiClient espClient;
PubSubClient pubclient(espClient);

void SETRGB(int r, int g, int b);
byte Read_Keypad(void);

void setup_wifi() {

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Started Up");

  analogWriteRange(255);

}

String getValue(String data, char separator, int index)  // daves usefull funciton
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

void callback(char* topic, byte* payload, unsigned int length) {  // decompoiseng the mqtt message

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  /* gets called when get message */
  /* getValue(packet, ':', 0).equals("RGB") */
  /* insert main code here */

  Serial.println(packet);
  if (getValue(packet, ':', 0).equals("RGB")) { // first part of the sting RGB

    if (getValue(packet, ':', 1).equals("OFF")) { // second part of the string "..."
      digitalWrite(ENABLE, HIGH);
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
    }

    if (getValue(packet, ':', 1).equals("STATIC")) { // second part of the string "..."
      String colors = getValue(packet, ':', 2);

      //    digitalWrite(ENABLE,LOW);

      SETRGB(getValue(colors, ',' , 0).toInt(), getValue(colors, ',' , 1).toInt(), getValue(colors, ',' , 2).toInt());

      String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);

      pubclient.publish( "/home/bedroom/lights/RGB", packet.c_str() );


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



  //  RGB:STATIC:255,255,255
}

void reconnect() {

  // Loop until we're reconnected
  while (!pubclient.connected()) {

    Serial.print("Attempting MQTT connection...");

    /* Connect to diffrent chennels */

    if (pubclient.connect("BedRoomLights", mqtt_user , mqtt_password)) {  // define in header
      Serial.println("connected");

      // Subscribe to chenels here
      pubclient.subscribe( "/home/bedroom/lights/RGB" );

    } else {
      Serial.print("failed, rc=");
      Serial.print(pubclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }

  }

}

void setup() {

  Serial.begin(115200);
  delay(10);

  setup_wifi();

  pubclient.setServer(mqtt_server, 1883);
  pubclient.setCallback(callback);

  reconnect();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);

  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);
}

void loop() {

  if (!pubclient.connected()) {
    reconnect();
  }

  //if( Read_Keypad() == 15)
  //{ SETRGB(R++,G++,B++);}
  unsigned long currentMillis = millis();

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

        pubclient.publish( "/home/bedroom/lights/RGB", packet.c_str() );

      }

      analogWrite(RED_PIN, prevR);
      analogWrite(GREEN_PIN, prevG);
      analogWrite(BLUE_PIN, prevB);

      if (prevR == 0 && prevG == 0 && prevB == 0 ) {
        digitalWrite(ENABLE, HIGH);
      }
      else
      {
        digitalWrite(ENABLE, LOW);
      }

    }
  }

  if (currentMillis - previousMillisKeypad >= 150) {
    // save the last time you blinked the LED
    previousMillisKeypad = currentMillis;

    // if anywhere this is where the fade would be.

    if (Read_Keypad() == 0 ) {

      if (buttonPress) {

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);

        pubclient.publish( "/home/bedroom/lights/RGB", packet.c_str() );

        buttonPress = false;

      }

    } else {

      buttonPress = true;

      // Serial.println("tick   tock");
      switch ( Read_Keypad() ) {

        case 1:
          {
            if (currR <= 247) {
              SETRGB((currR + 8) , currG, currB);
            }
            else {
              SETRGB((currR = 255) , currG, currB);
            }
          } break;

        case 5:
          {
            if (currR >= 8) {
              SETRGB((currR - 8) , currG, currB);
            }
            else {
              SETRGB((currR = 0) , currG, currB);
            }
          } break;

        case 2:
          {
            if (currG <= 247) {
              SETRGB(currR , (currG + 8), currB);
            }
          } break;

        case 6:
          {
            if (currG >= 8) {
              SETRGB(currR, (currG - 8), currB);
            }
          } break;

        case 3:
          {
            if (currB <= 247) {
              SETRGB(currR , currG, (currB + 8));
            }
          } break;

        case 7:
          {
            if (currB >= 8) {
              SETRGB(currR , currG, (currB - 8));
            }
          } break;

        case 13:
          {
            SETRGB(255, 255, 0);
          } break;

        case 14:
          {
            SETRGB(50, 50, 254); //lerhaps fine tune these.
            // even have these as mqtt defined funtiins and colors.
          } break;

        case 16:
          {
            SETRGB(255 , 255, 255);
          } break;

        case 15:
          {
            SETRGB(0 , 0, 0);
          } break;

      }

    }


  }

  pubclient.loop();

}

void SETRGB(int r, int g, int b)
{
  if (r == 0 && g == 0 && b == 0 ) {
    digitalWrite(ENABLE, HIGH);
  }
  else
  {
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

