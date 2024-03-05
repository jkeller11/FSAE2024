#include <SPI.h>   
#include <mcp_can.h>

#define PE1 0x0CFFF048
#define PE2 0x0CFFF148
#define PE3 0x0CFFF248
#define PE4 0x0CFFF348
#define PE5 0x0CFFF448
#define PE6 0x0CFFF548

#define SPI_PIN 10

byte canMsg1[8] = {byte(1 & 0xFF), byte(1 >> 8), byte(2 & 0xFF), byte( 2 >> 8), byte(3 & 0xFF), byte(3 >> 8), byte(4 & 0xFF), byte(4 >> 8)};
byte canMsg2[8] = {byte(5 & 0xFF), byte(5 >> 8), byte(6 & 0xFF), byte( 6 >> 8), byte(7 & 0xFF), byte(7 >> 8), byte(8 & 0xFF), byte(8 >> 8)};
byte canMsg3[8] = {byte(9 & 0xFF), byte(9 >> 8), byte(10 & 0xFF), byte(10 >> 8), byte(11 & 0xFF), byte(11 >> 8), byte(12 & 0xFF), byte(12 >> 8)};
byte canMsg4[8] = {byte(13 & 0xFF), byte(13 >> 8), byte(14 & 0xFF), byte(14 >> 8), byte(15 & 0xFF), byte(15 >> 8), byte(16 & 0xFF), byte(16 >> 8)};
byte canMsg5[8] = {byte(17 & 0xFF), byte(17 >> 8), byte(18 & 0xFF), byte(18 >> 8), byte(19 & 0xFF), byte(19 >> 8), byte(20 & 0xFF), byte(20 >> 8)};
byte canMsg6[8] = {byte(21 & 0xFF), byte(21 >> 8), byte(22 & 0xFF), byte(22 >> 8), byte(23 & 0xFF), byte(23 >> 8), byte(24 & 0xFF), byte(24 >> 8)};

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
  randomSeed(analogRead(0));                                              // randomSeed() shuffle the random function
}

void loop() {

  //radnomly generatre values for can values for testing
  //int rand1 = random(5001);
  //int rand2 = random(5001);
  //int rand3 = random(5001);
  //int rand4 = random(5001);

  
  //See PE3 CAN Bus Protocol for info on IDs and data
  //CAN message data values scaled acording to resolution 
  //EX analog #1 is 0 to 5 but with a resolution of 0.001 so 5123 would be 5.123
  // CAN0.sendMsgBuf(PE1, 1, 8, canMsg1);                 //RPM,TPS,Fuel Open Time and Ignition Angle                          
  // CAN0.sendMsgBuf(PE2, 1, 8, canMsg2);                 //Barometer, MAP, Lambda and Pressure Type
  // CAN0.sendMsgBuf(PE3, 1, 8, canMsg3);                 //Analog Inputs 1-4
  // CAN0.sendMsgBuf(PE4, 1, 8, canMsg4);                 //Analog Inputs 5-8
  // CAN0.sendMsgBuf(PE5, 1, 8, canMsg5);                 //Wheel Speed Freq 1-4
  // CAN0.sendMsgBuf(PE6, 1, 8, canMsg6);                 //Battery Voltage, Air Temp, Coolant Temp and Temp Type
  // delay(100);

  //Test Pot send data
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  byte pot[8] = {0, 0, 0, 0, 0, 0, byte(sensorValue & 0xFF), byte(sensorValue >> 8)};
  CAN0.sendMsgBuf(PE4, 1, 8, pot);
  Serial.println(voltage);
  delay(100);



}