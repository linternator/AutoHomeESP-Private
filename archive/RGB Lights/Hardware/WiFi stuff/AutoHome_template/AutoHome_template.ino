#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;
WiFiUDP udpSend;

/* Network Settings that can be changed */
const char* ssid     = "";
const char* password = "";
byte serverIP[] = {192, 168, 1, 117};

/* Device Settings that can be changed */
String id = "5";
String deviceName = "RGLights";
String deviceType = "RGBLEDS";

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

      // if( getValue(packet, ':', 0).equals("<udp command>) ) { }

    }

  }
  
  delay(10);

}
