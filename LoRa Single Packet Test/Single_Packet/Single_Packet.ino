#include <RH_RF95.h>
#include "functions.h"

//RFM95 Setup Parameters
#define RFM95_CS 16
#define RFM95_INT 21
#define RFM95_RST 17
#define RFM95_FREQ 915.0
#define RFM95_CODINGRATE 8
#define RFM95_BANDWIDTH 250000
#define RFM95_HeaderID 0x22
#define RFM95_SPREADFACTOR 8  // Spreading Factor Maxx is 12 any higher than 10 does not seem to function
#define RFM95_TXPOWER 23       //23 is max any higher than 13 can cause serial connection to not work properly
#define buffSize 50

byte LoRaBuff[buffSize];

//Create RFM95 object
RH_RF95 RF95(RFM95_CS, RFM95_INT);

void setup() {

  Serial.begin(115200);
  while (!Serial) delay(10);

  while (!startLoRa(RFM95_FREQ, RF95, RFM95_SPREADFACTOR, RFM95_TXPOWER, RFM95_CODINGRATE, RFM95_BANDWIDTH, RFM95_HeaderID)) {while (1);}
}

void loop() {
  
  for(int x = 0; x <= 100; x++){
    if (!sendLoRaTestData(x, RF95, buffSize)) {
    Serial.println("Faild to Send");
    } 
    else {
      Serial.println("Packet to Sent");
    }
    delay(250);
  }

  Serial.print("Done");
  while(1);
}
