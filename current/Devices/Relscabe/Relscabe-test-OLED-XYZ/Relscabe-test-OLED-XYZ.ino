#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <WiFi.h>
#include <SSD1306.h>
#include "OLEDDisplayUi.h"
//#include "images.h"

#define OLED_ADDR 0x3C
#define OLED_SDA  5
#define OLED_SCL  4

#define I2C_SDA 19
#define I2C_SCL 18

float ex = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

// SSD1306 display(OLED_ADDR, OLED_SDA, OLED_SCL);
//OLEDDisplayUi ui     ( &display );

#define BNO055_SAMPLERATE_DELAY_MS (100)
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);  // VIBRATE
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

    //    WiFi.mode(WIFI_STA);
//    WiFi.disconnect();
//    delay(100);
//    display.init();
//    display.setTextAlignment(TEXT_ALIGN_LEFT);
//    display.setFont(ArialMT_Plain_16);
//    display.drawXbm(34, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
//    display.drawString(34, 40, "LION");
//    display.display();


  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
   // while(1);
  }

    /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
  digitalWrite(12,LOW);  // VIBRATE
    
  delay(100); // 1000
  
  /* Display some basic information on this sensor */
  displaySensorDetails();


}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */

  /* The processing sketch expects data as roll, pitch, heading */
//  Serial.print(F("Orientation: "));
//  Serial.print((float)event.orientation.x);
ex = (float)event.orientation.z;
  Serial.println(ex);
//  Serial.print((float)event.orientation.y);
//  Serial.print(F(" "));
//  Serial.println((float)event.orientation.z);
//  Serial.println(F(""));

  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  delay(BNO055_SAMPLERATE_DELAY_MS);

//   display.setFont(ArialMT_Plain_10);
 //   display.setTextAlignment(TEXT_ALIGN_LEFT);
 //   display.display();
    // WiFi.scanNetworks will return the number of networks found
  //  int n = WiFi.scanNetworks();

//       display.clear();
        //display.display();
    //    display.drawString(0, 0, (float)event.orientation.z));
    //    display.drawString(0, 0, (String)ex);
    //    display.drawString(0, 10, (String)event.orientation.y);
    //    display.drawString(0, 20, (String)event.orientation.z);
        
  
  //      display.drawStringMaxWidth(0, iadj * 10, 100, WiFi.SSID(n));
 //       display.drawString(110, iadj * 10, (String)WiFi.RSSI(n));


//    display.init();
//    display.setTextAlignment(TEXT_ALIGN_LEFT);
//    display.setFont(ArialMT_Plain_16);
//    display.drawXbm(34, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
//    display.drawString(34, 40, (String)ex);
//    display.display();

    
//        display.display();    
}
