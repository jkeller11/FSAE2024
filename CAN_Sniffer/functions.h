//Attempts to configure and start MCP2515
bool startCAN(int CS_PIN, long baudRate, Adafruit_MCP2515 &MCP){

  //Set MCP filter for the IDs used acording to PE3 Documentation on CAN Protocl
  MCP.filterExtended(0x0000F000,0x0000F800);

  //Attempts start 10 times until timeout
  for(int x = 0; x < 10; x++){
    if(!MCP.begin(baudRate)){
      Serial.println("Error initializing MCP2515.");
      delay(1000);
    }
    else{
      Serial.println("MCP2515 chip found");
      return true;
    }
  }

  Serial.println("MCP2515 Not Found");
  return false;
}

//Attempts to configure and start RFM95 LoRa Chip
bool startLoRa(long freq, RH_RF95 &RF95, uint8_t spreadFactor, uint8_t TxPower, uint8_t codingRate, int bandwidth, int ID){
  
  
  for(int x = 0; x < 10; x++){
    if (!RF95.init()) {
      Serial.println("LoRa module initialization failed");
      delay(1000);
    }
    else{
      Serial.println("LoRa module initialization Successful");

      //Set Parameters of RFM95 module
      RF95.setFrequency(freq);
      RF95.setSpreadingFactor(spreadFactor);
      RF95.setTxPower(TxPower);
      RF95.setCodingRate4(codingRate);
      RF95.setSignalBandwidth(bandwidth);
      RF95.setHeaderFrom(ID);

      return true;
    }
  }
  Serial.println("RFM95 Not Found");
  return false;
}

//Attempts to configure and start ADX345
bool startADXL345(Adafruit_ADXL345_Unified &ACCEL){
  for(int x = 0; x < 10; x++){
    if (!ACCEL.begin()) {
      Serial.println("ADXL345 initialization failed");
      delay(1000);
    }
    else{
      Serial.println("ADXL345 initialization Successful");

      //Set G Range for ADX345
      ACCEL.setRange(ADXL345_RANGE_8_G);
      return true;
    }
  }
  Serial.println("ADXL345 Not Found");
  return false;
}

//Print ADXL345 data for debugging
void printADXL345(Adafruit_ADXL345_Unified &ACCEL){
   /* Get a new sensor event */ 
  sensors_event_t event; 
  ACCEL.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  //delay(500);
}

//Reads Accelerometer data and store it in the array parameter
void readADXL345(byte buff[], Adafruit_ADXL345_Unified &ACCEL){
   // Get a new sensor event // 
  sensors_event_t event; 
  ACCEL.getEvent(&event);

  //Convert and push accerlation values to LoRa Buffer Array
  buff[44] = byte(int(event.acceleration.x*100) & 0xFF); //LSB first
  buff[45] = byte(int(event.acceleration.x*100) >> 8); //MSB second
  buff[46] = byte(int(event.acceleration.y*100) & 0xFF); //LSB first
  buff[47] = byte(int(event.acceleration.y*100) >> 8); //MSB second
  buff[48] = byte(int(event.acceleration.z*100) & 0xFF); //LSB first
  buff[49] = byte(int(event.acceleration.z*100) >> 8); //MSB second
}

//Prints out last recevied CAN packet ID for debugging
void printCANID(Adafruit_MCP2515 &MCP){
  Serial.print("Received id 0x"); 
  Serial.println(MCP.packetId(), HEX);    //Print HEX ID
}

//Reads CAN data for dash data and store it in the array parameter
//Also Sets Neopixels when value is Set
void readCAN(byte buff[], Adafruit_MCP2515 &MCP, float &RPM, float &BattVoltage, float &Lambda, float &OilPressure, float &EngineCoolant, int packetSize){
  byte MCPBuf[8]; //Temp buffer array

  MCP.readBytes(MCPBuf, packetSize);      // Parse packetsize of bytes into MCPBuf STORED [LowByte, HighByte]
  int ID = MCP.packetId();

  //RPM
  if (ID == 0x0CFFF048) { 
    RPM = (MCPBuf[1] << 8) + MCPBuf[0];  //Parse RPM for tachometer
   
    for(int x = 0; x < 8; x++){
      buff[x] = MCPBuf[x];
    }
  }

  //Lambda
  else if (ID == 0x0CFFF148) { 
    Lambda = (MCPBuf[5] << 8) + MCPBuf[4];  
    if(Lambda > 32767){                      //signed int convserion
      Lambda = Lambda - 65536;
    }
    for(int x = 0; x < 6; x++){
      buff[x+8] = MCPBuf[x];
    }

  }

  //Oil Pressure
  else if (ID == 0x0CFFF248) {
    //Set Values 
    OilPressure = (MCPBuf[1] << 8) + MCPBuf[0];  
    if(OilPressure > 32767){                      //signed int convserion
      OilPressure = OilPressure - 65536;
    }
     for(int x = 0; x < 8; x++){
      buff[x+14] = MCPBuf[x];
    }
  }
  else if (ID == 0x0CFFF348) {
    for(int x = 0; x < 8; x++){
      buff[x+22] = MCPBuf[x];
    }
  }
  
  else if (ID == 0x0CFFF448) {
    for(int x = 0; x < 8; x++){
      buff[x+30] = MCPBuf[x];
    }
  }

  //Battery Voltage, Engine Coolant
  else if (ID== 0x0CFFF548) { 
    BattVoltage = (MCPBuf[1] << 8) + MCPBuf[0];  
    if(BattVoltage > 32767){                      //signed int convserion
      BattVoltage = BattVoltage - 65536;
    }

    EngineCoolant = (MCPBuf[5] << 8) + MCPBuf[4];  
    if(EngineCoolant > 32767){                      //signed int convserion
      EngineCoolant = EngineCoolant - 65536;
    }

    for(int x = 0; x < 6; x++){
      buff[x+38] = MCPBuf[x];
    }
  }
}

//Sends LoRaBuff to raspberry PI
bool sendLoRa(byte buff[], RH_RF95 &RF95, int buffSize){
  return RF95.send((uint8_t*)buff, buffSize);
 
}

//Sends a packet of data with the value sent based on datas position in packet
//Mutlipler is for adjusting values
//RPM will always be zero
bool sendLoRaTestDataMulti(int multiplier, RH_RF95 &RF95, int buffSize){
  byte buff[buffSize];

  for (int x = 0; x <buffSize; x++){
    if(x % 2 == 0){
      buff[x] == byte((x*multiplier) & 0xFF); // grab low byte
    }
    else{
      buff[x] == byte((x*multiplier) >> 8); // grab high byte
    }
  }

  return RF95.send((uint8_t*)buff, buffSize);
 
}

//Sends a packet of empty data with only the specificed value being set
//Position must be even value!!
bool sendLoRaTestData(int position, int value, RH_RF95 &RF95, int buffSize){
  byte buff[buffSize];

  // define array as all zeros to avoid junk value
  for(int x = 0; x < buffSize; x++){
    buff[x] = 0;
  }

  // check if position is even and then set values
  if(position % 2 == 0){
    buff[position] = byte(value & 0xFF); // grab low byte
    buff[position+1] = byte(value >> 8); // grab high byte

    return RF95.send((uint8_t*)buff, buffSize);
  }
  else{
    return false;
  }
}

//Sets color on Neopixel Stick
void SetSTICK(float rpm){
  // Set all pixel colors to 'off'
  STICK.clear(); 

  //Calculate number of LEDS to turn on
  rpm = (rpm - 1800) / 1300;

  //Sets Neopixels on Stick based on RPM value
  for(int x = 0; x <= rpm; x++){
    STICK.setPixelColor(x, STICK.Color(255, 0, 0));
  }
  
  // Send the updated pixel colors to the hardware
  STICK.show(); 
}

//Sets color on Standard Neopixel
//See Packet_Layout_SQLite_Sensor_Pinout.xlsx for more info
void SetNEO_BATTERY(float val){
  int neoLED = 0; //Index of chained Neopixels
  val = val/100;  //Calc real value read from CAN

  if(val >= 12){
    NEO.setPixelColor(neoLED, NEO.Color(0, 255, 0));
  }
  else if(val >= 11.8){
    NEO.setPixelColor(neoLED, NEO.Color(255, 172, 28));
  }
  else{
    NEO.setPixelColor(neoLED, NEO.Color(255, 0, 0));
  }

  NEO.show();
}

//Sets color on Standard Neopixel
//See Packet_Layout_SQLite_Sensor_Pinout.xlsx for more info
void SetNEO_OIL(float val){
  int neoLED = 1; //Index of chained Neopixels
  val = ((val/1000) * 25) - 12.5;  //Calc real value read from CAN and convert to PSI

  if(val >= 10){
    NEO.setPixelColor(neoLED, NEO.Color(0, 255, 0));
  }
  else{
    NEO.setPixelColor(neoLED, NEO.Color(255, 0, 0));
  }
  NEO.show();
}

//Sets color on Standard Neopixel
//See Packet_Layout_SQLite_Sensor_Pinout.xlsx for more info
void SetNEO_COOLANT(float val){
  int neoLED = 2; //Index of chained Neopixels
  val = val/10;  //Calc real value read from CAN

  if(val > 190){
    NEO.setPixelColor(neoLED, NEO.Color(255, 0, 0));
  }
  else if(val >= 150){
    NEO.setPixelColor(neoLED, NEO.Color(0, 255, 0));
  }
  else{
    NEO.setPixelColor(neoLED, NEO.Color(0, 0, 255));
  }
  NEO.show(); 
}

//TO DO GET REAL LAMBDA VALS
//Sets color on Standard Neopixel
//See Packet_Layout_SQLite_Sensor_Pinout.xlsx for more info
void SetNEO_LAMBDA(float val){
  int neoLED = 4; //Index of chained Neopixels
  val = val/100;  //Calc real value read from CAN

  if(val > 1.01){
    NEO.setPixelColor(neoLED, NEO.Color(255, 172, 28));
  }
  else if(val >= 0.98){
    NEO.setPixelColor(neoLED, NEO.Color(0, 255, 0));
  }
  else{
    NEO.setPixelColor(neoLED, NEO.Color(255, 0, 0));
  }
  NEO.show();
}

//Sets color on Standard Neopixel
//See Packet_Layout_SQLite_Sensor_Pinout.xlsx for more info
void SetNEO_NEUTRAL(bool val){
  int neoLED = 3; //Index of chained Neopixels

  if(val){
    NEO.setPixelColor(neoLED, NEO.Color(0, 0, 255));
  }
  else{
    NEO.setPixelColor(neoLED, NEO.Color(0, 0, 0));
  }

  NEO.show();
}


