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


byte RPM1[8] = {byte(1000 & 0xFF), byte(1000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM2[8] = {byte(2000 & 0xFF), byte(2000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM3[8] = {byte(3000 & 0xFF), byte(3000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM4[8] = {byte(4000 & 0xFF), byte(4000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM5[8] = {byte(5000 & 0xFF), byte(5000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM6[8] = {byte(6000 & 0xFF), byte(6000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM7[8] = {byte(7000 & 0xFF), byte(7000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM8[8] = {byte(8000 & 0xFF), byte(8000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM9[8] = {byte(9000 & 0xFF), byte(9000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM10[8] = {byte(10000 & 0xFF), byte(10000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM11[8] = {byte(11000 & 0xFF), byte(11000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM12[8] = {byte(12000 & 0xFF), byte(12000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};
byte RPM13[8] = {byte(13000 & 0xFF), byte(13000 >> 8), byte(0 & 0xFF), byte(0 >> 8), byte(1500 & 0xFF), byte(1500 >> 8), byte(0 & 0xFF), byte(0 >> 8)};


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
  // //Bat Volt
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg2);
  // delay(3000);
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg3);
  // delay(3000);
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg1);
  // delay(3000);

  // //Coolant temp
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg6);
  // delay(3000);
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg7);
  // delay(3000);
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg1);
  // delay(3000);

  // //Oil Pressure
  // CAN0.sendMsgBuf(PE3, 1, 8, canMsg4);
  // delay(3000);
  // CAN0.sendMsgBuf(PE3, 1, 8, canMsg5);
  // delay(3000);
  // CAN0.sendMsgBuf(PE3, 1, 8, canMsg1);
  // delay(3000);

  // delay(5000);

  //RPM
  CAN0.sendMsgBuf(PE1, 1, 8, RPM1);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM2);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM3);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM4);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM5);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM6);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM7);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM8);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM9);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM10);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM11);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM12);
  delay(100);
  CAN0.sendMsgBuf(PE1, 1, 8, RPM13);
  delay(3000);
  



}