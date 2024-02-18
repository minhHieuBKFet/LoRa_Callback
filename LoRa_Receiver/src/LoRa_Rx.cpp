#include <LoRa.h>
#include <SPI.h>

#define ss 5
#define rst 14
#define dio0 2


// Outgoing message variable
String outMessage;

// Message counter
byte msgCount = 0;

// Source and destination addresses
byte localAddress = 0xBB;  // address of this device
byte destination = 0xFF;   // destination to send to

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
  
  Serial.println("This is data sent to me:  ");
  Serial.println(incoming);

// ----------------------------------update version---------------
  // // If the recipient isn't this device or broadcast,
  // if (recipient != localAddress && recipient != 0xFF) {
  // Serial.println("This message is not for me.");
  //   return;  // skip rest of function
  // }  
}

void setup() {
  Serial.begin(9600);
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
  Serial.printf("Enter your request: ");
}

void loop() {
  delay(2000);
  // Send packet if button pressed
  if(Serial.available() > 0) {
    
    String tmp = Serial.readString();
    delay(100);
    Serial.println(tmp);
    // Compose and send message
    outMessage = tmp;

    sendMessage(outMessage);
    // delay(100);
    // Place LoRa back into Receive Mode
    LoRa.receive();
  }
}

