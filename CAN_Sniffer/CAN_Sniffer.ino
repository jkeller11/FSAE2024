//TODO LOG
//Add dashboard code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Adafruit_MCP2515.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "functions.h"

//MCP2515 Parameters
#define CAN_BAUDRATE (250000)  // Set CAN bus baud rate
#define CAN_CS 5               //Set CS pin

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
float RPM, BattVoltage, Lambda, OilPressure, EngineCoolant = 0;

//Create MCP object
Adafruit_MCP2515 MCP(CAN_CS);

//Create RFM95 object
RH_RF95 RF95(RFM95_CS, RFM95_INT);

//Create ADXL345 object and assign dummy ID (unused)
Adafruit_ADXL345_Unified ACCEL = Adafruit_ADXL345_Unified(12345);

void setup() {
  //Pins for checking if data collection is 'ON'
  pinMode(13,OUTPUT);
  pinMode(12,INPUT_PULLUP);
  digitalWrite(13, LOW);

  Serial.begin(115200);
  while (!Serial) delay(10);

  while (!startCAN(CAN_CS, CAN_BAUDRATE, MCP)) {while (1);}

  while (!startLoRa(RFM95_FREQ, RF95, RFM95_SPREADFACTOR, RFM95_TXPOWER, RFM95_CODINGRATE, RFM95_BANDWIDTH, RFM95_HeaderID)) {while (1);}

  while (!startADXL345(ACCEL)) {while (1);}
}

void loop() {
  //For Data collection switch on dashboard
  if(digitalRead(12) == LOW){
    //Reads Accelerometer data and store it in the array
    readADXL345(LoRaBuff, ACCEL);

    // Check for new CAN packet
    int packetSize = MCP.parsePacket();

    if (packetSize) {
      // received a packet read and add to LoRaBuff
      readCAN(LoRaBuff, MCP, RPM, BattVoltage, Lambda, OilPressure, EngineCoolant, packetSize);
    }

    //Sends LoRaBuff Array to raspberry PI
    //sendLoRa(LoRaBuff, RF95, buffSize);

    unsigned long StartTime = millis();
    //Sends LoRaBuff Array to raspberry PI Comment out for normal use
    if (!sendLoRa(LoRaBuff, RF95, buffSize)) {
      Serial.println("Faild to Send");
    } else {
      //Serial.println("Packet Sent");
      Serial.print("Time:"); Serial.println(millis() - StartTime);
    }
  }
}
