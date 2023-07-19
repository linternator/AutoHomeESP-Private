//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <AutoHome.h>

int buttonA = 26; // button for ? on 
int buttonB = 25; // butotn for ? of
int buttonC = 13; // button to wake screen ? 

int buttonD = 12; // button to toggle screen ?
int buttonE = 27;
int buttonF = 4;

int deBounce = 85;
long interval = 1000;  // interval at which to blink (milliseconds)

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

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
      lcd.clear();
      lcd.setCursor(0, 0);         // move cursor to   (0, 0)
      lcd.print( packet );        // print message at (0, 0) 
      // fix warp around with diffrent lines :/
      

  }

}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
  
   Wire.begin(23, 22);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
   lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
   lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");
  delay(500);


  pinMode(buttonA,INPUT_PULLUP);
  pinMode(buttonB,INPUT_PULLUP);
  pinMode(buttonC,INPUT_PULLUP);
  
  pinMode(buttonD,INPUT_PULLUP);
  pinMode(buttonE,INPUT_PULLUP);
  pinMode(buttonF,INPUT_PULLUP);
  

}

void detectButtons(void)
{
  lcd.setCursor(2,3);
  
  if( !digitalRead(buttonA)  )
    {
      lcd.print("Button A");  // debuging
    }
       
if( digitalRead(buttonB) == LOW)
    {       // send MQTT message
       String packetOUT = "ButtonB";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(buttonB) == 0)
                {
                  delay(deBounce);
                }
    }
      

if( digitalRead(buttonC) == LOW)
    {       // send MQTT message
       String packetOUT = "ButtonC";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(buttonC) == 0)
                {
                  delay(deBounce);
                }
    }

if( digitalRead(buttonD) == LOW)
    {       // send MQTT message
       String packetOUT = "ButtonD";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(buttonD) == 0)
                {
                  delay(deBounce);
                }
    }
      

if( digitalRead(buttonE) == LOW)
    {       // send MQTT message
       String packetOUT = "ButtonE";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(buttonE) == 0)
                {
                  delay(deBounce);
                }
    }
      

if( digitalRead(buttonF) == LOW)
    {       // send MQTT message
       String packetOUT = "ButtonF";
       autohome.sendPacket( packetOUT.c_str() );
       Serial.println(packetOUT);
             while( digitalRead(buttonF) == 0)
                {
                  delay(deBounce);
                }
    }
     // else
        {
      //    lcd.setCursor(2,3);
     //     lcd.print("           ");
        }


}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();
  unsigned long currentMillis = millis();

  // turn of backlight ? 

  // send buttons



if( digitalRead(buttonA) == 0)
  {
    interval = currentMillis + 1000; // set timer for light

                // light on
            lcd.backlight();
  }


  if (currentMillis == interval) 
      {
          // light off
          lcd.noBacklight();
      } 

detectButtons();

}
