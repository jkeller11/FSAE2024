#include <Adafruit_MCP2515.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "functions.h"

//RFM95 Setup Parameters
#define RFM95_CS 16
#define RFM95_INT 21
#define RFM95_RST 17
#define RFM95_FREQ 915.0
#define RFM95_CODINGRATE 8
#define RFM95_BANDWIDTH 250000
#define RFM95_HeaderID 0x22
#define RFM95_SPREADFACTOR 8  // Spreading Factor Maxx is 12 any higher than 10 does not seem to function
#define RFM95_TXPOWER 23       //23 is max any higher than 13 can cause serial connection to not work properly
#define buffSize 50

byte LoRaBuff[buffSize];

//Create RFM95 object
RH_RF95 RF95(RFM95_CS, RFM95_INT);

//Create ADXL345 object and assign dummy ID (unused)
Adafruit_ADXL345_Unified ACCEL = Adafruit_ADXL345_Unified(12345);


void setup() {

  Serial.begin(115200);
  while (!Serial) delay(10);

  while (!startADXL345(ACCEL)) {while (1);}

  while (!startLoRa(RFM95_FREQ, RF95, RFM95_SPREADFACTOR, RFM95_TXPOWER, RFM95_CODINGRATE, RFM95_BANDWIDTH, RFM95_HeaderID)) {while (1);}
}

void loop() {
  
  for(int x = 0; x <= 200; x++){
    //Reads Accelerometer data and store it in the array
    readADXL345(LoRaBuff, ACCEL);
    
    if (!sendLoRaTestData(x, RF95, buffSize)) {
      Serial.println("Faild to Send");
    } 
    else {
      delay(250);
      Serial.println("Packet Sent");
    }  
  }

 for(int x = 200; x >= 0; x--){
    //Reads Accelerometer data and store it in the array
    readADXL345(LoRaBuff, ACCEL);
    
    if (!sendLoRaTestData(x, RF95, buffSize)) {
      Serial.println("Faild to Send");
    } 
    else {
      delay(250);
      Serial.println("Packet Sent");
    }
 }   
}
