#include <AutoHome.h>

AutoHome autohome;

/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */


char const* wifi_ssid = "RG-Wifi"; // "RG-Wifi";
char const* wifi_password = "STBCrg123"; // "STBCrg123";
char const* mqtt_broker_ip =  "192.168.1.10"; // "192.168.1.10";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "keypad";
char const* mqtt_topic = "/home/bedroom/keypad";

#define SCL_PIN 4
#define SDO_PIN 5

unsigned long previousMillisKeypad = 0;


int lastButton = 0;

boolean buttonPress = false;

byte Read_Keypad(void);
void processKeypad(int keyIn);
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);

  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisKeypad >= 150) {

    previousMillisKeypad = currentMillis;

    int keyIn = Read_Keypad();

    if ( keyIn == 0 ) {

      if (buttonPress) {

        buttonPress = false;

      }

      lastButton = 0;

    } else {

      buttonPress = true;

      processKeypad(keyIn);

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
    {
      
      Key_State = Count;
    
    //  Serial.write(Key_State);
    }
    
    digitalWrite(SCL_PIN, HIGH);
  }

  return Key_State;
}

void processKeypad(int keyIn) {

    Serial.print(keyIn);
    Serial.print(lastButton);
    
  if (lastButton != keyIn) {

    String packet = "Bedroom:" + String(keyIn);
    autohome.sendPacket( packet.c_str() );
    
  }
  
  lastButton = keyIn;
  
}
