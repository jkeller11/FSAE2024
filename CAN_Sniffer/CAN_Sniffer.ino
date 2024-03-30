//See Packet_Layout_SQLite_Sensor_Pinout.xlsx for more info on program specifics
#include <Adafruit_MCP2515.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>
#include "functions.h"   //custom functions to clean up code

//MCP2515 Parameters
//See Adafruit_MCP2515 library for more info
#define CAN_BAUDRATE (250000)  // Set CAN bus baud rate
#define CAN_CS 5               //Set CS pin

//RFM95 Setup Parameters
//See RadioHead library for more info
#define RFM95_CS 16
#define RFM95_INT 21
#define RFM95_RST 17
#define RFM95_FREQ 915.0          
#define RFM95_CODINGRATE 8
#define RFM95_BANDWIDTH 250000
#define RFM95_HeaderID 0x22     //ID that Raspi Will check for when reading packet
#define RFM95_SPREADFACTOR 8    //Spreading Factor Maxx is 12 any higher than 10 does not seem to function
#define RFM95_TXPOWER 23        //23 is max any higher than 13 can cause serial connection to not work properly
#define buffSize 56             //Size of LoRa packet -- Array of bytes

//Neopixel Parameters
//See Neopixel library for more info
#define STICK_NUM 8
#define STICK_PIN 6
#define NEO_NUM 5
#define NEO_PIN 24

//Global Variables for lora array and Dash Values
byte LoRaBuff[buffSize]; //LoRa Packets
float RPM, BattVoltage, OilPressure, EngineCoolant = 0;

//Create MCP object
Adafruit_MCP2515 MCP(CAN_CS);

//Create RFM95 object
RH_RF95 RF95(RFM95_CS, RFM95_INT);

//Create ADXL345 object and assign dummy ID (unused)
Adafruit_ADXL345_Unified ACCEL = Adafruit_ADXL345_Unified(12345);

//Create Neopixel Objects
Adafruit_NeoPixel STICK(STICK_NUM, STICK_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel NEO(NEO_NUM, NEO_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Pins for checking if data collection is 'ON' and neutral light
  pinMode(13,OUTPUT);
  pinMode(12,INPUT_PULLUP);
  digitalWrite(13, LOW);
  pinMode(10,INPUT_PULLUP);

  // INITIALIZE NeoPixel objects
  STICK.begin();
  NEO.begin();

  //Set Neopixels Max Brightness to 50%
  STICK.setBrightness(25);
  NEO.setBrightness(25);

  //Start Serial, MCP2515 (CAN), ADXL345 (Accelerometer), RFM95 (LoRa)
  Serial.begin(9600);
  //while (!Serial) delay(1000);
  while (!startCAN(CAN_CS, CAN_BAUDRATE, MCP)) {while (1);}
  while (!startLoRa(RFM95_FREQ, RF95, RFM95_SPREADFACTOR, RFM95_TXPOWER, RFM95_CODINGRATE, RFM95_BANDWIDTH, RFM95_HeaderID)) {while (1);}
  while (!startADXL345(ACCEL)) {while (1);}
}

void loop() {

  //Turns on Neutral Light if Pin 10 is pulled to ground
  SetNEO_NEUTRAL(digitalRead(10), NEO);

  // Check for new CAN packet
  int packetSize = MCP.parsePacket();

  //Filters packets and ignores irennous packets
  if((packetSize && (MCP.packetId() & 0x00000800 == 0x00000000)) || (packetSize && (MCP.packetId() == 0x0CFFF848))){
    // received a packet read and add to LoRaBuff as well as set neopixels
    //readCAN(LoRaBuff, MCP, RPM, BattVoltage, OilPressure, EngineCoolant, packetSize, NEO, STICK);
  }

  //For Data collection switch on dashboard
  if(digitalRead(12) == LOW){
    //Reads Accelerometer data and store it in the array
    readADXL345(LoRaBuff, ACCEL);

    //Sends LoRaBuff Array to raspberry PI
    sendLoRa(LoRaBuff, RF95, buffSize);
    Serial.println("Sent");

    //For Debugging execution Time
    // unsigned long StartTime = millis();
    // //Sends LoRaBuff Array to raspberry PI Comment out for normal use
    // if (!sendLoRa(LoRaBuff, RF95, buffSize)) {
    //   Serial.println("Faild to Send");
    // } else {
    //   //Serial.println("Packet Sent");
    //   Serial.print("Time:"); Serial.println(millis() - StartTime);
    // }
  }
  
}
