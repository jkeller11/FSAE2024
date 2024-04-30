#include <SPI.h>   
#include <mcp_can.h>

#define PE1 0x0CFFF048
#define PE2 0x0CFFF148
#define PE3 0x0CFFF248
#define PE4 0x0CFFF348
#define PE5 0x0CFFF448
#define PE6 0x0CFFF548

#define SPI_PIN 10

byte canMsg1[8] = {byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte canMsg2[8] = {byte(2000 & 0xFF), byte(2000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte canMsg3[8] = {byte(1180 & 0xFF), byte(1180 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8)};

byte canMsg4[8] = {byte(0 & 0xFF), byte(0 >> 8), byte(20000 & 0xFF), byte(20000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte canMsg5[8] = {byte(0 & 0xFF), byte(0 >> 8), byte(10000 & 0xFF), byte(10000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8)};

byte canMsg6[8] = {byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1900 & 0xFF), byte(1900 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte canMsg7[8] = {byte(0 & 0xFF), byte(0 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};


MCP_CAN CAN0(SPI_PIN);


void setup() {
  Serial.begin(9600);                                                     //Arduino comm with PC
  SPI.begin();                                                            //SPI comm start

  CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ);                             //Parameters for CAN

  if (CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {             //IF to make sure CAN
    Serial.println("MCP2515 Initialized Successfully!");                  //parameters were set up
  }                                                                     
  else {
    Serial.println("Error Initializing MCP2515...");
  }

  CAN0.setMode(MCP_NORMAL);                                               //CAN set to Normal Mode
}

void loop() {

  //See PE3 CAN Bus Protocol for info on IDs and data
  //CAN message data values scaled acording to resolution 
  //EX analog #1 is 0 to 5 but with a resolution of 0.001 so 5123 would be 5.123
  //CAN0.sendMsgBuf(PE1, 1, 8, canMsg1);                 //RPM,TPS,Fuel Open Time and Ignition Angle                          
  //CAN0.sendMsgBuf(PE2, 1, 8, canMsg1);                 //Barometer, MAP, Lambda and Pressure Type
  //CAN0.sendMsgBuf(PE3, 1, 8, canMsg1);                 //Analog Inputs 1-4
  //CAN0.sendMsgBuf(PE4, 1, 8, canMsg1);                 //Analog Inputs 5-8
  //CAN0.sendMsgBuf(PE5, 1, 8, canMsg1);                 //Wheel Speed Freq 1-4
  //CAN0.sendMsgBuf(PE6, 1, 8, canMsg1);                 //Battery Voltage, Air Temp, Coolant Temp and Temp Type
  //delay(1000);

  //Bat Volt
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg2);
  delay(3000);
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg3);
  delay(3000);
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg1);
  delay(3000);

  //Coolant temp
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg6);
  delay(3000);
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg7);
  delay(3000);
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg1);
  delay(3000);

  //Oil Pressure
  CAN0.sendMsgBuf(PE3, 1, 8, canMsg4);
  delay(3000);
  CAN0.sendMsgBuf(PE3, 1, 8, canMsg5);
  delay(3000);
  CAN0.sendMsgBuf(PE3, 1, 8, canMsg1);
  delay(3000);

  delay(5000);

  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg2);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg3);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg4);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg5);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg6);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg7);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg8);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg9);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg10);
  // delay(100);
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg11);
  // delay(100);



}