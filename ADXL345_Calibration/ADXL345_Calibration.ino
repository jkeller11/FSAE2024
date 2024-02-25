#include <Wire.h> // Include the Wire library for I2C communication

#define ADXL345_ADDRESS (0x53) // ADXL345 device address

// ADXL345 Register Addresses
#define ADXL345_OFSX 0x1E // X-axis offset register
#define ADXL345_OFSY 0x1F // Y-axis offset register
#define ADXL345_OFSZ 0x20 // Z-axis offset register

void setup() {
  Serial.begin(9600); // Start serial communication
  Wire.begin(); // Initialize I2C communication
  
  // Set the offset values for each axis (replace these values with your desired offsets)
  setOffset(3,0,-1); 
}

void loop() {
  // Nothing in the loop for this example
}

// Function to set offset registers only do one at a time
void setOffset(int Zoffset, int Yoffset, int Xoffset) {
  Wire.beginTransmission(ADXL345_ADDRESS); // Start I2C transmission
  //Wire.write(ADXL345_OFSZ); // Select the Z-axis offset register
  //Wire.write(Zoffset);
  //Wire.write(ADXL345_OFSY); // Select the Y-axis offset register
  //Wire.write(Yoffset);
  Wire.write(ADXL345_OFSX); // Select the X-axis offset register
  Wire.write(Xoffset);
  Wire.endTransmission(); // End transmission
}
