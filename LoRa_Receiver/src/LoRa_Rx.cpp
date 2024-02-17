#include <LoRa.h>
#include <SPI.h>


#define ss 5
#define rst 14
#define dio0 2


byte localAddress = 0x01;  // Address of this device (Controller = 0x01)
byte machdo = 0xAA;       // Address of Mesuring

//Data varialble
String TempData;
String PressData;
//Sensor
String temperature;
String pressCurrent;

// Remote sensor time variables
unsigned long currentActive1 = millis();
// unsigned long currentActive2 = millis();
const long checkInterval = 12500;  // 12.5 second sensor check interval
 
//Outgoing Message counter
byte msgCount = 0;

// FUNCTION getValue() - Extract value from delimited string
// String getValue(String data, char separator, int index) {
//   int found = 0;
//   int strIndex[] = { 0, -1 };
//   int maxIndex = data.length() - 1;
 
//   for (int i = 0; i <= maxIndex && found <= index; i++) {
//     if (data.charAt(i) == separator || i == maxIndex) {
//       found++;
//       strIndex[0] = strIndex[1] + 1;
//       strIndex[1] = (i == maxIndex) ? i + 1 : i;
//     }
//   }
//   return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
// }

//FUNTION onReceive() - Receive call-back funtion
void onReceive(int packetSize) {
  if(packetSize == 0) return;     // if there's no packet, return

  //read packet header bytes:
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
 
  // if the recipient isn't this device
  if (recipient != localAddress) {
    Serial.println("This message is not for me.");
    return;  // skip rest of function
  }

  // Determine sender, then update data variables and time stamps
  if (sender == machdo) {
    //Remote Sensor 
    TempData = incoming;
    currentActive1 = millis();
  }
}
// FUNCTION sendMessage() - Send LoRa Packet
void sendMessage(String outgoing, byte target) {
  LoRa.beginPacket();             // start packet
  LoRa.write(target);             // add sensorAddress1 address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}

// FUNCTION  getValues() - get the Data values from the machdo
void getValues() {
// Check to see if sensors have reported in recently
  // Get current timestamp value
  unsigned long currentMillis = millis();
 
  // See if we have exceeded the check interval time limit
  // Sensor 1
  if (currentMillis - currentActive1 <= checkInterval) {
    // Data is good, extract temp and press
    temperature = TempData;   // Remote 1 Temperature
    pressCurrent = PressData;  // Remote PressCurrent
  } else {
    temperature = "??.??";
    pressCurrent = "??.??";
  }
}
void setup() {
  Serial.begin(115200);
  while ((!Serial));

  // Setup LoRa module
  LoRa.setPins(ss, rst, dio0);
 
  Serial.println("LoRa Bridge Test");
  
  // Start LoRa module at local frequency
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
 
  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("LoRa init succeeded.");
}

void loop() {
  // Get latest data values
  getValues();

  Serial.print("Data sensor :  ");
  Serial.print("\n    Temp : ");
  Serial.print(temperature);
  Serial.print("\n    PressCurrent ");
  Serial.println(pressCurrent);
  
  // Send message to remote 1
  String outMsg1 = "";
  outMsg1 = outMsg1 + msgCount;
  sendMessage(outMsg1, machdo);

  // Place LoRa back into Receive Mode
  LoRa.receive();
 
  // Refresh the data values
  getValues();
  delay(300);
}






/*

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Node Receive: ");
  Serial.println(message);
}

void onTxDone() {
  Serial.println("Truyen hoan tat");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
void setup() {
  Serial.begin(115200);                   // initialize serial
  while (!Serial);

  LoRa.setPins(ss, rst , dio0);

  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println("Connecting.....");
    delay(500);
  }
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  //LoRa_rxMode();
}

void loop() {
  if (runEvery(5000)) { // repeat every 5000 millis

    String message = "HeLoRa World! ";
    message += "I'm a bridge ";
    message += millis();

    LoRa_sendMessage(message); // send a message

    Serial.println("Send Message!");
  }
}
*/



 /*
void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
 
  LoRa.setPins(ss, rst, dio0);    //setup pins module LoRa  
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println("Connecting.....");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
}
 
void loop() 
{
 
  // Serial.print("\nConnecting: ");
  int packetSize = LoRa.parsePacket();    // try to parse packet

  if (packetSize) 
  {
    
    Serial.print("Received packet '");
 
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }
    Serial.print("' with RSSI ");         // print RSSI of packet
    Serial.println(LoRa.packetRssi());
  }
  
}
*/