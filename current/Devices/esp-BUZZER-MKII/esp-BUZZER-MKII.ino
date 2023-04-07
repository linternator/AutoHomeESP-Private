#include <AutoHome.h>

AutoHome autohome;

#define buzz 2  // the full buzzing tune
#define beep 4  // just a little beep

#define aa 13 // setting switches
#define bb 12
#define cc 14
#define dd 16                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
#define ee 5

#define butty 3 // stop button ? ??  

int m0de = 0;  // mode selelct
int buz = 0;     // 0 = off// 1 = buzz// 2 = beep//
int watier = 100; // time its buzzing for

uint8_t Switches[5];

unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);      // debug  
Serial.println("starting settup");

pinMode(aa, OUTPUT);
pinMode(bb, OUTPUT);
pinMode(cc, OUTPUT);
pinMode(dd, OUTPUT);
pinMode(ee, OUTPUT);
pinMode(buzz, OUTPUT);
pinMode(beep, OUTPUT);
pinMode(butty, INPUT_PULLUP);

  digitalWrite(aa, HIGH);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);
  digitalWrite(buzz,HIGH);
  digitalWrite(beep,HIGH);
  digitalWrite(butty,HIGH);
  autohome.setPacketHandler(mqtt_callback); // needed here ? 
  autohome.begin();
  Serial.println("setup done");
}

void rando()  // makes a cool noise but not realy that usefull
{
  delay(100);
  Serial.println("blep");
  digitalWrite(aa, LOW);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);//
  digitalWrite(buzz,LOW);
  digitalWrite(beep,HIGH);
    delay(500);
  Serial.println("blep2");
  digitalWrite(aa, LOW);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);// 
  digitalWrite(buzz,LOW);
  digitalWrite(beep,LOW);
    delay(500);
  Serial.println("blep3");
  digitalWrite(aa, LOW);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, LOW);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, LOW);//
  digitalWrite(buzz,LOW);
  digitalWrite(beep,HIGH);
    delay(500);
  Serial.println("blep4");
  digitalWrite(aa, HIGH);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);//
  digitalWrite(buzz,LOW);
  digitalWrite(beep,HIGH);
    delay(500);
  Serial.println("blep5");
  digitalWrite(aa, HIGH);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);//
  digitalWrite(buzz,HIGH);
  digitalWrite(beep,LOW);
      delay(500);
  Serial.println("blep6");
  digitalWrite(aa, LOW);
  digitalWrite(bb, LOW);
  digitalWrite(cc, LOW);
  digitalWrite(dd, LOW);
  digitalWrite(ee, LOW);

  digitalWrite(buzz,HIGH);
  digitalWrite(beep,HIGH);
}



void M0DE()
{
      // work out a better way of doing this. 
      // maybe sending an array like "00101" and splitting it ? 

  switch (m0de)
    {
      case 0:
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);digitalWrite(ee, HIGH);
        }   break;

      case  1 :
        {
            digitalWrite(aa, LOW);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);  digitalWrite(ee, HIGH);
        }   break;

      case  2 :
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, LOW);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);  digitalWrite(ee, HIGH);
        }   break;

      case  3 :
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, LOW);  digitalWrite(dd, HIGH);  digitalWrite(ee, HIGH);
        }   break;

      case  4 :
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, LOW);  digitalWrite(ee, HIGH);
        }   break;

      case  5 : 
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);  digitalWrite(ee, LOW);
        }   break;




    }
}

void BUZZ(int buz) 
{
 // M0DE();
   switch(buz)
   {
      case 0:
        {
          digitalWrite(buzz,HIGH);
          digitalWrite(beep,HIGH);
         // Serial.println("offing");
        } break;

       case 1 :
        {
          digitalWrite(buzz,HIGH);
          digitalWrite(beep,LOW);
       //   Serial.println("buzzing");
        } break;

       case 2 :
        {
          digitalWrite(buzz,LOW);
          digitalWrite(beep,HIGH);
         // Serial.println("beeping");
        } break; 
   }
}



/* Gets called when get message from any topic*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  if (autohome.mqtt_callback(topic,payload, length) == 0){
  String packet = "";


  for (int i = 0; i < length; i++) 
    {
      packet = packet + (char)payload[i];
    }

              if (autohome.getValue(packet, ':', 0).equals("BUZZ")) 
              { // example // BUZZ:a:200    -- log wired buzz
                 buz = 1;
                 m0de = (autohome.getValue(packet, ':', 1)).toInt();    // the mode the buzzer is in.
                 M0DE();
                 watier = (autohome.getValue(packet, ':', 2)).toInt();  // the how long the beep lasts. 

                  String packetOUT = "BUZZER IS:" + String(m0de) + "," + String(buz) + "," + String(watier);
                  autohome.sendPacket( packetOUT.c_str() );
                  Serial.println(packetOUT);
              }

          if (autohome.getValue(packet, ':', 0).equals("BEEP"))
              { 
                  buz = 2; // 0 = off// 1 = buzz// 2 = beep//
                  
                  m0de = (autohome.getValue(packet, ':', 1)).toInt();    // the mode the buzzer is in.
                  M0DE();
                  watier = (autohome.getValue(packet, ':', 2)).toInt();  // the how long the beep lasts.
                   
                  String packetOUT = "BUZZER IS:" + String(m0de) + "," + String(buz) + "," + String(watier);
                  autohome.sendPacket( packetOUT.c_str() );
                  Serial.println(packetOUT);
              }

if (autohome.getValue(packet, ':', 0).equals("TEST"))
              { 
                  rando();
              } 
              
if (autohome.getValue(packet, ':', 0).equals("STOP"))
              { 
                  BUZZ(0);
                   watier = 1; 
              } 

if (autohome.getValue(packet, ':', 0).equals("stop"))       
              { 
                  BUZZ(0);
                   watier = 1; 
              } 

if (autohome.getValue(packet, ':', 0).equals("status"))     // get status
  {
    String packetOUT = "BUZZER IS:" + String(m0de) + "," + String(buz) + "," + String(watier);
    autohome.sendPacket( packetOUT.c_str() );
    Serial.println(packetOUT);
  }

}}

void loop() {
  // put your main code here, to run repeatedly:

autohome.loop();
// delay(1);

if( digitalRead(butty) == 0 )
  {
    BUZZ(0);
    buz = 0;
    watier = 100;
    m0de = 0;
    M0DE();
    String packetOUT = "Stop button pushed!";
    autohome.sendPacket( packetOUT.c_str() );
    Serial.println("intrupted stopping!");
      while( digitalRead(butty) == 0)
        {
          delay(2);
        }
    
  }


// recive MQTT comand 
// example // BUZZ:0:200    -- log wired buzz
// example // BEEP:3:1      -- short beep

 //   0 - 5   = buzz combinations 
 //   0-99999   = time in ms? 

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= watier) 
  {
    previousMillis = currentMillis;
             
    BUZZ(0);  // stop if times up.
    buz = 0;
    m0de = 0;
    M0DE();
    watier = 100;
    Serial.println("times up!! stopping now :3");

  }
              
   BUZZ(buz);
              
}
