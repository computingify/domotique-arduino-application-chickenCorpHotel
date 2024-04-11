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
#define DEBUG_MSG_ONE_LN(x) Serial.print(x)
#else
#define DEBUG_MSG(x) // define empty, so macro does nothing
#endif

// Objects instentiation
ChickenCorpDoor mNode;
LoRaHomeNode mLoRaHome(mNode);

// sampling management
unsigned long nextSendTime = 0;    // last send time
unsigned long nextProcessTime = 0; // last processing time
bool isNewMessageReceived(false);
bool isButtonProcessing(false);
unsigned int i;

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
  if(1000 < i){
    DEBUG_MSG_ONE_LN(".");
    i = 0;
  }
  i++;

  // Application processing Task
  if (tick >= nextProcessTime
    || isNewMessageReceived
    || isButtonProcessing) {
    bool isRunFastly = mNode.appProcessing();

    // Artificially set the time after ProcessingTimeInterval
    if (!isRunFastly) {
      nextProcessTime = millis() + mNode.getProcessingTimeInterval();
    }else {
      nextProcessTime = millis();
    }
    DEBUG_MSG("Processing");
  }

  // Send Task
  if ((tick >= nextSendTime)
    || (mNode.getTransmissionNowFlag() == true)) {
    mNode.setTransmissionNowFlag(false);
    DEBUG_MSG_VAR(tick);
    mLoRaHome.sendToGateway();
    DEBUG_MSG("");

    nextSendTime = millis() + mNode.getTransmissionTimeInterval();
  }

  // Receive Task
  isNewMessageReceived = mLoRaHome.receiveLoraMessage();

  // Button Management for door action
  isButtonProcessing = mNode.buttonMgt();

  // if (tick >= nextProcessTime || tick >= nextSendTime) {
  //   mLoRaHome.reset();
  //   DEBUG_MSG("Reset");
  // }
}