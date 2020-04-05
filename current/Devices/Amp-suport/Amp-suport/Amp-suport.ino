#include <Wire.h>

int Lbig = 0x00;          // 10110XXX
int Lsmall = 0x00;        // 1010XXXX

int Rbig = 0x00;          // 00110XXX 
int Rsmall =0x00;         // 0010XXXX

int volTOTbig = 100;      // 11100XXX
int volTOTsmall = 100;    // 1101XXXX

int Volume = 0;
int PrevVol = 0;

int LVolume = 0;
int PrevLVol = 0;

int RVolume = 0;
int PrevRVol = 0;

int wait = 50;  // dealy beteween chaning volume // consider moving to main loop

// input A vol
// input B vol
// input C vol

// add system for R and L indapencance. 
// finish instant vol change. 
// intregreat MQTT
// intergreat MQTT feed back.

void setup()
{
  Wire.begin(0,2);
  void Mute(true);  // start muted, to avoid pops and noises on start up.

  Serial.begin(115200);
  Serial.println("\nI2C Scanner");

}

void WriteVolNOW(char type)
{
  int MajorBYTE = 0b00000000;
  int MinorBYTE = 0b00000000;
  int MajorCMD = 0;
  int MinorCMD = 0;

  PrevVol = Volume; // set desired volume as current, then write it.

   switch (type)   // Major Volume.
    {
      case 'T':
        {
          MajorCMD = 0b11100000; // '11100XXX'
        } break;
      case 'R':
        {
          MajorCMD = 0b00110000; // 00110XXX
        } break;
      case 'L':
        {
           MajorCMD = 0b10110000; // 10110XXX
        } break;
    }

   switch (type)   // minor Volume.
    {
      case 'T':
        {
           MinorCMD = 0b11010000; // 11100XXX  // set mode as big vol not small vol
        } break;
      case 'R':
        {
           MinorCMD = 0b00110000; // 00110XXX
        } break;
      case 'L':
        {
            MinorCMD = 0b10110000; // 10110XXX' + MinorBYTE;
        } break;
    }

  Wire.beginTransmission(0x44);       // adress of device
  Wire.write(MinorCMD | (0b00001111 & PrevVol%10)); // write minor volume
  Wire.write(MajorCMD | (0b00000111 & PrevVol/10)); // write major volume
  Wire.endTransmission();

  // add thing for doing this depending on type
}

void Mute(bool mute)
{
   Wire.beginTransmission(0x44);
  //  if(mute == true)
  //   {
  //      Wire.write(0b01111001 );
  //   }
  //     else
  //     {
  //       wire.write(0b01111000  );
  //     }
      
  Wire.write( (0b011110001 & mute));    // one line ? change last bit.
  Wire.endTransmission();

}

void WriteVol(char type)
{

int MajorBYTE = 0b00000000;
int MinorBYTE = 0b00000000;
int MajorCMD = 0;
int MinorCMD = 0; 

  switch (type)   // Major Volume.
    {
      case 'T':
        {
          MajorCMD = 0b11100000; // '11100XXX'
        } break;
      case 'R':
        {
          MajorCMD = 0b00110000; // 00110XXX
        } break;
      case 'L':
        {
           MajorCMD = 0b10110000; // 10110XXX
        } break;
    }

   switch (type)   // minor Volume.
    {
      case 'T':
        {
           MinorCMD = 0b11010000; // 11100XXX  // set mode as big vol not small vol
        } break;
      case 'R':
        {
           MinorCMD = 0b00110000; // 00110XXX
        } break;
      case 'L':
        {
            MinorCMD = 0b10110000; // 10110XXX' + MinorBYTE;
        } break;
    }

  Wire.beginTransmission(0x44);       // adress of device
  Wire.write(MinorCMD | (0b00001111 & PrevVol%10)); // write minor volume
  Wire.write(MajorCMD | (0b00000111 & PrevVol/10)); // write major volume
  Wire.endTransmission();

////////////// Volume
  if(PrevVol > Volume)   // if PrevRvol greater than volume.
    {
      PrevVol --;
    }
    
  if(PrevVol < Volume)   // if PrevRvol lessthan than volume.
    {
      PrevVol ++;
    }

//////////// L
  if(PrevLVol > LVolume)   // if PrevRvol greater than volume.
    {
      PrevLVol --;
    }
    
  if(PrevLVol < LVolume)   // if PrevRvol lessthan than volume.
    {
      PrevLVol ++;
    }

//////////// R
  if(PrevRVol > RVolume)   // if PrevRvol greater than volume.
    {
      RVolume --;
    }
    
  if(PrevRVol < RVolume)   // if PrevRvol lessthan than volume.
    {
      RVolume ++;
    }
}

void loop()
{


if (Volume != PrevVol)
  {
    WriteVol('T');  // edit total volume
  }

if (LVolume != PrevLVol)
  {
    WriteVol('L');
  }

if (RVolume != PrevLVol)
  {
    WriteVol('R');
  }

// get number form serial. set to Volume.
char invol;
//invol = Serial.read();
// Volume = Serial.read();

if (Serial.available() > 0) {

      Serial.print("Value:");
      int vol = Serial.parseInt();
      Serial.println(vol);
      Serial.read();
      //Volume = vol;

//      h vol       atenuation dB
 Volume = map(vol, 0, 100, 79, 0);

 // make volume into exponant. 
  }

Mute(false); // do not mute


// map(value, fromLow, fromHigh, toLow, toHigh)  // turn 0-100 volume. to -79dB to -0dB
  delay(wait);  // wait for i2c to send, and volume to move
Serial.print("current = ");
Serial.print(PrevVol );
Serial.print(" | ");
Serial.print("Goal = ");
Serial.println(Volume);
}
