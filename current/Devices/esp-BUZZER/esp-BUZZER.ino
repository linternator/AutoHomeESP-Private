#include <AutoHome.h>

AutoHome autohome;

#define buzz 0  // the full buzzing tune
#define beep 2  // just a little beep

#define aa 13 // setting switches
#define bb 12
#define cc 14
#define dd 16
#define ee 4

char m0de = "a";  // mode selelct
int buz = 0;     // 0 = off// 1 = buzz// 2 = beep//
int watier = 0; // time its buzzing for


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
  digitalWrite(aa, HIGH);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);
  digitalWrite(buzz,HIGH);
  digitalWrite(beep,HIGH);
  autohome.setPacketHandler(mqtt_callback); // needed here ? 
  autohome.begin();
  Serial.println("setup done");
}

void rando()  // makes a cool noise but not realy that usefull
{
  delay(500);
  Serial.println("blep");
  digitalWrite(aa, LOW);
  digitalWrite(bb, HIGH);
  digitalWrite(cc, HIGH);
  digitalWrite(dd, HIGH);
  digitalWrite(ee, HIGH);//
  digitalWrite(buzz,HIGH);
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
  digitalWrite(beep,HIGH);
      delay(500);
  Serial.println("blep6");
  digitalWrite(aa, LOW);
  digitalWrite(bb, LOW);
  digitalWrite(cc, LOW);
  digitalWrite(dd, LOW);
  digitalWrite(ee, LOW);

  digitalWrite(buzz,LOW);
  digitalWrite(beep,LOW);
}



void M0DE()
{
  switch (m0de)
    {
      case : "a"
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);digitalWrite(ee, HIGH);
        }   break;

      case : "b"
        {
            digitalWrite(aa, LOW);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);  digitalWrite(ee, HIGH);
        }   break;

      case : "c"
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, LOW);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);  digitalWrite(ee, HIGH);
        }   break;

      case : "d"
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, LOW);  digitalWrite(dd, HIGH);  digitalWrite(ee, HIGH);
        }   break;

      case : "e"
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, LOW);  digitalWrite(ee, HIGH);
        }   break;

      case : "f"
        {
            digitalWrite(aa, HIGH);  digitalWrite(bb, HIGH);  digitalWrite(cc, HIGH);  digitalWrite(dd, HIGH);  digitalWrite(ee, LOW);
        }   break;

    }
}

void BUZZ() 
{
  M0DE();
   switch(buz)
   {
      case : '0'
        {
          digitalWrite(buzz,LOW);
          digitalWrite(beep,LOW);
        } break;

       case : '1'
        {
          digitalWrite(buzz,HIGH);
          digitalWrite(beep,LOW);
        } break;

       case : '2'
        {
          digitalWrite(buzz,LOW);
          digitalWrite(beep,HIGH);
        } break; 
   }
}



/* Gets called when get message from any topic*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  String packet = "";

  for (int i = 0; i < length; i++) 
    {
      packet = packet + (char)payload[i];
    }
              if (getValue(packet, ':', 0).equals("BUZZ")) 
              { // example // BUZZ:a:200    -- log wired buzz
                  m0de = (getValue(packet, ':', 1));    // the mode the buzzer is in.
                  watier = (getValue(packet, ':', 2));  // the how long the beep lasts. 
            
                 String packet = "RGB:IS:" + String(currR) + "," + String(currG) + "," + String(currB);
                 autohome.sendPacket( packet.c_str() );
              }


          if (getValue(packet, ':', 0).equals("BEEP"))
              { 
                  m0de = "a";  // mode selelct
                  buzz = 2; // 0 = off// 1 = buzz// 2 = beep//
                  watier = (getValue(packet, ':', 2));  // the how long the beep lasts.
              }
String packet = "BUZZER:IS:" + String(m0de) + "," + String(buzz) + "," + String(watier);
autohome.sendPacket( packet.c_str() );
}




void loop() {
  // put your main code here, to run repeatedly:
unsigned long currentMillis = millis();

autohome.loop();





// recive MQTT comand 
// example // BUZZ:a:200    -- log wired buzz
// example // BEEP:f:1      -- short beep

 //   a - z   = buzz combinations 
 //   0-999   = time in seconds? 


// if mills == (current timed ones ?) * 1,000  ? 
// {
// derive MODE 
// derive buzz
// void MODE();
// void BUZZ();
// }

}
