#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "OTAUpdate.h"
#include "MQTT.h"

void autohome_setup() {

  Serial.begin(115200);
  delay(10);

  setup_wifi();

  setup_ota();

  pubclient.setServer(mqtt_server, 1883);
  pubclient.setCallback(callback);

  reconnect();

}

void autohome_loop(){

  if (!pubclient.connected()) {
    reconnect();
  }

  pubclient.loop();
  ArduinoOTA.handle();
  
}

