#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <DataStructure.h>
#define ss 4
#define rst 2
#define dio0 14


char *sensorDataString;
char randomString;
struct sensorData sensorData_st;

// Outgoing message variable
String outMessage;

// Bridge data variable
String inMessage;
 
// Previous value Bridge data variable
String inMessageOld;
 
// Outgoing Message counter
byte msgCount = 0;
 
// Source and destination addresses
byte localAddress = 0xAA;  // address of this device (must be unique, 0xAA or 0xBB)
byte destination = 0x01;   // destination to send to (Bridge = 0x01)
 
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
 
  // If the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;  // skip rest of function
  }
 
  // If we are this far then this message is for us
  // Update the controller data variable
  inMessage = incoming;
}

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
String generateRandomString(int length) {
  String randomString = "";
  const char charSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for (int i = 0; i < length; i++) {
    randomString += charSet[random(0, sizeof(charSet) - 1)];
  }

  return randomString;
}
void device_dataManagement()
{  
  // Generate random sensor data
  sensorData_st.temperature = random(10, 40); // Random temperature value between 10 and 40
  sensorData_st.pressCurrent = random(30, 70);    // Random humidity value between 30 and 70

  // Generate random string
  char dateTime_string[] = "2024-02-17 08:30:00"; // Replace this with your actual datetime string
  String randomString = generateRandomString(10); // Generate a random string with length 10

  // Format the sensor data string
  createSensorDataString(sensorDataString, "CHARrrr", dateTime_string, sensorData_st);
}
void setup() {
 
  Serial.begin(9600);
  while (!Serial);
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
}

void loop() {
  // device_dataManagement();
  // Serial.println(sensorDataString);
  // Run only if requested
  if (inMessage != inMessageOld) {
    // New message variable, take reading and send to controller

    // Printing the results on the serial monitor
    Serial.print("Data being sent ...: ");
    Serial.print(sensorDataString);

    // Format the outgoing message string
    String outMsg = "";
    outMsg = outMsg + sensorDataString + randomString;

// Send data as LoRa packet
sendMessage(outMsg);
    // Send data as LoRa packet
    sendMessage(outMsg);
 
    // LoRa.beginPacket();
    // createSensorDataString(sensorDataString, "aa", "bb", sensorData_st);
    // Serial.println(sensorDataString);
    
    // Print controller variables
    Serial.print("Old Controller Data = ");
    Serial.println(inMessageOld);
    Serial.print("New Controller Data = ");
    Serial.println(inMessage);
 
    // Update the"old" data variable
    inMessageOld = inMessage;
 
    // Place LoRa in Receive Mode
    LoRa.receive();

    delay(2000);
 
  }
}

//--------------------------------------------LoRa Node-----------------------------------
/*

void LoRa_rxMode(){
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
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

  Serial.print("Gateway Receive: ");
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

void setup() 
{
  Serial.begin(115200); 
  while (!Serial);
  Serial.println("LoRa TX");
 
  LoRa.setPins(ss, rst , dio0);    //setup pins module LoRa 
  
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println("Connecting.....");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}
 
void loop() {
  if (runEvery(5000)) { // repeat every 5000 millis

    String message = "HeLoRa! ";
    message += "I'm machdo! ";
    // message += millis();

    LoRa_sendMessage(message); // send a message

    Serial.println("Send Message!");
  }
}

*/
// ------------------------------------------------------------

//int counter = 0;

 /*
void setup() 
{
  Serial.begin(115200); 
  while (!Serial);
  Serial.println("LoRa TX");
 
  LoRa.setPins(ss, rst , dio0);    //setup pins module LoRa 
  
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
  int i = 0;
  while(i < 1)
  {
    LoRa.receive();
    i++;
  }
  int packetSize = LoRa.parsePacket();
  if(packetSize) { 
    Serial.print("Connect success!");
    Serial.print("Sending packet: ");
    // Serial.println(counter);
 
    // LoRa.beginPacket();   //Send LoRa packet to receiver
    // LoRa.print("hello ");
    // LoRa.print(counter);
    // LoRa.endPacket();
    // counter++;
    // delay(100);
  }
  LoRa.beginPacket();   //Send LoRa packet to receiver
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(100);
}
 /*
   if(RF_requestData == true){
    digitalWrite(PIN_NUM_5V_CTRL, HIGH);
    digitalWrite(PIN_NUM_12V_CTRL, HIGH);
    device_getData();
    device_dataManagement();
    digitalWrite(PIN_NUM_5V_CTRL, LOW);
    digitalWrite(PIN_NUM_12V_CTRL, LOW);
    RF_requestData = false;
    device_previousDataControl = millis();
  } 
  else if(millis() - device_previousDataControl >= DEVICE_DATA_SAVE_INTERVAL){
    digitalWrite(PIN_NUM_5V_CTRL, HIGH);
    digitalWrite(PIN_NUM_12V_CTRL, HIGH);
    device_getData();
    device_dataManagement();
    digitalWrite(PIN_NUM_5V_CTRL, LOW);
    digitalWrite(PIN_NUM_12V_CTRL, LOW);
    device_previousDataControl = millis();
  }

 */
