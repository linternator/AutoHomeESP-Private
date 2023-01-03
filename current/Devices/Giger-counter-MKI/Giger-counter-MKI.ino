#include <PubSubClient.h>


// converitons
// https://cs.stanford.edu/people/nick/geiger/
// https://cs.stanford.edu/people/nick/mrgeiger/

#include <AutoHome.h>
// PubSubClient = version 2.5.0

AutoHome autohome;

int PULSE_PIN = 27;

int BEEPER = 14;

bool beeper_var = false;

bool countYES = true;  // send MQTT beep.

unsigned long previousMillis = 0;  // will store last time the count was messured.
const long interval = 60000;       // interval of messuring the count (each minute) each 60 seconds

float count = 0;
float uSievert = 0;


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  if (!autohome.mqtt_callback(topic, payload, length)) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    Serial.print(packet);
    
  if (autohome.getValue(packet, ':', 0).equals("beeper=1")) // beep IRL on 
  {
    digitalWrite(BEEPER, HIGH);
    mqtt_send_stats();
  }

  if (autohome.getValue(packet, ':', 0).equals("beeper=0")) // beep IRL off
  {
    digitalWrite(BEEPER, LOW);
    mqtt_send_stats();
  }


  if (autohome.getValue(packet, ':', 0).equals("pulseMQTT=0"))  // send MQTT beep off
  {
    countYES = false;
    mqtt_send_stats();
  }

  if (autohome.getValue(packet, ':', 0).equals("pulseMQTT=1"))  // send MQTT beep on
  {
    countYES = true;
    mqtt_send_stats();
  }

  if (autohome.getValue(packet, ':', 0).equals("status"))  // send status
  {
    mqtt_send_stats();
  }

  }  
}


void mqtt_send_stats()
{
  String packet = 
                  "beeper : " + String(digitalRead(BEEPER)) +
                  " mqtt_beep : " + String(beeper_var) +
                  " uSievert / H : " + String(uSievert) +
                  " count per min : " + String(count)                  
                 ;
  autohome.sendPacket(packet.c_str());
}

void setup() {

  pinMode(PULSE_PIN, INPUT);
  pinMode(BEEPER, OUTPUT);
//  digitalWrite(BEEPER, HIGH);

  autohome.setPacketHandler(mqtt_callback);

  autohome.begin();

  Serial.begin(115200);
}

void loop() {

  // if recive MQTT message


  autohome.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // multiply its Counts Per minute by 0.0057 to get the equivalent uSv/hr radiation level

    mqtt_send_stats();

    uSievert = 0;
    uSievert = (count * 0.0057);
    count = 0;
  }


  if (digitalRead(PULSE_PIN) == LOW) {  // if giger counter ticks
    count++;

    if (countYES == true) {
      // send MQTT message.
      uSievert = (count * 0.0057);
        String packet = "Tick"                 ;
  autohome.sendPacket(packet.c_str());
    }

    while (digitalRead(PULSE_PIN) == LOW) {
      // wait
    }
  }
}
