#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;
WiFiUDP udpSend;

/* Network Settings that can be changed */
const char* ssid     = "Lintern-Auto";
const char* password = "autohome";
byte serverIP[] = {192, 168, 1, 117};

/* Device Settings that can be changed */
String id = "4";
String deviceName = "LIGHTSENSOR";
String deviceType = "ANALOG_SENSOR";

/* Group Settings */
String groupID = "GLOBAL";

/* Static Settings */
byte ipMulti[] = {224, 0, 0, 3};
unsigned int portMulti = 12345;
char packetBuffer[255]; //buffer to hold incoming packet
Ticker t;

void sendName() {

  Serial.println("Name Update Sent");
  
  udpSend.beginPacket(serverIP, 54321);
  String message = String("NAMER:" + id + ":" + deviceName + ":" + deviceType + ":" + groupID);
  char charBuf[50];
  message.toCharArray(charBuf, 50);
  Serial.println(udpSend.write(charBuf));
  udpSend.endPacket();
  
}

void getPinMode() {

  Serial.println("Pin Mode Request Sent");
  
  udpSend.beginPacket(serverIP, 54321);
  String message = String("PMREQ:" + id);
  char charBuf[50];
  message.toCharArray(charBuf, 50);
  udpSend.write(charBuf);
  udpSend.endPacket();
  
}

void sendAnalogValue(){
 
  int an = analogRead(A0);
  
  Serial.print("Analog Reading Sent");
  Serial.print(an);
  Serial.println();
  
  udpSend.beginPacket(serverIP, 12321);
  String message = String("ANVAL:" + id + ":" + an);
  char charBuf[50];
  message.toCharArray(charBuf, 50);
  udpSend.write(charBuf);
  udpSend.endPacket();
  
}

void setup() {

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  udp.beginMulticast(WiFi.localIP(), ipMulti, portMulti);
  udpSend.begin(54321);
  
  delay(100);
  
  Serial.println("Started Up");
  
  sendName();

}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void interruptPacket(){
 
  udpSend.beginPacket(serverIP, 54321);
  String message = String("INTR:" + id);
  char charBuf[50];
  message.toCharArray(charBuf, 50);
  Serial.println(udpSend.write(charBuf));
  udpSend.endPacket();
  
}

void loop() {
    
  int packetSize = udp.parsePacket(); // note that this includes the UDP header
  if (packetSize)
  {

    // read the packet into packetBufffer
    int len = udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;

    String packet = String(packetBuffer);
    
    Serial.println("Packet : " + packet);
    
    if (getValue(packet, ':', 1).equals(id)) {

       Serial.println("Packet for me :-)");
      
      if ( getValue(packet, ':', 0).equals("NAMEC") ) {

        Serial.println("Name Check");
        
        sendName();

      }

      if ( getValue(packet, ':', 0).equals("SSGPIO") ) {

        Serial.println("Set GPIO");

        digitalWrite(getValue(packet, ':', 2).toInt(), getValue(packet, ':', 3).toInt());
        
        Serial.println("Setting pin " + getValue(packet, ':', 2) + " to " + getValue(packet, ':', 3));

      }
      
      if ( getValue(packet, ':', 0).equals("PMGPIO") ) {

        Serial.println("Set Port Map for GPIO");
        
        if(getValue(packet, ':', 3).equals("INPUT")){
          
          pinMode(getValue(packet, ':', 2).toInt(), INPUT);
          Serial.println("Setting pin " + getValue(packet, ':', 2) + " to INPUT");
          
        } else if(getValue(packet, ':', 3).equals("OUTPUT")){
         
          pinMode(getValue(packet, ':', 2).toInt(), OUTPUT);
          Serial.println("Setting pin " + getValue(packet, ':', 2) + " to OUTPUT");
          
        }     

      }
      
      if ( getValue(packet, ':', 0).equals("ISGPIO") ) {

        Serial.println("Set Interupt for GPIO");

        if(getValue(packet, ':', 3).equals("CHANGE")){
          
          attachInterrupt(getValue(packet, ':', 2).toInt(), interruptPacket, CHANGE);
          
        } else if(getValue(packet, ':', 3).equals("RISING")){
          
          attachInterrupt(getValue(packet, ':', 2).toInt(), interruptPacket, RISING);
          
        } else if(getValue(packet, ':', 3).equals("FALLING")){
          
          attachInterrupt(getValue(packet, ':', 2).toInt(), interruptPacket, FALLING);
          
        }

      }

      if( getValue( packet, ':', 0).equals("GSGPIO") ){
       
          int pin = getValue(packet, ':', 2).toInt();
          int pinValue = digitalRead(pin);
          Serial.println("Pin State requested : " + pin );
          Serial.println("Pin State Sent : " + pinValue );
          udpSend.beginPacket(serverIP, 32123);
          String message = String("RSGPIO:" + id + ":" + getValue(packet, ':', 2).toInt() + ":" + pinValue);
          char charBuf[50];
          message.toCharArray(charBuf, 50);
          udpSend.write(charBuf);
          udpSend.endPacket();
        
      }
      
      if( getValue( packet, ':', 0).equals("TSGPIO") ){
       
         Serial.println("Toggle GPIO");
        
         digitalWrite(getValue(packet, ':', 2).toInt(), LOW);
         
         delay(getValue(packet, ':', 3).toInt());
         
         digitalWrite(getValue(packet, ':', 2).toInt(), HIGH);
         
        
      }
      
      if( getValue( packet, ':', 0).equals("ASGPIO") ){
       
         Serial.println("Analog GPIO Set");
        
         t.attach_ms(getValue(packet, ':', 2).toInt(), sendAnalogValue);
      }

    }

  }
  delay(10);

}
