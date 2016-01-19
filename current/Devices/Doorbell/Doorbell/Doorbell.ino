#include <dummy.h>

/* This example is a simple setup example to show you how to setup the AutoHome library. */
/* If you run this example with the variables filled out the esp will print out any */
/* received packets from the mqtt broker */

#include <AutoHome.h>

/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */
char const* wifi_ssid = "RG-Wifi";
char const* wifi_password = "STBCrg123";
char const* mqtt_broker_ip = "192.168.1.10";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "DoorBell";
char const* mqtt_topic = "/home/door";

AutoHome autohome;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);

  autohome.sendPacket("Doorbell:Button:0");

  ESP.deepSleep(500);

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

}
