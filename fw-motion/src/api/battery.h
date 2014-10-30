#include "defines.h"

#ifndef BATTERY_H_
#define BATTERY_H_

void Battery_Init(void);

/**
 * calculates the battery_level in mV
 * @return battery level in mV
 */
uint16_t Battery_GetVoltage(void);

#endif
