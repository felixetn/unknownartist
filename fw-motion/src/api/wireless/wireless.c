/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file wirelessLayer0.c
 * @author Team Innovation (Sommersemester 2012)
 **/

/*
 * Include files
 */

#include "wireless.h"
#include "AMB8420/amb8420.h"
#include "bluetooth/bluetooth.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "../carid.h"
#include <string.h>
#include <stdarg.h>
#include "../printf_stdarg.h"

typedef struct {
	wirelessSubscribeFunction callback;
	uint8_t messageType;
} wirelessLayer2_subscriber;

/*
 * Global and local variable declarations
 */
static wirelessLayer2_subscriber subscribers[WIRELESS_MAX_SUBSCRIBER_FUNCTIONS];

// Used for debug output methods
static wirelessMessage_t msg;

/*
 * Global and local functions
 */

/**
 * @brief is sending a message to the driver
 */
void wirelessSend(wireless_interface_t wiif, wirelessMessage_t* msg) {
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	portBASE_TYPE returnpd = pdFALSE;

	msg->sourceId = carid;

	switch (wiif) {
		case WI_IF_AMB8420:
			AMB8420_Send(msg);
			break;
		case WI_IF_BLUETOOTH:
			Bluetooth_Send(msg);
			break;
	}
}


void wirelessDebugMessage(wireless_interface_t wiif, char* message) {
	msg.destinationId = 0;
	msg.messageType = 0;
	msg.priority = 3;
	msg.dataLength = strlen(message);
	memcpy(msg.data, message, msg.dataLength);
	wirelessSend(wiif, &msg);
}

void wirelessFormattedDebugMessage(wireless_interface_t wiif, const char* format, ...) {
	va_list args;
	char* strStart = (char*)msg.data;
	va_start( args, format );
	print(&strStart, format, args);

	msg.destinationId = 0;
	msg.messageType = 0;
	msg.priority = 3;
	msg.dataLength = strlen((const char*)msg.data);
	wirelessSend(wiif, &msg);
}

/**
 * @brief Initializes the layer and calls layer 2 initialization
 */
void wirelessInit(void) {
	uint8_t i;

	AMB8420_Init();
	Bluetooth_Init();

	//reserve some space for the callback function-subscription
	for (i = 0; i < WIRELESS_MAX_SUBSCRIBER_FUNCTIONS; i++) {
		subscribers[i].callback = 0;
		subscribers[i].messageType = 0;
	}
}

/**
 * @brief adds a function to the list of functions to be called on receiving a message
 * @param function pointer to the callback function
 * @param message type - 0 is reserved
 * @return result status of operation
 */
uint8_t wirelessSubscribe(wirelessSubscribeFunction callback, uint8_t messageType) {
	volatile uint8_t i;

	for (i = 0; i < WIRELESS_MAX_SUBSCRIBER_FUNCTIONS; i++) {
		if (subscribers[i].callback == 0) {
			subscribers[i].callback = callback;
			subscribers[i].messageType = messageType;

			return 1;
		}
	}
	return 0;
}

/**
 * @brief is checking if a function is subscribed
 * @param function pointer to the callback function
 * @param message type - 0 is reserved
 * @return result status of operation
 */
uint8_t isSubscribed(wirelessSubscribeFunction callback, uint8_t messageType) {
	uint8_t i;

	for (i = 0; i < WIRELESS_MAX_SUBSCRIBER_FUNCTIONS; i++) {
		if (subscribers[i].callback == callback && subscribers[i].messageType == messageType) {
			return 1;
		}
	}
	return 0;
}

/**
 * @brief removes a function from the list of functions to be called on receiving a message
 * @param function pointer to the callback function
 * @param message type - 0 is reserved
 * @return result status of operation
 */
void wirelessUnsubscribe(wirelessSubscribeFunction callback, uint8_t messageType) {
	uint8_t i;

	for (i = 0; i < WIRELESS_MAX_SUBSCRIBER_FUNCTIONS; i++) {
		if (subscribers[i].callback == callback && subscribers[i].messageType == messageType) {
			subscribers[i].callback = 0;
			subscribers[i].messageType = 0;
		}
	}
}

/**
 * @brief receive a message
 * @param received layer 1 message
 * @return the received message
 */
void wirelessReceive(wireless_interface_t wiif, wirelessMessage_t* message) {
	uint8_t i;

	for (i = 0; i < WIRELESS_MAX_SUBSCRIBER_FUNCTIONS; i++) {
		if ((subscribers[i].callback != 0) && (subscribers[i].messageType == message->messageType)) {
			subscribers[i].callback(wiif, message);
		}
	}
}
