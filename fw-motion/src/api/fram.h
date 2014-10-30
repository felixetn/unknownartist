/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file fram.h
 * @author Christian Illy
 * @brief TODO - insert a brief description of that interface
 **/

#ifndef FRAM_H_
#define FRAM_H_
#include "fstdint.h"

#define FRAM_MODULE_I2C_BASE_ADDRESS	0xA0

void FRAM_Init(void);

uint8_t FRAM_Available(uint8_t module);

void FRAM_Write(uint8_t module, uint16_t address, uint8_t* buffer, uint16_t len);
void FRAM_Read(uint8_t module, uint16_t address, uint8_t* buffer, uint16_t len);

#endif /* FRAM_H_ */
