/** This sketch tests both the sensor and screen.
 * It will write the sensor data to the screen and serial.*/

#include <Wire.h>
#include "SSD1306Wire.h"
#include "BNO055_support.h"

// Sensor
//This structure contains the details of the BNO055 device that is connected. (Updated after initialization)
#define bno055_boot_time 250 // Found by trial and error
struct bno055_t myBNO;
struct bno055_euler myEulerData; //Structure to hold the Euler data

// Screen
#define OLED_ADDR 0x3C
#define OLED_SDA 5
#define OLED_SCL 4
SSD1306Wire display(OLED_ADDR, OLED_SDA, OLED_SCL);
#include <Wire.h>
 
//void setup() {
//  Wire.begin(5,4);
//  Serial.begin(115200);
//  Serial.println("\nI2C Scanner");
//}
 
//void loop() {
//  byte error, address;
//  int nDevices;
//  Serial.println("Scanning...");
//  nDevices = 0;
//  for(address = 1; address < 127; address++ ) {
//    Wire.beginTransmission(address);
//    error = Wire.endTransmission();
//    if (error == 0) {
//      Serial.print("I2C device found at address 0x");
//      if (address<16) {
//        Serial.print("0");
//      }
//      Serial.println(address,HEX);
//      nDevices++;
//    }
//    else if (error==4) {
//      Serial.print("Unknow error at address 0x");
//      if (address<16) {
//        Serial.print("0");
//      }
//      Serial.println(address,HEX);
//    }    

unsigned long lastTime = 0;
unsigned long lastTime_serial = 0;

void setup_bno055()
{
    Serial.println("Init the sensor...");

    // Ensures the sensor have had time to boot
    unsigned long timeSinceStartMs = millis();
    if (timeSinceStartMs < bno055_boot_time)
    {
        // Sensor needs a little time to start before it can be setup
        Serial.println("Waits " + String(bno055_boot_time - timeSinceStartMs) + "ms for the sensor to boot....");
        delay(bno055_boot_time - timeSinceStartMs);
    }

    //Initialize I2C communication
    Wire.begin(5, 4);

    //Initialization of the BNO055
    BNO_Init(&myBNO); //Assigning the structure to hold information about the device

    //Configuration to NDoF mode
    bno055_set_operation_mode(OPERATION_MODE_NDOF);

    delay(1);

    Serial.println("Sensor have been initialized");
}

void setup_screen()
{
    Serial.println("Init the screen...");
    // Initialising the UI will init the display too.
    display.init();

    //display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);

    Serial.println("Screen have been initialized");
}

void setup() //This code is executed once
{
    //Initialize the Serial Port to view information on the Serial Monitor
    Serial.begin(115200);
    setup_screen();
    setup_bno055();
    
}

void update_screen()
{
    // clear the display
    display.clear();

    //Convert to degrees
    float heading = float(myEulerData.h) / 16.00; //yaw
    float roll = float(myEulerData.r) / 16.00;
    float pitch = float(myEulerData.p) / 16.00;

    // Draws on the display
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(5, 0, "Time Stamp: " + String(millis()));
    display.drawString(5, 15, "Heading(Yaw): " + String(heading));
    display.drawString(5, 30, "Roll: " + String(roll));
    display.drawString(5, 45, "Pitch: " + String(pitch));

    // write the buffer to the display
    display.display();
}

void read_sensor()
{
    //Update Euler data into the structure
    bno055_read_euler_hrp(&myEulerData);
}

void write_sensor_to_serial()
{
    Serial.print("Time Stamp: ");
    Serial.println(lastTime);

    Serial.print("Heading(Yaw): ");               //To read out the Heading (Yaw)
    Serial.println(float(myEulerData.h) / 16.00); //Convert to degrees

    Serial.print("Roll: ");                       //To read out the Roll
    Serial.println(float(myEulerData.r) / 16.00); //Convert to degrees

    Serial.print("Pitch: ");                      //To read out the Pitch
    Serial.println(float(myEulerData.p) / 16.00); //Convert to degrees

    Serial.println(); //Extra line to differentiate between packets
}

void loop()
{
    if ((millis() - lastTime) >= 16)
    {
        lastTime = millis();
        read_sensor();
        update_screen();
    }

    if ((millis() - lastTime_serial) >= 200)
    {
        lastTime_serial = millis();
        write_sensor_to_serial();


//  byte error, address;
//  int nDevices;
//  Serial.println("Scanning...");
//  nDevices = 0;
//  for(address = 1; address < 127; address++ ) {
//    Wire.beginTransmission(address);
//    error = Wire.endTransmission();
//    if (error == 0) {
//      Serial.print("I2C device found at address 0x");
//      if (address<16) {
//        Serial.print("0");
//      }
//      Serial.println(address,HEX);
//      nDevices++;
//    }
//    else if (error==4) {
//      Serial.print("Unknow error at address 0x");
//      if (address<16) {
//        Serial.print("0");
//      }
//      Serial.println(address,HEX);


        
    }
}
