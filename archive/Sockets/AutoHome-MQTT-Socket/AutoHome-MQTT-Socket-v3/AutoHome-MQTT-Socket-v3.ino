#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED_PIN 14
#define RELAY 16
#define BUTTON1 13
#define BUTTON2 12

/* Network Settings that can be changed */
const char* ssid     = "RG-Wifi";
const char* password = "STBCrg123";
const char* mqtt_server = "82.13.163.212";
const char* mqtt_user = "autohome";
const char* mqtt_password = "autoHOME";
const char* mqtt_client = "WallSocketLights";

WiFiClient espClient;
PubSubClient pubclient(espClient);

boolean LEDFlash = false;
unsigned long previousMillisLED = 0;
int buttonState1 = 0;
int lastButtonState1 = 1;
int buttonState2 = 0;
int lastButtonState2 = 1;

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
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  digitalWrite(LED_PIN, LOW);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Started Up");

  analogWriteRange(255);

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

void callback(char* topic, byte* payload, unsigned int length) {

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  if (getValue(packet, ':', 0).equals("LED")) {

    if (getValue(packet, ':', 1).equals("OFF")) {

      LEDFlash = false;
//      digitalWrite(LED_PIN, digitalRead(RELAY));

    }


    if (getValue(packet, ':', 1).equals("FLASH")) {

      LEDFlash = true;

    }

  }

  if (getValue(packet, ':', 0).equals("SOCKET")) {

    if (getValue(packet, ':', 1).equals("OFF")) {
      
      digitalWrite(RELAY, LOW);
	  digitalWrite(LED_PIN, digitalRead(RELAY));

    }


    if (getValue(packet, ':', 1).equals("ON")) {

      digitalWrite(RELAY, HIGH);
	  digitalWrite(LED_PIN, digitalRead(RELAY));

    }
    
  }

}

void reconnect() {

  // Loop until we're reconnected
  while (!pubclient.connected()) {

    Serial.print("Attempting MQTT connection...");

    /* Connect to diffrent chennels */

    if (pubclient.connect(mqtt_client, mqtt_user, mqtt_password)) {
      Serial.println("connected");

      // Subscribe to chenels here
      pubclient.subscribe("/home/bedroom/sockets/1");
      // pubclient.subscribe(<Topic>);

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

  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  Serial.begin(115200);
  delay(10);

  setup_wifi();

  pubclient.setServer(mqtt_server, 1883);
  pubclient.setCallback(callback);

  reconnect();

  digitalWrite(RELAY, HIGH);

}

void loop() {

  unsigned long currentMillis = millis();

  if (!pubclient.connected()) {

    reconnect();

  }

  if (LEDFlash) {

//    if (currentMillis - previousMillisLED > 300) {
//
//      previousMillisLED = currentMillis;
//
//      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
//
//    }

  }

  buttonState1 = digitalRead(BUTTON1);

  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1 == HIGH) {

      pubclient.publish( "/home/bedroom/sockets/1", "SOCKET:BUTTON1" );
      digitalWrite(RELAY, HIGH);
      digitalWrite(LED_PIN, digitalRead(RELAY));

    }

    lastButtonState1 = buttonState1;

  }

  buttonState2 = digitalRead(BUTTON2);

  // compare the buttonState to its previous state
  if (buttonState2 != lastButtonState2) {
    // if the state has changed, increment the counter
    if (buttonState2 == HIGH) {

      pubclient.publish( "/home/bedroom/sockets/1", "SOCKET:BUTTON2" );
      digitalWrite(RELAY, LOW);
	  digitalWrite(LED_PIN, digitalRead(RELAY));

    }

    lastButtonState2 = buttonState2;

  }

  pubclient.loop();

}
