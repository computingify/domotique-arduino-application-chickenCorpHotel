#include "ChickenCorpDoor.h"
#include "NodeConfig.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(x) Serial.println(F(x))
#define DEBUG_MSG_VAR(x) Serial.println(x)
#else
#define DEBUG_MSG(x) // define empty, so macro does nothing
#define DEBUG_MSG_VAR(x)
#endif


/*!********************************************************************
 * @brief Construct a new Node:: Node object
 **********************************************************************/
ChickenCorpDoor::ChickenCorpDoor() :
    mLux(A0),
    mMotor(2)
{
}

/**
* Function invoked by the node right after its own setup (as per Arduino Setup function)
* To be used for applicative setup
*/
void ChickenCorpDoor::appSetup() {
    // set the Id of the node
    this->setNodeId(NODE_ID);
    this->setProcessingTimeInterval(PROCESSING_TIME_INTERVAL);
    this->setTransmissionTimeInterval(TRANSMISSION_TIME_INTERVAL);
    // ask for current state transmission
    setTransmissionNowFlag(true);
}


/**
* Add JSON Tx payload messages
* @param payload the JSON payload to be completed as per application needs
*/
void ChickenCorpDoor::addJsonTxPayload(JsonDocument& payload) {
    // send a simple tx counter
    static uint8_t i = 0;
    payload["tx"] = i++;
    DEBUG_MSG("--- Send msg ...");
}

/**
* Parse JSON Rx payload
* One should avoid any long processing in this routine. LoraNode::AppProcessing is the one to be used for this purpose
* Limit the processing to parsing the payload and retrieving the expected attributes
* @param payload the JSON payload received by the node
*/
void ChickenCorpDoor::parseJsonRxPayload(JsonDocument& payload) {
    // assume receicing a message with json key "msg", display it
    if (payload["msg"].isNull() == false)
    {
        DEBUG_MSG("--- receive:");
        DEBUG_MSG_VAR((const char*)payload["msg"]);
    }
}

/**
* App processing of the node.
* Invoke every loop of the nodes before Rx and Tx
* ONe should benefit from using processingTimeInterval to avoid overloading the node
*/
void ChickenCorpDoor::appProcessing() {
    mLux.Run();
}