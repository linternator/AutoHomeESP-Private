/* This example is a simple setup example to show you how to setup the AutoHome library. */
/* If you run this example with the variables filled out the esp will print out any */
/* received packets from the mqtt broker */

#include <AutoHome.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>


/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */
char const* wifi_ssid = "RG-Wifi";
char const* wifi_password = "STBCrg123";
char const* mqtt_broker_ip = "192.168.1.10";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "Room-sensor";
char const* mqtt_topic = "/Home/Bedroom/Sensor";

unsigned long previousemillis = 0;
int Time = 500;

AutoHome autohome;

Adafruit_BMP085 bmp;


/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length){

  String packet = "";

  for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  Serial.print(packet);
  
}


void PIR()
{
  Serial.println("movement!!!");

  String packet = "MOVMENT";
  autohome.sendPacket(packet.c_str());

}


void setup() {
  // put your setup code here, to run once:
  Wire.begin(13, 12);
// pir atached to pin 14


  pinMode(14, INPUT);
 // attachInterrupt( 14, PIR, RISING);

  Serial.begin(115200);
  autohome.setPacketHandler(mqtt_callback);
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);

   if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }

}

void loop() {
autohome.loop();

unsigned long currentMills = millis();
          // previousemillis

     if (currentMills - previousemillis <= Time)
     {
       previousemillis = currentMills;
        Serial.println("bam");
  String packet = ("bmpT:" + String(bmp.readTemperature()) + ".bmpP:" + String(bmp.readPressure()) + ".bmpH:" + String(bmp.readAltitude()));
  autohome.sendPacket(packet.c_str());
     }

}





