/* This example is a simple setup example to show you how to setup the AutoHome library. */

#include <AutoHome.h>

#define sal6 13
#define sal5 12
#define sal4 14
#define sal3 27
#define sal2 26
#define sal1 25
#define stue 33
#define kalder 32
#define stop 35
#define help 34

int deBounce = 242;


AutoHome autohome;

/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
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
  // put your setup code here, to run once:

    pinMode( sal6 , INPUT_PULLUP);
    pinMode( sal5 , INPUT_PULLUP);
    pinMode( sal4 , INPUT_PULLUP);
    pinMode( sal3 , INPUT_PULLUP);
    pinMode( sal2 , INPUT_PULLUP);
    pinMode( sal1 , INPUT_PULLUP);
    pinMode( stue , INPUT_PULLUP);
    pinMode( kalder , INPUT_PULLUP);
    pinMode( stop , INPUT_PULLUP);
    pinMode( help , INPUT_PULLUP);  // PULL DOWN ???

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

  delay(deBounce);

  if( digitalRead(sal6) == LOW)
    {       // send MQTT message
       String packetOUT = "SAL6";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(sal6) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(sal5) == LOW)
    {       // send MQTT message
       String packetOUT = "SAL5";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(sal5) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(sal4) == LOW)
    {       // send MQTT message
       String packetOUT = "SAL4";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
              while( digitalRead(sal4) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(sal3) == LOW )
    {       // send MQTT message
       String packetOUT = "SAL3";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
              while( digitalRead(sal3) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(sal2) == LOW)
    {       // send MQTT message
       String packetOUT = "SAL2";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
            while( digitalRead(sal2) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(sal1) == LOW)     // too much work. 
    {       // send MQTT message
       String packetOUT = "SAL1";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
            while( digitalRead(sal1) == 0)
                {
                  delay(2);
                }
    }

  if( digitalRead(stue) == LOW) // too much workd
    {       // send MQTT message
       String packetOUT = "STUE";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(stue) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(kalder) == LOW) // too much work
    {       // send MQTT message
       String packetOUT = "KALDER";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
        while( digitalRead(kalder) == 0)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(stop) == HIGH)         // active high // ADD pullup resitor ? 
    {       // send MQTT message
       String packetOUT = "stop";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
        while( digitalRead(stop) == HIGH)
                {
                  delay(deBounce);
                }
    }

  if( digitalRead(help) == 0)              // ALLWAYS TRIGGERED. add pull up resitor ? 
    {       // send MQTT message
       String packetOUT = "help";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
        while( digitalRead(help) == 0)
                {
                  delay(deBounce);
                }
    }

   
    
}





// might be an issue with gpio pull up ? consider adding pull up resitor in hardwar for bottom 5 pins
