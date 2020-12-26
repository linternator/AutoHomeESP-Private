#include <AutoHome.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "BNO055_support.h"

AutoHome autohome;

// sensor
#define bno055_boot_time 250 // Found by trial and error
struct bno055_t myBNO;
struct bno055_euler myEulerData; //Structure to hold the Euler data

// Screen
#define OLED_ADDR 0x3C
#define OLED_SDA 5
#define OLED_SCL 4
SSD1306Wire display(OLED_ADDR, OLED_SDA, OLED_SCL);


unsigned long lastTime = 0;
unsigned long lastTime_serial = 0;

int Xold;
int Yold;
int Zold;

    bool isWithinValidRange = false;
    int mqttPitch   = -1;
    int mqttRole    = -1;
    int mqttHeading = -1;

String me33age = "";  // mqtt mesages inbound

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


/* This function will be called every time a packet is received from the mqtt topic. */
/* This is registered in the setup() */
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  if (!autohome.mqtt_callback(topic, payload, length)) {

    String packet = "";

    for (int i = 0; i < length; i++) {
      packet = packet + (char)payload[i];
    }

    Serial.print(packet);
    me33age = packet;

  }

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
 //   display.drawString(5, 0, "mqtt: " + String( me33age ));
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


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  /* This registers the function that gets called when a packet is recieved. */
  autohome.setPacketHandler(mqtt_callback);

  /* This starts the library and connects the esp to the wifi and the mqtt broker */
  autohome.begin();
  setup_screen();
  setup_bno055();

}

void loop() {

  /* This needs to be called in the loop as it handels the reconection to the mqtt server if it disconnects*/
  autohome.loop();
  if ((millis() - lastTime) >= 16)
  {
    lastTime = millis();
    read_sensor();
    update_screen();
  }

  if ((millis() - lastTime_serial) >= 25)
  {
    lastTime_serial = millis();
 //   write_sensor_to_serial();
 
    float rawHeading = float(myEulerData.h) / 16.00;
    int heading = round(rawHeading);  // 3

    float rawRole = float(myEulerData.r) / 16.00;
    int role = round(rawRole);  // 3

    float rawPitch = float(myEulerData.p) / 16.00;
    int pitch = round(rawPitch);  

    


    
    if (pitch < 90 && pitch > 45) // between 90 and 45 // twisty
      {
        mqttPitch = 0;
        isWithinValidRange = true;
      }
        else if (pitch > 90)
        {
          mqttPitch = pitch - 90;
          mqttPitch = map(mqttPitch, 0, 180, 0, 255);
          isWithinValidRange = true;
        }
          else if (pitch < -90)
          {
            mqttPitch = pitch + 270;
            mqttPitch = map(mqttPitch, 90, 180, 128, 255);
            isWithinValidRange = true;
          }
            else if (pitch > -90 && pitch < -45)
            {
              mqttPitch = 255;
              isWithinValidRange = true;
            }



//     mqttRole    = role;       // betweeen 0 and 42 // up / dow
  if (role > -50 && role < 0)
        {
          mqttRole = role* -1;

            mqttRole = map(mqttRole, -50, 0, 0, 255);
        }

//     mqttHeading = heading;   // between 20 and 102 // left / right
  if (heading > 20 && heading < 105)
        {
          //mqttHeading = -1;

            mqttHeading = map(heading, 42, 105, 0, 255);
        }




    // send auto home packet
    if (mqttPitch != Zold && isWithinValidRange)    // twist
    {
      String packet = "Z:IS:" + String(mqttPitch);
      autohome.sendPacket( packet.c_str() );
    }

        if (mqttRole != Yold)
    {
      String packet = "Y:IS:" + String(mqttRole);
      autohome.sendPacket( packet.c_str() );
    }

        if (mqttHeading != Xold)
    {
      String packet = "X:IS:" + String(mqttHeading);
      autohome.sendPacket( packet.c_str() );
    }

    Zold = mqttPitch;
    Yold = mqttRole;
    Xold = mqttHeading;
  }

}
