#ifndef BLUETOOTH_LAYER1_H
#define BLUETOOTH_LAYER1_H

#include "../wireless.h"
#include "../../base_type.h"

/***
 * Initializes the standard out.
 */
void Bluetooth_Init(void);

extern void Bluetooth_Send(wirelessMessage_t* msg);

#endif

