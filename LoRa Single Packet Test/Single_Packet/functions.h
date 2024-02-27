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
