#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* Network Settings that can be changed */
const char* ssid     = "wifi name";
const char* password = "wifi password";
const char* mqtt_server = "server adress";

#define RED 12
#define GREEN 13
#define BLUE 14
#define ENABLE 2	// PSU controle pin

#define SCL_PIN 4	// clcok for buttons
#define SDO_PIN 5	// data pin for buttons

int R = 0;
int G = 0;
int B = 0;

unsigned long previousMillis = 0;	// delay place holder

WiFiClient espClient;
PubSubClient pubclient(espClient);

void setup_wifi() {

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
  Serial.println("Started Up");

  analogWriteRange(255);

}

String getValue(String data, char separator, int index)  // daves usefull funciton
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

void callback(char* topic, byte* payload, unsigned int length) {  // decompoiseng the mqtt message

  String packet = "";

   for (int i = 0; i < length; i++) {
    packet = packet + (char)payload[i];
  }

  /* gets called when get message */
  /* getValue(packet, ':', 0).equals("RGB") */
  /* insert main code here */

  Serial.println(packet);
  if(getValue(packet, ':', 0).equals("RGB")){  // first part of the sting RGB

    if(getValue(packet, ':', 1).equals("OFF")){  // second part of the string "..."
      digitalWrite(ENABLE,HIGH);
      analogWrite(RED,0);
      analogWrite(GREEN,0);
      analogWrite(BLUE,0);
    }

    if(getValue(packet, ':', 1).equals("STATIC")){  // second part of the string "..."
         String colors = getValue(packet, ':', 2);

       //    digitalWrite(ENABLE,LOW);

          SETRGB(getValue(colors, ',' ,0).toInt(), getValue(colors, ',' ,1).toInt(), getValue(colors, ',' ,2).toInt());

       //    analogWrite(RED, getValue(colors, ',' ,0).toInt());
       //    analogWrite(GREEN, getValue(colors, ',' ,1).toInt());
       //    analogWrite(BLUE, getValue(colors, ',' ,2).toInt());
    }

  }



//  RGB:STATIC:255,255,255
}

void reconnect() {

  // Loop until we're reconnected
  while (!pubclient.connected()) {

    Serial.print("Attempting MQTT connection...");

  /* Connect to diffrent chennels */

   if (pubclient.connect("BedRoomLights", "autohome" , "autoHOME")) {  // define in header
      Serial.println("connected");

       // Subscribe to chenels here
        pubclient.subscribe( "/home/bedroom/lights/RGB" ); 

    } else {
      Serial.print("failed, rc=");
      Serial.print(pubclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }

  }

}

void setup() {

  Serial.begin(115200);
  delay(10);

  setup_wifi();

  pubclient.setServer(mqtt_server, 1883);
  pubclient.setCallback(callback);

  reconnect();

  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(ENABLE,OUTPUT);
digitalWrite(ENABLE,HIGH);

 pinMode(SCL_PIN,OUTPUT);
 pinMode(SDO_PIN,INPUT);
}

void loop() {

   if (!pubclient.connected()) {
    reconnect();
   }

   //if( Read_Keypad() == 15)
   //{ SETRGB(R++,G++,B++);}
unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= 150) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

// Serial.println("tick   tock");
  switch( Read_Keypad() ){

    case 1:
        {
          if(R <= 247) {
          SETRGB((R+8) , G, B); }
          else {
          SETRGB((R=255) , G, B); }
        } break;

    case 5:
        {
          if(R >= 8) {
          SETRGB((R-8) , G, B); }
          else {
          SETRGB((R=0) , G, B); }
        } break;

    case 2:
        {
          if(G <= 247) {
          SETRGB(R , (G+8), B); }
        } break;

    case 6:
        {
          if(G >= 8) {
          SETRGB(R, (G-8), B); }
        } break;

    case 3:
        {
          if(B <= 247) {
          SETRGB(R , G, (B+8)); }
        } break;

    case 7:
        {
          if(B >= 8) {
          SETRGB(R , G, (B-8)); }
        } break;

    case 15:
        {
          SETRGB(255 ,255, 255);
        } break;

            case 16:
        {
          SETRGB(0 , 0, 0);
        } break;

  }}

  pubclient.loop();

}

void SETRGB(int r, int g, int b)	// sets the LED lights
{
if(r == 0 && g == 0 && b ==0 ){
  digitalWrite(ENABLE, HIGH);
}
  else
    { digitalWrite(ENABLE,LOW); }

    analogWrite(RED,r);
    analogWrite(GREEN,g);
    analogWrite(BLUE,b);

    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

  R = r;
  G = g;
  B = b;

}



/* Read the state of the keypad */
byte Read_Keypad(void)
{
  byte Count;
  byte Key_State = 0;

  /* Pulse the clock pin 16 times (one for each key of the keypad)
     and read the state of the data pin on each pulse */
  for(Count = 1; Count <= 16; Count++)
  {
    digitalWrite(SCL_PIN, LOW);

    /* If the data pin is low (active low mode) then store the
       current key number */
    if (!digitalRead(SDO_PIN))
     { Key_State = Count;
        Serial.write(Key_State);
     }
           digitalWrite(SCL_PIN, HIGH);
     }

  return Key_State;
 //  delay(10);
}

