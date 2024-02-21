#include <SPI.h>   
#include <mcp_can.h>

#define PE1 0x0CFFF048
#define PE2 0x0CFFF148
#define PE3 0x0CFFF248
#define PE4 0x0CFFF348
#define PE5 0x0CFFF448
#define PE6 0x0CFFF548

#define SPI_PIN 10

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
  randomSeed(analogRead(0));                                              // randomSeed() will then shuffle the random function
}

void loop() {

  //radnomly generatre values for can values for testing
  int rand1 = random(5001);
  int rand2 = random(5001);
  int rand3 = random(5001);
  int rand4 = random(5001);

  byte canMsg2[8] = {byte(rand1 & 0xFF), byte(rand1 >> 8),byte(rand2 & 0xFF), byte(rand2 >> 8),byte(rand3 & 0xFF), byte( rand3 >> 8), byte(rand4 & 0xFF), byte(rand4 >> 8)};
  byte canMsg1[8] = {byte(1000 & 0xFF), byte(1000 >> 8), byte(2 & 0xFF), byte( 2 >> 8), byte(3 & 0xFF), byte(3 >> 8), byte(4 & 0xFF), byte(4 >> 8)};
  byte canMsg3[8] = {byte(22 & 0xFF), byte(22 >> 8), byte(2 & 0xFF), byte( 2 >> 8), byte(3 & 0xFF), byte(3 >> 8), byte(4 & 0xFF), byte(4 >> 8)};
  

  
  //See PE3 CAN Bus Protocol for info on IDs and data
  //CAN message data values scaled acording to resolution 
  //EX analog #1 is 0 to 5 but with a resolution of 0.001 so 5123 would be 5.123
  CAN0.sendMsgBuf(PE1, 1, 8, canMsg1);                 //RPM,TPS,Fuel Open Time and Ignition Angle                          
  CAN0.sendMsgBuf(PE2, 1, 8, canMsg1);                 //Barometer, MAP, Lambda and Pressure Type
  CAN0.sendMsgBuf(PE3, 1, 8, canMsg1);                 //Analog Inputs 1-4
  CAN0.sendMsgBuf(PE4, 1, 8, canMsg2);                 //Analog Inputs 5-8
  CAN0.sendMsgBuf(PE5, 1, 8, canMsg2);                 //Wheel Speed Freq 1-4
  CAN0.sendMsgBuf(PE6, 1, 8, canMsg3);                 //Battery Voltage, Air Temp, Coolant Temp and Temp Type
  delay(100);
}