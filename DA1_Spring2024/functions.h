//Attempts to configure and start MCP2515
bool startCAN(int CS_PIN, long baudRate, Adafruit_MCP2515 &MCP){

  //Set MCP filter for the IDs used acording to PE3 Documentation on CAN Protocl
  MCP.filterExtended(0x0000F000,0x0000F800);

  //Attempts start 10 times until timeout
  for(int x = 0; x < 10; x++){
    if(!MCP.begin(baudRate)){
      //Serial.println("Error initializing MCP2515.");
      delay(1000);
    }
    else{
      //Serial.println("MCP2515 chip found");
      return true;
    }
  }

 Serial.println("MCP2515 Not Found");
  return false;
}

//Attempts to configure and start ADX345
bool startADXL345(Adafruit_ADXL345_Unified &ACCEL){
  for(int x = 0; x < 10; x++){
    if (!ACCEL.begin()) {
      //Serial.println("ADXL345 initialization failed");
      delay(1000);
    }
    else{
      //Serial.println("ADXL345 initialization Successful");

      //Set G Range for ADX345
      ACCEL.setRange(ADXL345_RANGE_16_G);
      return true;
    }
  }
  Serial.println("ADXL345 Not Found");
  return false;
}

//Prints out last recevied CAN packet ID for debugging
void printCANID(Adafruit_MCP2515 &MCP){
  Serial.print("Received id 0x"); 
  Serial.println(MCP.packetId(), HEX);    //Print HEX ID
}

//Reads CAN data for dash data and store it in the array parameter
void readCAN(Adafruit_MCP2515 &MCP, float &ShockPot, float &SteeringAngle, float &freq, int packetSize){
  byte MCPBuf[8]; //Temp buffer array

  MCP.readBytes(MCPBuf, packetSize);      // Parse packetsize of bytes into MCPBuf STORED [LowByte, HighByte]
  int ID = MCP.packetId();

  //RPM
  if (ID == 0x0CFFF048) { 
  }

  else if (ID == 0x0CFFF148) {   
  }

  else if (ID == 0x0CFFF248) {
    ShockPot = (MCPBuf[1] << 8) + MCPBuf[0];      //Analog 1
    SteeringAngle = (MCPBuf[3] << 8) + MCPBuf[2]; //Analog 2

    if(ShockPot > 32767){                      //signed int convserion
      ShockPot = ShockPot - 65536;
    }

    if(SteeringAngle > 32767){                      //signed int convserion
      SteeringAngle = SteeringAngle - 65536;
    }
  }

  else if (ID == 0x0CFFF348) {
  }
  
  else if (ID == 0x0CFFF448) {
    freq = (MCPBuf[1] << 8) + MCPBuf[0]; //digital 1
    
    if(freq > 32767){                      //signed int convserion
      freq = freq - 65536;
    }
  }

  else if (ID== 0x0CFFF548) { 
  }
}
