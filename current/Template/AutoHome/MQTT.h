void callback(char* topic, byte* payload, unsigned int length) {

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  mqtt_callback(topic, payload, length);

}

void reconnect() {

  // Loop until we're reconnected
  while (!pubclient.connected()) {

    Serial.print("Attempting MQTT connection...");

    /* Connect to diffrent chennels */

    if (pubclient.connect(mqtt_client, mqtt_user, mqtt_password)) {
      Serial.println("connected");

      mqtt_subscribe();

    } else {
      Serial.print("failed, rc=");
      Serial.print(pubclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }

  }

}
