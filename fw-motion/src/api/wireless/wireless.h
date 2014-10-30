#ifndef WIRELESS_H_
#define WIRELESS_H_

#include "../api.h"

#define WIRELESSLAYER_MAX_DATA_LENGTH 64
#define WIRELESS_MAX_SUBSCRIBER_FUNCTIONS 30
#define SENDBUFFER_SIZE 20

#define UARTMESSAGE_BUFFER_SIZE (WIRELESSLAYER_MAX_DATA_LENGTH + 7 + 3 + 3 + 5)

// Priority 2 and 3 messages are only sent if either there is no message of higher
// priority to be sent or on every N-th message which is sent. Define what value is
// used for N.
#define PRIORITY3 9
#define PRIORITY2 3

// definition of the message format
typedef struct {
		uint8_t destinationId;
		uint8_t sourceId;
		uint8_t messageType;
		uint8_t priority;
		uint8_t dataLength;
		uint8_t data[WIRELESSLAYER_MAX_DATA_LENGTH];
} wirelessMessage_t;

typedef enum {
	WI_IF_AMB8420,
	WI_IF_BLUETOOTH
} wireless_interface_t;

// callback function type
typedef void (*wirelessSubscribeFunction)(wireless_interface_t, wirelessMessage_t*);


/**
 * @brief Initializes the layer and calls layer 3 initialization
 */
void wirelessInit(void);


/**
 * @brief adds a function to the list of functions to be called on receiving a message
 * @param function pointer to the callback function
 * @param message type - 0 is reserved
 * @return result status of operation
 */
uint8_t wirelessSubscribe(wirelessSubscribeFunction callback, uint8_t messageType);

/**
 * @brief is checking if a function is subscribed
 * @param function pointer to the callback function
 * @param message type - 0 is reserved
 * @return result status of operation
 */
uint8_t isSubscribed(wirelessSubscribeFunction callback, uint8_t messageType);

/**
 * @brief removes a function from the list of functions to be called on receiving a message
 * @param function pointer to the callback function
 * @param message type - 0 is reserved
 * @return result status of operation
 */
void wirelessUnsubscribe(wirelessSubscribeFunction callback, uint8_t messageType);

/**
 * @brief receive a message
 * @param received layer 1 message
 * @return the received message
 */
void wirelessReceive(wireless_interface_t wiif, wirelessMessage_t* message);

/**
 * @brief send a message
 * @param the pointer of the message to be sent
 */
void wirelessSend(wireless_interface_t wiif, wirelessMessage_t* message);

void wirelessFormattedDebugMessage(wireless_interface_t wiif, const char* format, ...);

void wirelessDebugMessage(wireless_interface_t wiif, char* message);

#endif /* WIRELESSLAYER0_H_ */
