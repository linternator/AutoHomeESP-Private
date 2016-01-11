#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

/* Network Settings that can be changed */
const char* ssid     = "RG-Wifi";
const char* password = "STBCrg123";
const char* mqtt_server = "82.13.163.212";
const char* mqtt_user = "autohome";
const char* mqtt_password = "autoHOME";
const char* mqtt_client = "BMP085";

WiFiClient espClient;
PubSubClient pubclient(espClient);
Adafruit_BMP085 bmp;

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

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void callback(char* topic, byte* payload, unsigned int length) {

  String packet = "";

   for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  /* gets called when get message */
  /* getValue(packet, ':', 0).equals("RGB") */
  /* insert main code here */

}

void reconnect() {
  
  // Loop until we're reconnected
  while (!pubclient.connected()) {
   
    Serial.print("Attempting MQTT connection...");

  /* Connect to diffrent chennels */
//    if (pubclient.connect(mqtt_client, mqtt_user, mqtt_password)) {
//      Serial.println("connected");
//
//      // Subscribe to chenels here
////      pubclient.publish("/home/bedroom/bmp085", "Hello World");
//      // pubclient.subscribe(<Topic>);
//
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(pubclient.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(5000);
//    }
    
  }
  
}

void setup() {

  Serial.begin(115200);
  delay(10);

  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }

  setup_wifi();

  pubclient.setServer(mqtt_server, 1883);
  pubclient.setCallback(callback);

  reconnect();

}

void loop() {

   if (!pubclient.connected()) {
    reconnect();
   }
  
   pubclient.loop();

}
