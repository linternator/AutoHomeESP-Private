#include <AutoHome.h>

/* ESP Socket modded software designed for the Sonoff smart switch

   Pinouts :
             - Switch GPIO0 - D0
             - LED Green - D13
             - Relay - D12

*/

AutoHome autohome;

#define G_LED 13
#define RELAY 12
#define BUTTON 0

boolean outputState = false;
boolean identify = false;
long identifyStart = 0;
int identifyCount = 0;
long buttonStart = 0;
int buttonCount = 0;


void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  if (autohome.mqtt_callback(topic, payload, length) == 0) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    if (autohome.getValue(packet, ':', 0).equals("SOCKET")) {

      if (autohome.getValue(packet, ':', 1).equals("GETSTATE")) {

        if (outputState) {
          autohome.sendPacket("SOCKET:STATE:1");
        } else {
          autohome.sendPacket("SOCKET:STATE:0");
        }

      } else if (autohome.getValue(packet, ':', 1).equals("IDENTIFY")) {

        identify = true;
        identifyStart = millis();

      } else if (autohome.getValue(packet, ':', 1).equals("SETSTATE")) {

        if (autohome.getValue(packet, ':', 2).equals("1")) {

          digitalWrite(RELAY, HIGH);
          outputState = true;
          autohome.sendPacket("SOCKET:STATE:1");

        } else if (autohome.getValue(packet, ':', 2).equals("0")) {

          digitalWrite(RELAY, LOW);
          outputState = false;
          autohome.sendPacket("SOCKET:STATE:0");

        }

      }

    }

    Serial.println(packet);

  }

}

void setup() {

  Serial.begin(115200);

  pinMode(G_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(RELAY, OUTPUT);

  digitalWrite(G_LED, HIGH);

  autohome.setPacketHandler(mqtt_callback);

  autohome.begin();

}

void loop() {

  if (digitalRead(BUTTON) == LOW) {

    if ((millis() - buttonStart) > 1000) {

      buttonStart = millis();

      if (buttonCount > 5) {

        buttonCount = 0;
        digitalWrite(RELAY, !digitalRead(RELAY));

      }

      buttonCount++;

    }

  }


  if (identify) {

    if ((millis() - identifyStart) > 100) {

      identifyStart = millis();

      if (identifyCount > 10) {

        identify = false;
        identifyCount = 0;
        digitalWrite(G_LED, HIGH);

      }

      identifyCount++;
      digitalWrite(G_LED, !digitalRead(G_LED));

    }

  } else {

    outputState = digitalRead(RELAY);
    digitalWrite(G_LED, !outputState);

  }

  autohome.loop();

}
