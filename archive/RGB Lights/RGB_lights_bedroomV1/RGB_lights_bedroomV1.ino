int Red = 4;  
int Green = 2;  
int Blue = 3;  
int enable = 22;  

int ENable = 23;  
int one = 48;  
int two = 46;  
int three = 44;  
int four = 42;  
int five = 40;  
int six = 38;  
int seven = 36;  
int eight = 34;  

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
  
  


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  
    

  
  
//  digitalWrite(enable,LOW);
  
   pinMode(Red, OUTPUT);
 pinMode(Green, OUTPUT);
 pinMode(Blue, OUTPUT);
 
 pinMode(enable, OUTPUT);
 
 pinMode(ENable, OUTPUT);
 
pinMode(one, INPUT); 
pinMode(two, INPUT);
pinMode(three, INPUT);
pinMode(four, INPUT);
pinMode(five, INPUT);
pinMode(six, INPUT);
pinMode(seven, INPUT);
pinMode(eight, INPUT);
  Serial.begin(9600);
 
 digitalWrite(ENable,HIGH); 
 
 startup();  
 
}


void loop() {
  
while( (digitalRead(one) == 0) &&  (digitalRead(two) == 0) && (digitalRead(three) == 0) && (digitalRead(four) == 0) && (digitalRead(five) == 0) && (digitalRead(six) == 0) && (digitalRead(seven) == 0) && (digitalRead(eight) == 0))
{
  
  che();
  
  switch(mode)
      {
         
          case 0:
              {
                digitalWrite(enable,HIGH);
                digitalWrite(ENable,HIGH);
                delay(50);
              } break;
             
          case 1:
              {
                White();
              } break;
              
          case 2:
              {
                r3d();
              } break;
              
         case 3:
              {
                gr33n();
              } break;
              
          case 4:
              {
                blu3();
              } break;
              
              case 5:
              {
                Rando();
              } break;
              
           case 6:
              {
                mix();
              } break;
              
              delay(20);      
      }
  
  
che();
  
}}

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
}

void che() {
  
  if ((digitalRead(one) == 1) && (digitalRead(seven) == 1)) // debug, if controle miss reading
{
      digitalWrite(enable,HIGH);
      digitalWrite(ENable,LOW);
      delay(1000);
      digitalWrite(ENable,HIGH);
}
   
if(digitalRead(one) == 1)
      {
          digitalWrite(enable,HIGH);
          mode = 0;
      }
      
if(digitalRead(two) == 1)
      {
        mode = 1;
      }

if(digitalRead(four) == 1)
      {
        mode = 2;
      }
      
if(digitalRead(six) == 1)
      {
        mode = 3;
      }

if(digitalRead(eight) == 1)
      {
        mode = 4;
      }

if(digitalRead(three) == 1)
      {
        mode = 5;
      }
      
if(digitalRead(seven) == 1)
      {
        mode = 6;
      }

 
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
  
  
void Rando() {

digitalWrite(enable,LOW);
  PiN = random(1,4);
  Value = random(0,255);
  Delly = random(10,125);
  Speed = random(1,5);
  
if(PiN == 2)
            {
                if(Value < RValue)
                    {
                      for(int fadeValue = RValue ; fadeValue >= Value; fadeValue -=1) { 
                      analogWrite( Red, fadeValue ); 
                      delay(Delly);
              che();       
                      delay(10);                            
                  } RValue = Value;}
                      else
                            {
                                for(int fadeValue = RValue ; fadeValue <= random(0, 255); fadeValue +=1) { 
                                analogWrite( Red, fadeValue );   
                                delay(Delly);  
                    che();            
                                delay(10);                            
                              }RValue = Value;}
            }
            
            
if(PiN == 3)
            {
                if(Value < GValue)
                  {
                      for(int fadeValue = GValue ; fadeValue >= Value; fadeValue -=1) { 
                      analogWrite( Green, fadeValue ); 
                      delay(Delly); 
       che();               
                      delay(10);                            
                  } GValue = Value; }
                      else
                            {
                                for(int fadeValue = GValue ; fadeValue <= Value; fadeValue +=1) { 
                                analogWrite( Green, fadeValue ); 
                                delay(Delly); 
                 che();               
                                delay(10);                            
                              } GValue = Value; }
            }       
          
if(PiN == 1)
                        {
                if(Value < BValue)
                  {
                      for(int fadeValue = BValue ; fadeValue >= Value; fadeValue -=1) { 
                      analogWrite( Blue, fadeValue ); 
                      delay(Delly); 
       che();               
                      delay(10);                            
                  } BValue = Value; }
                      else
                            {
                                for(int fadeValue = GValue ; fadeValue <= Value; fadeValue +=1) { 
                                analogWrite( Blue, fadeValue ); 
                                delay(Delly);                
                                delay(10);                            
                              } BValue = Value; }
            }
      delay(Delly); 
che();      
  } 
  
  
  
  void mix() {
  Value = random(0,2);
   PiN = random(1,4);
    Delly = random(10,125);
    
  if(PiN == 2)
        {
          if(Value = 0)
              {
                   analogWrite( Red, 0 );
              }
                    else
                          {
                            analogWrite( Red, 255 );
                          }
        }
        
  if(PiN == 1)
        {
          if(Value = 0)
              {
                   analogWrite( Green, 0 );
              }
                    else
                          {
                            analogWrite( Green, 255 );
                          }
        }
        
  if(PiN == 3)
        {
          if(Value = 0)
              {
                   analogWrite( Blue, 0 );
              }
                    else
                          {
                            analogWrite( Blue, 255 );
                          }
        }
        
     //   delay(1000);
        
        
        for(int fadeValue = Delly ; fadeValue >= 0; fadeValue -=1)
             {
               delay(10);
               che();
             }
             
             analogWrite( Blue, 0 );
             analogWrite( Red, 0 );
             analogWrite( Green, 0 );
 
  } 
  
  
  
  
  
  
  
  
