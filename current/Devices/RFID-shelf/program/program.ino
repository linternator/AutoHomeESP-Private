// include auto home

int RFID1 = 0;
int RFID2 = 0;
int RFID3 = 0;

int prevRFID1 = 0;
int prevRFID2 = 0;
int prevRFID3 = 0;

int potvar = 0; 


int SC1 = 2;  // chip select for first RFID reader
int SC2 = 4;  // chip select for second RFID reader
int SC3 = 5;  // chip select for third RFID reader
int MISO = 12;    // SPI
int MOSI = 13;
int RST = 9;
int SCL = 14;

int LED = 10; // LED back lighitn of shelf
int POT = 0;  // potentomitor on analog pin

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:


// read RFID pads
// send message

// read pot, 
// set lights


}
