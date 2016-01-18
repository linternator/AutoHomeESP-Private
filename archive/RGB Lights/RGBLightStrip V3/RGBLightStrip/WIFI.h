#include <ESP8266WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;

#include "Configuration.h"
#include "StringUtil.h"

PubSubClient pubclient(espClient);

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
