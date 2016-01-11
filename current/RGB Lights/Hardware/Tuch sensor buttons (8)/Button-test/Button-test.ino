#define GND 7 //
#define VCC 6 // 

#define DV 2
#define CK 5 // 3
#define D0 3
#define RST 4 // 5

int array[8];





void setup() {

 Serial.begin(9600);

//output
pinMode(GND, OUTPUT);
pinMode(VCC, OUTPUT);
pinMode(D0, INPUT);   // data in
pinMode(RST, OUTPUT);
pinMode(CK, OUTPUT);
pinMode(DV, INPUT); // inturput ? 

digitalWrite(GND,LOW);
digitalWrite(VCC,HIGH);

int arr[8];

}


void snook()
{

//   digitalWrite(DV,HIGH);
//  delay(1);
//  digitalWrite(DV,LOW);

  
  digitalWrite(RST,HIGH);
  delay(1);
 // Serial.println( " ");
  digitalWrite(RST,LOW);
  delay(1);
  
  digitalWrite(CK,HIGH);
  delay(1);// button 1
  array[0] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);
  delay(1);// button 2
  array[1] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);            // button 3 
  delay(1);
  array[2] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);            // button 4
  delay(1);
  array[3] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);            // button 5
  delay(1);
  array[4] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);            // button 6
  delay(1);
  array[5] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);            // button 7 
  delay(1);
  array[6] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
  digitalWrite(CK,HIGH);            // button 8 
  delay(1);
  array[7] = digitalRead(D0); 
  digitalWrite(CK,LOW);
  delay(1);
}



void loop() {

 if(digitalRead(DV) == 1 )
{
//Serial.println( digitalRead(DV) );  
 snook();
  
 for(int x = 0; x < 8; x++)
  {
    Serial.print(array[x]);
  }
Serial.println( " ");
//Serial.println( digitalRead(DV) );
//Serial.println("   " );
  
  // put your main code here, to run repeatedly:
}
}

 //snook(&arr);


