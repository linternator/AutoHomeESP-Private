based on theis board
https://www.banggood.com/da/LILYGO-TTGO-ESP32-WiFi-+-bluetooth-18650-Battery-Protection-Board-0_96-Inch-OLED-Development-Tool-p-1213497.html?gmcCountry=DK&currency=DKK&createTmp=1&utm_source=googleshopping&utm_medium=cpc_bgs&utm_content=xibei&utm_campaign=xibei-ssc-dk-da-all-0303&cur_warehouse=CN
https://everythingesp.com/ttgo-esp32-oled-16850-running-micropython-4fipscom/



## Requirments

BNO055 by Robert bosch gnbh. version - 1.2.1
( eddit libary to use adress 2, 0x29 )
```C++
#define OLED_ADDR 0x29
#define OLED_SDA  5
#define OLED_SCL  4
```
( Adress = 0x29, SDA = 5, SCL = 18)

ESP8266 and ESP32 OLED driver for SSD1306 displays by ThingPulse. -- verssion 4.1.0
```C++
#define OLED_ADDR 0x3C
#define OLED_SDA  5
#define OLED_SCL  4
```