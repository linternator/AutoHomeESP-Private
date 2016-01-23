int Red = 13; //3;  
int Green = 12;  
int Blue = 14; //1;  
int enable = 2; // 5; // enable for PSU 

int R = 128;
int G = 128;
int B = 128;

int STATE;
int array[8]; 

#define SCL_PIN 5 // 7 // switch
#define SDO_PIN 4 // 8  // switch


void setup() {

  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  pinMode(enable, OUTPUT);
  digitalWrite(enable,HIGH);
  
  Serial.begin(6900);

 // Serial.begin(9600);

//output
//pinMode(GND, OUTPUT);
//pinMode(VCC, OUTPUT);



int arr[8];

}

byte Read_Keypad(void)
{
  byte Count;
  byte Key_State = 0;
Serial.println("test");
  /* Pulse the clock pin 16 times (one for each key of the keypad)
     and read the state of the data pin on each pulse */
  for(Count = 1; Count <= 16; Count++)
  {
    digitalWrite(SCL_PIN, LOW);
    delay(50);
   Serial.println(Count);
   delay(50);
    /* If the data pin is low (active low mode) then store the
       current key number */
    if (!digitalRead(SDO_PIN))
      Key_State = Count;
   
    digitalWrite(SCL_PIN, HIGH);
  } 
 
  return Key_State;
}

void loop() 
{
delay(50);
switch(Read_Keypad())
{
  case 0:
  {
    Serial.println("0");
  }break;

  
  case 1:
  {
    Serial.println("1");
  }break;

  
  case 2:
  {
    Serial.println("2");
  }break;

  
  case 3:
  {
    Serial.println("3");
  }break;
}


}
