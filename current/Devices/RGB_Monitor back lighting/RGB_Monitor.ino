/* This example is a simple setup example to show you how to setup the AutoHome library. */

/*

      Blue - D14
      Green - D12
      Red - D16

*/

#include <AutoHome.h>

AutoHome autohome;

#define RED_PIN 16
#define GREEN_PIN 12
#define BLUE_PIN 14

int currR = 0;
int currG = 0;
int currB = 0;

void SETRGB(int r, int g, int b);

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
        SETRGB(autohome.getValue(colors, ',' , 0).toInt(), autohome.getValue(colors, ',' , 1).toInt(), autohome.getValue(colors, ',' , 2).toInt());

        String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
        autohome.sendPacket( packet.c_str() );

      }

    }

    Serial.print(packet);
  }

}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

}

void SETRGB(int r, int g, int b) {

  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);

  currR = r;
  currG = g;
  currB = b;

}
