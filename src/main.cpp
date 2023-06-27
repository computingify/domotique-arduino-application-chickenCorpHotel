#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <LoRaHomeNode.h>
#include <SPI.h>
#include "ChickenCorpDoor.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(x) Serial.println(F(x))
#define DEBUG_MSG_VAR(x) Serial.println(x)
#else
#define DEBUG_MSG(x) // define empty, so macro does nothing
#endif

// Objects instentiation
ChickenCorpDoor mNode;
LoRaHomeNode mLoRaHome(mNode);

// sampling management
unsigned long lastSendTime = 0;    // last send time
unsigned long lastProcessTime = 0; // last processing time

void setup()
{

  //initialize Serial Monitor
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial)
    ;
#endif
  DEBUG_MSG("initializing LoRa Node");
  // initialize LoRa    
  mLoRaHome.setup();
  // call node specific configuration (end user)
  mNode.appSetup();

  // Update Data before start
  mNode.appProcessing();
}

/**
* Main loop of the LoRa Node
* Constantly try to receive JSON LoRa message
* Every transmissionTimeInterval send JSON LoRa messages
*/
void loop()
{
  unsigned long tick = millis();
  if ((tick - lastProcessTime) > mNode.getProcessingTimeInterval())
  {
    mNode.appProcessing();
    lastProcessTime = millis();
  }
  if (((tick - lastSendTime) > mNode.getTransmissionTimeInterval()) || (mNode.getTransmissionNowFlag() == true))
  {
    mNode.setTransmissionNowFlag(false);
    mLoRaHome.sendToGateway();
    lastSendTime = millis(); // timestamp the message
  }
  mLoRaHome.receiveLoraMessage();
}
