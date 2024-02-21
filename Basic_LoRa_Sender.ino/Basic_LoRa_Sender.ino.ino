#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS   16
#define RFM95_INT  21
#define RFM95_RST  17

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Destination address
uint8_t destinationAddress = 0xF1;

void setup() {
  Serial.begin(115200);
  if (!rf95.init()) {
    Serial.println("LoRa module initialization failed");
    while (1);
  }
  rf95.setFrequency(915.0);

  // Set the destination address
  rf95.setThisAddress(destinationAddress);
  rf95.setSpreadingFactor( 10);
}

int val = 0;
String message = "Number: ";

void loop() {
 
  String mess = message + val;
  int str_len = mess.length() + 1;
  char send[str_len];
  mess.toCharArray(send, str_len);
  
  // Send the message to the destination address
  rf95.send((uint8_t*)send, strlen(send));
  rf95.waitPacketSent();
  delay(2000);  // Send the message every  two second   WILL NEED TO REMOVE
  Serial.println("Message Sent");
  val++;
}
