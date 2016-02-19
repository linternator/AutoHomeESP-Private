#include <AutoHome.h>

AutoHome autohome;

/* These variables need to be filled out with the information for your wifi details */
/* and the mqtt broker */
char const* wifi_ssid = "SoMakeIt"; // "RG-Wifi";
char const* wifi_password = "JeanLucPicard"; // "STBCrg123";
char const* mqtt_broker_ip =  "http://82.13.163.212"; // "192.168.1.10";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "keypad";
char const* mqtt_topic = "/home/bedroom/keypad";


#define SCL_PIN 12
#define SDO_PIN 13

unsigned long previousMillisKeypad = 0;
unsigned long previousMillisFade = 0;
int fadeTime = 0;

int lastButton = 0;

boolean fade = false;
boolean buttonPress = false;

void SETRGB(int r, int g, int b);
byte Read_Keypad(void);
void process_fade(long currentMillis);
void processKeypad();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup() {
  
  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);

  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);

  analogWriteRange(255);

}

void loop() {
 
  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();

 // unsigned long currentMillis = millis();

//  process_fade(currentMillis);

//  if (currentMillis - previousMillisKeypad >= 150) {

 //   previousMillisKeypad = currentMillis;

    if (Read_Keypad() == 0 ) {

      if (buttonPress) {

        buttonPress = false;

 if(lastButton != 0)
      {
//          autohome.sendPacket( packet.c_str() );
                    lastButton = 0;
      }
        else  
          {lastButton = 0;}

      }

    } else {

      buttonPress = true;

      processKeypad();

    }

  // }

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
    { Key_State = Count;
      Serial.write(Key_State);
    }
    digitalWrite(SCL_PIN, HIGH);
  }

  return Key_State;
  //  delay(10);
}

void processKeypad() {

     
      //  lastButton = Read_Keypad() 
        if(lastButton != Read_Keypad()){
          autohome.sendPacket("Bedroom:"+Read_Keypad());
        }
        lastButton = Read_Keypad();
      }   
      



 /* switch ( Read_Keypad() ) {

//    case 0:
//    {
//      if(lastButton != 0)
//      {
//          String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
//          autohome.sendPacket( packet.c_str() );
//                    lastButton = 0;
//      }
//        else  
//          {lastButton = 0;}
//        break;
//    }

    case 1: {

{
        if(lastButton != 1){
          autohome.sendPacket("RGB:BUTTON:1");
        }
        lastButton = 1;
        break;
      }
      }

    case 5: {
        lastButton = 5;
{
        if(lastButton != 5){
          autohome.sendPacket("RGB:BUTTON:5");
        }
        lastButton = 5;
        break;        
      } 
      }

    case 2: {       
        lastButton = 2;		
{       
        if(lastButton != 2){       
          autohome.sendPacket("RGB:BUTTON:2");
        }
        lastButton = 2;
        break;        
      } 
      }

    case 6: {      
        lastButton = 6;	
	{      
        if(lastButton != 6){       
          autohome.sendPacket("RGB:BUTTON:6");
        }
        lastButton = 6;
        break;    
      }      
      }

    case 3: {
        lastButton = 3;
      {
        if(lastButton != 3){
          autohome.sendPacket("RGB:BUTTON:3");
        }
        lastButton = 3;
        break;
      } 
      }

    case 7:
      {
        lastButton = 7 {
        if(lastButton != 7){
          autohome.sendPacket("RGB:BUTTON:7");
        }
        lastButton = 7;
        break;
      }   
      }


    case 4: {
      {
        lastButton = 4 {
        if(lastButton != 4){
          autohome.sendPacket("RGB:BUTTON:4");
        }
        lastButton = 4;
        break;
      }   
      }

    case 8: {
      {
        lastButton = 8 {
        if(lastButton != 8){
          autohome.sendPacket("RGB:BUTTON:8");
        }
        lastButton = 8;
        break;
      }   
      }
      
    case 9:{
      {
        lastButton = 9 {
        if(lastButton != 9){
          autohome.sendPacket("RGB:BUTTON:9");
        }
        lastButton = 9;
        break;
      }   
      }

    case 10:{
      {
        lastButton = 10 {
        if(lastButton != 10){
          autohome.sendPacket("RGB:BUTTON:10");
        }
        lastButton = 10;
        break;
      }   
      } 
      
    case 11:{
      {
        lastButton = 11 {
        if(lastButton != 11){
          autohome.sendPacket("RGB:BUTTON:11");
        }
        lastButton = 11;
        break;
      }   
      }

    case 12:{
      {
        lastButton = 12 {
        if(lastButton != 12){
          autohome.sendPacket("RGB:BUTTON:12");
        }
        lastButton = 12;
        break;
      }   
      }
            

    case 13: {

        lastButton = 13;

          String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
    autohome.sendPacket( packet.c_str() );

        break;

      }

    case 14: {



        break;

      }

    case 15: {

        lastButton = 15;

          String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
    autohome.sendPacket( packet.c_str() );

        break;

      }

    case 16: {

        SETRGB(255 , 255, 255);

        lastButton = 16;

          String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
    autohome.sendPacket( packet.c_str() );

        break;

      }

  

*/  

