/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file fram.c
 * @author Christian Illy
 * @brief TODO - insert a brief description of that module
 **/

#include "fram.h"
#include "i2c.h"

static uint8_t enabledModules;

void FRAM_Init(void) {
	I2C_BUS1_Init();

	enabledModules = 0;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (0 << 1)) << 0;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (1 << 1)) << 1;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (2 << 1)) << 2;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (3 << 1)) << 3;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (4 << 1)) << 4;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (5 << 1)) << 5;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (6 << 1)) << 6;
	enabledModules |= I2C_BUS1_CheckAddressPresence(
			FRAM_MODULE_I2C_BASE_ADDRESS + (7 << 1)) << 7;
}

uint8_t FRAM_Available(uint8_t module) {
	if (module > 7)
		return 0;
	if (enabledModules & (1 << module))
		return 1;
	return 0;
}

void FRAM_Write(uint8_t module, uint16_t address, uint8_t* buffer, uint16_t len) {
	if (!FRAM_Available(module))
		return;
	I2C_BUS1_Start(FRAM_MODULE_I2C_BASE_ADDRESS + (module << 1), 1);
	I2C_BUS1_Write(address >> 8);
	I2C_BUS1_Write(address & 0xff);
	while (len--) {
		I2C_BUS1_Write(*buffer);
		buffer++;
	}
	I2C_BUS1_Stop();
}

void FRAM_Read(uint8_t module, uint16_t address, uint8_t* buffer, uint16_t len) {
	if (!FRAM_Available(module))
		return;
	I2C_BUS1_Start(FRAM_MODULE_I2C_BASE_ADDRESS + (module << 1), 1);
	I2C_BUS1_Write(address >> 8);
	I2C_BUS1_Write(address & 0xff);
	I2C_BUS1_Continue(FRAM_MODULE_I2C_BASE_ADDRESS + (module << 1), 0);
	if (len > 0) {
		while (--len) {
			*buffer = I2C_BUS1_Read();
			buffer++;
		}
		*buffer = I2C_BUS1_LastRead();
	}
	I2C_BUS1_Stop();
}
