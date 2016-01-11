/* MQTT Code (mqtt object is 'pubclient') */
#include "WIFI.h"

/* Subscribe to chenels here */
/* Use pubclient.subscribe(<Topic>); */
void mqtt_subscribe(){

  
  
}

/* Gets called when get message from any topic*/
void mqtt_callback(char* topic, byte* payload, unsigned int length){

    
  
}

/* Arduino code add any custom code after autohome */
#include "AutoHome.h"

void setup() {

  autohome_setup();

}

void loop() {

  autohome_loop();

}
