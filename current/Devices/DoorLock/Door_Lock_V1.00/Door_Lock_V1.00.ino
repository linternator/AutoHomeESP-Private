#include <Servo.h>
#include <AutoHome.h>

AutoHome autohome;
Servo myservo;  // create servo object to control a servo

char const* wifi_ssid = "RG-Wifi"; // "RG-Wifi";
char const* wifi_password = "STBCrg123"; // "STBCrg123";
char const* mqtt_broker_ip =  "192.168.1.10"; // "192.168.1.10";
char const* mqtt_user = "autohome";
char const* mqtt_password = "autoHOME";
char const* host_name = "keypad";
char const* mqtt_topic = "/home/bedroom/keypad";

                // a maximum of eight servo objects can be created
int pos = 0;    // variable to store the servo position

// sort

#define SCL_PIN 4
#define SDO_PIN 5

unsigned long previousMillisKeypad = 0;
// unsigned long previousMillisFade = 0;
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
  // put your setup code here, to run once:
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object


  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin(wifi_ssid, wifi_password, mqtt_broker_ip, mqtt_user, mqtt_password, host_name, mqtt_topic);

  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);
// include pins for othere keypad
  analogWriteRange(255);


}

void loop() {
  // put your main code here, to run repeatedly:
 autohome.loop();
}


// make this into a subrouteen for locking and unlocking the door.
   for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }







// adapt key pad subroutean for to read both keypads at once.
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
