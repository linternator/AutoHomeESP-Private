int Red = 13; //3;  
int Green = 12;  
int Blue = 14; //1;  
int enable = 2; // 5; // enable for PSU  

// int ENable = 23;  // enble / power for buttonz
#define SCL_PIN 5 // 7 // switch
#define SDO_PIN 4 // 8  // switch

int R = 128;  
int G = 128;
int B = 128;
 
    int PiN = 0;
  
    int Delly = 0;
    int Speed = 1;
  
  int Value  = 0;
  int RValue = 0;
  int GValue = 0;
  int BValue = 0;
  
  int mode = 0;
  int modeold = 0; // for state change detection.
  
  


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  
  
 pinMode(Red, OUTPUT);
 pinMode(Green, OUTPUT);
 pinMode(Blue, OUTPUT);
 
 pinMode(enable, OUTPUT);
 
/// pinMode(ENable, OUTPUT);


  Serial.begin(9600);
 
// digitalWrite(ENable,HIGH);  // initaly PSU is off
 
 startup(); // turns on psu and flashed all LEDs twice to show its working

// set up interupts  

//set up I2C buttons
//may need more verables for them
}


void loop() 
{
  
if( Read_Keypad() == 3) 
{
  Serial.write("3");
  digitalWrite(enable,HIGH);
}

if( Read_Keypad() == 1)
{
  Serial.write("1");
  White();
}

if( Read_Keypad() == 14)
{
  Serial.write("14");
  blu3();
}

else
{
  Serial.write("normal");
  White();
  delay(2000);
  r3d();
  delay(2000);
  blu3();
  delay(2000);
  gr33n();
  delay(2000);
}

}



void startup()   // flash twise to show works
{
        digitalWrite(enable,LOW); 
        analogWrite( Blue, 255 ); 
        delay(250);
        analogWrite( Blue, 0 ); 
        delay(250);
        analogWrite( Blue, 255 ); 
        delay(250);
        analogWrite( Blue, 0 ); 
        digitalWrite(enable,HIGH);
        Serial.println("started");
}



 

void White()
{
  digitalWrite(enable,LOW);
  analogWrite( Blue, 255 ); 
  analogWrite( Red, 255 ); 
  analogWrite( Green, 255 ); 
}

void r3d() {
  digitalWrite(enable,LOW);
  analogWrite( Blue, 0 ); 
  analogWrite( Red, 255 ); 
  analogWrite( Green, 0 ); 
}

void gr33n() {
  digitalWrite(enable,LOW);
  analogWrite( Blue, 0 ); 
  analogWrite( Red, 0 ); 
  analogWrite( Green, 255 ); 
}

void blu3() {
  digitalWrite(enable,LOW);
  analogWrite( Blue, 255 ); 
  analogWrite( Red, 0 ); 
  analogWrite( Green, 0 ); 
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
      Key_State = Count;
   
    digitalWrite(SCL_PIN, HIGH);
  } 
 
  return Key_State;
}
