#include <AutoHome.h>

/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */
char const* wifi_ssid = "RG-Wifi";
char const* wifi_password = "STBCrg123";
char const* mqtt_broker_ip = "82.13.163.212";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "David-Socket";
char const* mqtt_topic = "/home/David/Socket";

AutoHome autohome;

#define LED_PIN 14
#define RELAY 16
#define BUTTON1 13
#define BUTTON2 12

boolean LEDFlash = false;
unsigned long previousMillisLED = 0;
int buttonState1 = 0;
int lastButtonState1 = 1;
int buttonState2 = 0;
int lastButtonState2 = 1;


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


/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  Serial.print(packet);

  if (getValue(packet, ':', 0).equals("LED")) {

    if (getValue(packet, ':', 1).equals("OFF")) {

      LEDFlash = false;

      digitalWrite(LED_PIN, LOW);

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


void setup() {
  // put your setup code here, to run once:

  
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);


  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);



}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  unsigned long currentMillis = millis();  

  if (LEDFlash) {

    if (currentMillis - previousMillisLED > 300) {

      previousMillisLED = currentMillis;

      digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    }

  }

  buttonState1 = digitalRead(BUTTON1);

  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1 == HIGH) {

      autohome.sendPacket( "SOCKET:BUTTON1" );
      digitalWrite(RELAY, !digitalRead(RELAY));
      digitalWrite(LED_PIN, digitalRead(RELAY));

      
      String packet = "SOCKET:IS:" + String(digitalRead(RELAY));       

      autohome.sendPacket( packet.c_str() );

    }

    lastButtonState1 = buttonState1;

  }

  buttonState2 = digitalRead(BUTTON2);

  // compare the buttonState to its previous state
  if (buttonState2 != lastButtonState2) {
    // if the state has changed, increment the counter
    if (buttonState2 == HIGH) {

      autohome.sendPacket( "SOCKET:BUTTON2" );

    }

    lastButtonState2 = buttonState2;

  }


}
