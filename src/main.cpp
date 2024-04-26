#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <LoRaHomeNode.h>
#include <SPI.h>
#include "ChickenCorpDoor.h"

// #define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(x) Serial.println(F(x))
#define DEBUG_MSG_VAR(x) Serial.println(x)
#define DEBUG_MSG_ONE_LN(x) Serial.print(x)
#else
#define DEBUG_MSG(x) // define empty, so macro does nothing
#define DEBUG_MSG_VAR(x)
#define DEBUG_MSG_ONE_LN(x)
#endif

// Objects instentiation
ChickenCorpDoor mNode;
LoRaHomeNode mLoRaHome(mNode);

// sampling management
unsigned long nextSendTime;    // last send time
unsigned long nextProcessTime; // last processing time
bool isNewMessageReceived;
#ifdef DEBUG
unsigned int i;
#endif

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

  nextSendTime = 0;
  nextProcessTime = 0;
  isNewMessageReceived = false;
}

/**
* Main loop of the LoRa Node
* Constantly try to receive JSON LoRa message
* Every transmissionTimeInterval send JSON LoRa messages
*/
void loop()
{
  unsigned long tick = millis();
#ifdef DEBUG
  if(1000 < i){
    DEBUG_MSG_ONE_LN(".");
    i = 0;
  }
  i++;
#endif

  // Application processing Task
  if (tick >= nextProcessTime
    || isNewMessageReceived) {
    
    mNode.appProcessing();

    nextProcessTime = millis() + mNode.getProcessingTimeInterval();
  }

  // Send Task
  if ((tick >= nextSendTime)
    || (mNode.getTransmissionNowFlag() == true)) {

    mNode.setTransmissionNowFlag(false);

    mLoRaHome.sendToGateway();

    nextSendTime = millis() + mNode.getTransmissionTimeInterval();
  }

  // Receive Task
  isNewMessageReceived = mLoRaHome.receiveLoraMessage();
}