// PCA9685-Arduino Simple Example
#include "PCA9685.h"

int consilations[] = { pegisus, trio, ... , };      // list of star consitalitons.

int pegisus[] = {3,5,6,7,8,9,10};                   // star constilation LEDs
int trio[]    = {1,2,4};                            // star constaltion LEDs

// probably need an array of every LED for twinkling.


PCA9685 pwmController;                  // Library using default B000000 (A5-A0) i2c address, and default Wire @400kHz

void SetConstilation(int consilations, int brighness)       // set a constilation to a certain brighness.
{
  for( int i = 0, i ++, i == lenght( consilations[] ) )
    {
      pwmController.setChannelPWM(brighness , consilations[0]);
    }    
}

void Twinkle(int MAXbrighness )       // pick a random LED to increase the brighness temporeorly, then dim back to what it was.
{                                     // call fucntion periodicaly in main. with a max brighness for the twinkle.
  // pick rnadom LED.
  int Rando = randome(0, MAXLEDs);

  // read random LED.
int previouseLED = pwmController.getChannelPWM(Rando);

  // set randome led Twinkle brighness
pwmController.setChannelPWM(Rando, MAXbrighness);

  // delay ? 
  
  // set random LED previouse value
  pwmController.setChannelPWM(Rando, previouseLED);
}

// flowing brighness ? 
void flowing_waves(void)
{
//  pos // 
// set led 'pos'     to brighness 'pos'
// set led 'pos + 1' to brighness 'pos + 1'
// set led 'pos + 2' to brighness 'pos + 2'
// set led 'pos + 3' to brighness 'pos + 3'
// pos ++
}

void setup() {
    Serial.begin(115200);               // Begin Serial and Wire interfaces
    Wire.begin(0,2);

    pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line

    pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer

    pwmController.setPWMFrequency(100); // Set PWM freq to 100Hz (default is 200Hz, supports 24Hz to 1526Hz)

    pwmController.setChannelPWM(0, 128 << 4); // Set PWM to 128/255, shifted into 4096-land

    Serial.println(pwmController.getChannelPWM(0)); // Should output 2048, which is 128 << 4
}


void loop() {


  for (int i = 0; i <= 4096; i++) {
   
    for (int v = 0; v <= 15; v++)
      {
          pwmController.setChannelPWM(v, i);
      }   

  
    delay(100);
  }

}
