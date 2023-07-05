#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

const uint8_t NODE_ID = 1;
const unsigned long PROCESSING_TIME_INTERVAL = 5000;
const unsigned long TRANSMISSION_TIME_INTERVAL = 10000;
const uint16_t MY_NETWORK_ID = 0xACDC;

// Jeedom Message
#define MSG_LUX "lux"
#define MSG_TEMP "temp"
#define MSG_HUMIDITY "hum"
#define MSG_DOOR_STATE "door_state"
#define MSG_RADIO_STATE "radio_state"

// Jeedom Command
#define MSG_DOOR "door"
#define MSG_RADIO "radio"
#define OPEN "open"
#define CLOSE "close"
#define STOP "stop"
#define START "start"

#define PIN_LUX_METER A0
#define PIN_MOTOR_IN1 6
#define PIN_MOTOR_IN2 7
#define PIN_MOTOR_LIMIT_OPEN 8
#define PIN_MOTOR_LIMIT_CLOSE 9
#define PIN_DHT22 A2
#define PIN_RADIO 2
#endif 