#include <AutoHome.h>

struct Config {
  int stateUM;
  int pwm_thing;
};
struct Config conf = { 0,255 };
AutoHome autohome;


void mqtt_callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  if(!autohome.mqtt_callback(topic, payload, length)){

      String packet = "";

      for (int i = 0; i < length; i++) {
        packet = packet + (char)payload[i];
      }

      Serial.print(packet);

  }

}


void setup() {

  pinMode(2, INPUT);       // input to see what state its in.
  pinMode(0, OUTPUT);     // button output to controle it remotely.

  digitalWrite(0, LOW);  // dont press the Button.

  Serial.begin(115200);
  autohome.setPacketHandler(mqtt_callback);
  autohome.begin();
  readState(&conf);

}

void ButtonPush()
{
  digitalWrite(0, HIGH);
  delay(82);
  digitalWrite(0, LOW);
}

void loop() {

  autohome.loop();

  // if LED changes sned message update state.

  // select input button push 

}
template<typename T>
void readState(T* conf){

  LittleFS.begin();
  File veriables = LittleFS.open("/verables", "r");
  size_t fileSize = veriables.size();
  if (fileSize >= sizeof(*conf)){
    veriables.readBytes((char[sizeof(struct *conf)])*conf, sizeof(*conf));
  }
  veriables.close();
  LittleFS.end();
}
template<typename T>
void writeState(T* conf){
  LittleFS.begin();
  File veriables = LittleFS.open("/verables", "w");
  veriables.write(*conf, sizeof(*conf));
  veriables.close();
  LittleFS.end();
}
