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
    mLux(PIN_LUX_METER),
    mMotor(PIN_MOTOR_IN1, PIN_MOTOR_IN2, PIN_MOTOR_LIMIT_OPEN, PIN_MOTOR_LIMIT_CLOSE),
    mTempHumidity(PIN_DHT22, DHT22),
    mRadio(PIN_RADIO)
{
}

/**
* Function invoked by the node right after its own setup (as per Arduino Setup function)
* To be used for applicative setup
*/
void ChickenCorpDoor::appSetup() {
    // set the Id of the node
    setNodeId(NODE_ID);
    setProcessingTimeInterval(PROCESSING_TIME_INTERVAL);
    setTransmissionTimeInterval(TRANSMISSION_TIME_INTERVAL);
    // ask for current state transmission
    setTransmissionNowFlag(true);

    //Initialize Temperature Humidity sensor
    mTempHumidity.begin();
}


/**
* Add JSON Tx payload messages
* @param payload the JSON payload to be completed as per application needs
*/
void ChickenCorpDoor::addJsonTxPayload(JsonDocument& payload) {

    payload[MSG_LUX] = mLux.Get();
    payload[MSG_HUMIDITY] = mTempHumidity.readHumidity();
    payload[MSG_TEMP] = mTempHumidity.readTemperature();
    payload[MSG_DOOR_STATE] = mMotor.GetState();
    payload[MSG_RADIO_STATE] = mRadio.GetState();
    serializeJson(payload, Serial);
}

/**
* Parse JSON Rx payload
* One should avoid any long processing in this routine. LoraNode::AppProcessing is the one to be used for this purpose
* Limit the processing to parsing the payload and retrieving the expected attributes
* @param payload the JSON payload received by the node
* return true in case of new message received
*/
bool ChickenCorpDoor::parseJsonRxPayload(JsonDocument& payload) {
    bool isMessageReceived(false);

    if (!payload[MSG_DOOR].isNull()) {
        if (OPEN == payload[MSG_DOOR]) {
            mMotor.Open();
        }
        else if (CLOSE == payload[MSG_DOOR]) {
            mMotor.Close();
        }
        else if (STOP == payload[MSG_DOOR]) {
            mMotor.Stop();
        }
        isMessageReceived = true;
    }
    if (!payload[MSG_RADIO].isNull()) {
        if (START == payload[MSG_RADIO]) {
            mRadio.Enable();
        }
        else if (STOP == payload[MSG_RADIO]) {
            mRadio.Disable();
        }
    }

    needTransmissionNow = true;
    return isMessageReceived;
}

/**
* App processing of the node.
* Invoke every loop of the nodes before Rx and Tx
* ONe should benefit from using processingTimeInterval to avoid overloading the node
*/
bool ChickenCorpDoor::appProcessing() {
    bool isRunFastly(false);

    // Sample lux measure
    mLux.Run();

    // Manage door
    if (eDoorState::eOpened != mMotor.GetState()
        && eDoorState::eClosed != mMotor.GetState()) {
        isRunFastly = !mMotor.isProcessFinish();
    }

    return isRunFastly;
}