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
bool sendLoRaTestData(int value, RH_RF95 &RF95, int buffSize){
  byte buff[buffSize];

  // define array as all zeros to avoid junk value
  for(int x = 0; x < buffSize; x+=2){
    buff[x] = byte(value & 0xFF); // grab low byte
    buff[x+1] = byte(value >> 8); // grab high byte
  }


  
  return RF95.send((uint8_t*)buff, buffSize);
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
      ACCEL.setRange(ADXL345_RANGE_16_G);
      return true;
    }
  }
  Serial.println("ADXL345 Not Found");
  return false;
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