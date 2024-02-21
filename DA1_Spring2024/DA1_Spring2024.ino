// The purpose of this code is to test and validate CAN and sensor data out of the PE3 ECU to fullfill DA1

#include <Adafruit_MCP2515.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "functions.h"

#define CAN_BAUDRATE (250000)  // Set CAN bus baud rate
#define CAN_CS 5               //Set CS pin
#define first = true

//Create MCP object
Adafruit_MCP2515 MCP(CAN_CS);

//Create ADXL345 object and assign dummy ID (unused)
Adafruit_ADXL345_Unified ACCEL = Adafruit_ADXL345_Unified(12345);



float ShockPot, SteeringAngle, freq = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  while (!startCAN(CAN_CS, CAN_BAUDRATE, MCP)) {while (1);}

  while (!startADXL345(ACCEL)) {while (1);}

  ACCEL.setRange(ADXL345_RANGE_16_G);

  delay(3000);

  //Print headers for csv format
  Serial.println("Shock Pot, Steering Angle, Frequency, Accel X, Accel Y, Accel Z");

}

void loop() {
  //Get, convert and print data for testing
  sensors_event_t event; 
  ACCEL.getEvent(&event);

  // Check for new CAN packet
  int packetSize = MCP.parsePacket();

  if (packetSize) {
    // received a packet read and add to LoRaBuff
    readCAN( MCP, ShockPot, SteeringAngle, freq, packetSize);
  }
 
  Serial.print(ShockPot/1000);
  Serial.print(", ");
  Serial.print(SteeringAngle/1000);
  Serial.print(", ");
  Serial.print(freq); //not sure on conversion
  Serial.print(", ");
  Serial.print(event.acceleration.x);
  Serial.print(", ");
  Serial.print(event.acceleration.y);
  Serial.print(", ");
  Serial.println(event.acceleration.z);
  //delay(1000);
}
