//Attempts to configure and start MCP2515
bool startCAN(int CS_PIN, long baudRate, Adafruit_MCP2515 &MCP){

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
    bool lora = RF95.init();
    if (!lora) {
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
  buff[32] = byte(int(event.acceleration.x*100) & 0xFF); //LSB first
  buff[33] = byte(int(event.acceleration.x*100) >> 8); //MSB second
  buff[34] = byte(int(event.acceleration.y*100) & 0xFF); //LSB first
  buff[35] = byte(int(event.acceleration.y*100) >> 8); //MSB second
  buff[36] = byte(int(event.acceleration.z*100) & 0xFF); //LSB first
  buff[37] = byte(int(event.acceleration.z*100) >> 8); //MSB second
}

//Prints out last recevied CAN packet ID for debugging
void printCANID(Adafruit_MCP2515 &MCP){
  Serial.print("Received id 0x"); 
  Serial.println(MCP.packetId(), HEX);    //Print HEX ID
}

//Reads CAN data for dash data and store it in the array parameter
//Also Sets Neopixels when value is Set
void readCAN(byte buff[], Adafruit_MCP2515 &MCP, float &RPM, float &BattVoltage, float &OilPressure, float &EngineCoolant, int packetSize, Adafruit_NeoPixel &NEO){//, Adafruit_NeoPixel &STICK){
  byte MCPBuf[8]; //Temp buffer array

  MCP.readBytes(MCPBuf, packetSize);      // Parse packetsize of bytes into MCPBuf STORED [LowByte, HighByte]
  int ID = MCP.packetId();

  //RPM
  if (ID == 0x0CFFF048) { 
    RPM = (MCPBuf[1] << 8) + MCPBuf[0];  //Parse RPM for tachometer

    // //Calculate number of LEDS to turn on
    // float rpm = (RPM - 1800) / 1300;
    // int on_count = 0;

    // //Sets Neopixels on Stick based on RPM value
    // for(int x = 0; x <= rpm; x++){
    //   STICK.setPixelColor(x,255,0,0);
    //   on_count++;
    // }

    // //Turns off remaining neopixels
    // for(int x = on_count + 1; x < 8; x++){
    //   STICK.setPixelColor(x,0,0,0);
    // }

    // // Send the updated pixel colors to the neopixel
    // STICK.show(); 
    
    //Puts byte values into LoRa array
    for(int x = 0; x < 4; x++){
      buff[x] = MCPBuf[x];
    }
  }
  else if (ID == 0x0CFFF148) { 
    //Puts byte values into LoRa array
    for(int x = 0; x < 2; x++){
      buff[x+4] = MCPBuf[x+2];
    }
  }
  
  else if (ID == 0x0CFFF248) {
    OilPressure = (MCPBuf[3] << 8) + MCPBuf[2];  
    if(OilPressure > 32767){                      //signed int convserion
      OilPressure = OilPressure - 65536;
    }

    //Sets Neopixel based on new value
    float val = ((OilPressure/1000) * 25) - 12.5;  //Calc real value read from CAN and convert to PSI

    if(val >= 20){
      NEO.setPixelColor(1,0,255,0);
    }
    else if(val >= 10){
      NEO.setPixelColor(1,255,172,28);
    }
    else{
      NEO.setPixelColor(1,255,0,0);
    }
    NEO.show();

    Serial.print("OIL:");
    Serial.println(val);
    
    //Puts byte values into LoRa array
    for(int x = 0; x < 6; x++){
      buff[x+6] = MCPBuf[x+2];
    }
  }
  
  else if (ID == 0x0CFFF348) {
    //Puts byte values into LoRa array
    for(int x = 0; x < 6; x++){
      buff[x+12] = MCPBuf[x];
    }
  }
  
  else if (ID == 0x0CFFF448) {
    //Puts byte values into LoRa array
    for(int x = 0; x < 8; x++){
      buff[x+18] = MCPBuf[x];
    }
  }

  //Battery Voltage, Engine Coolant
  else if (ID== 0x0CFFF548) { 
    BattVoltage = (MCPBuf[1] << 8) + MCPBuf[0];  
    if(BattVoltage > 32767){                      //signed int convserion
      BattVoltage = BattVoltage - 65536;
    }

    //Sets Neopixel based on new value
    float val = BattVoltage/100;  //Calc real value read from CAN

    if(val >= 12){
      NEO.setPixelColor(0,0,255,0);
    }
    else if(val >= 11.8){
      NEO.setPixelColor(0,255,172,28);
    }
    else{
      NEO.setPixelColor(0,255,0,0);
    }

    NEO.show(); 

    Serial.print("Battery:");
    Serial.println(val);

    EngineCoolant = (MCPBuf[5] << 8) + MCPBuf[4];  
    if(EngineCoolant > 32767){                      //signed int convserion
      EngineCoolant = EngineCoolant - 65536;
    }

    //Sets Neopixel based on new value
    val = EngineCoolant/10;  //Calc real value read from CAN

    if(val > 200){
      NEO.setPixelColor(2,255,0,0);
    }
    else if(val >= 150){
      NEO.setPixelColor(2,0,255,0);
    }
    else{
      NEO.setPixelColor(2,0,0,255);
    }
    NEO.show(); 

    Serial.print("Coolant:");
    Serial.println(val);

    //Puts byte values into LoRa array
    for(int x = 0; x < 6; x++){
      buff[x+26] = MCPBuf[x];
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