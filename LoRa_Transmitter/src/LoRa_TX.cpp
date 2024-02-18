#include <LoRa.h>
#include <SPI.h>
#include <Arduino.h>
#define ss 4
#define rst 2
#define dio0 14
 
 
// Outgoing message variable
String outMessage;
 
// Message counter
byte msgCount = 0;
 
// Receive message variables
// String contents = "";
String Data = " ";
 
// Source and destination addresses
byte localAddress = 0xBB;  // address of this device
byte destination = 0xFF;   // destination to send to
 
int isDataComming = false;
 
// Send LoRa Packet
void sendMessage(String outgoing) {
  LoRa.beginPacket();             // start packet
  LoRa.write(destination);        // add destination address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}
 
// Receive Callback Function
void onReceive(int packetSize) {
  if (packetSize == 0) return;  // if there's no packet, return
 
  // Read packet header bytes:
  int recipient = LoRa.read();        // recipient address
  byte sender = LoRa.read();          // sender address
  byte incomingMsgId = LoRa.read();   // incoming msg ID
  byte incomingLength = LoRa.read();  // incoming msg length
 
  String incoming = "";  // payload of packet
 
  while (LoRa.available()) {        // can't use readString() in callback, so
    incoming += (char)LoRa.read();  // add bytes one by one
  }
 
  if (incomingLength != incoming.length()) {  // check length for error
    Serial.println("error: message length does not match length");
    return;  // skip rest of function
  }
     Serial.println(incoming);
{
  
  isDataComming = true;
}
}
void setup() {
 
  Serial.begin(115200);
  while (!Serial);
 
  Serial.println("LoRa Duplex with callback");
 
  // Setup LoRa module
  LoRa.setPins(ss, rst, dio0);
 
  // Start LoRa module at local frequency
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
 
  // Set Receive Call-back function
  LoRa.onReceive(onReceive);
 
  // Place LoRa in Receive Mode
  LoRa.receive();
 
  Serial.println("LoRa init succeeded.");
  Serial.println("Request from user: ");
}
 
void loop() {

  delay(100);
  // Send packet 
  if (isDataComming == true) {
    delay(1000);
    Serial.println("Request from user: ");
  // Compose and send message
    outMessage = Data + " " + String(millis());
    sendMessage(outMessage);
  // Place LoRa back into Receive Mode
    LoRa.receive();
    isDataComming = false;
  // delay(300);
  }
}
 
