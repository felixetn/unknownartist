/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file i2c.c
 * @author mschnaubelt
 * @brief I2C bus driver
 **/

// TODO
#include "i2c.h"
#include "api.h"

void I2C_BUS0_Init(void) {
	PIER04_IE4 = 1; // SDA0 pin to input (QFP-100)
	PIER04_IE5 = 1; // SCL0 pin to input (QFP-100)
	PUCR04_PU4 = 1;
	PUCR04_PU5 = 1;

	ICCR0_EN = 0; // stop I2C interface
	ICCR0_CS = 31;
	ICCR0_NSF = 1;
	ICCR0_EN = 1; // enable I2C interface

	IDAR0 = 0; // clear data register

	IBCR0_BER = 0; // clear bus error interrupt flag
	IBCR0_BEIE = 0; // bus error interrupt disabled
	IBCR0_ACK = 0; // no acknowledge generated
	IBCR0_GCAA = 0; // no call acknowledge is generated
	IBCR0_INTE = 0; // disable interrupt
	IBCR0_INT = 0; // clear transfer interrupt request flag
}

void I2C_BUS0_Acknowlegde(void) {
	while (IBSR0_LRB == 1)
		; // no anwser from slave, program stucks here
		  // a timeout mechanism should be implemented here
}

uint8_t I2C_BUS0_CheckAddressPresence(uint8_t slave_address) {
	uint8_t result;
	do {
		IBCR0_BER = 0; // clear bus error interrupt flag
		ICCR0_EN = 1; // enable I2C interface
		IDAR0 = slave_address; // slave_address is sent out with start condition

		IBCR0_MSS = 1; // set master mode and set start condition
		IBCR0_INT = 0; // clear transfer end interrupt flag

		while (IBCR0_INT == 0)
			; // look if transfer is in process
	} while (IBCR0_BER == 1); // retry if Bus-Error detected

	if (IBSR0_LRB == 1)
		result = 0;
	else
		result = 1;

	IBCR0_MSS = 0; // change to slave and release stop condition
	IBCR0_INT = 0; // clear transfer end interrupt flag
	while (IBSR0_BB)
		; // wait till bus free

	return result;
}

void I2C_BUS0_Start(uint8_t slave_address, uint8_t write) {
	if (!write)
		slave_address |= 0x01;

	do {
		IBCR0_BER = 0; // clear bus error interrupt flag
		ICCR0_EN = 1; // enable I2C interface
		IDAR0 = slave_address; // slave_address is sent out with start condition

		IBCR0_MSS = 1; // set master mode and set start condition
		IBCR0_INT = 0; // clear transfer end interrupt flag

		while (IBCR0_INT == 0)
			; // look if transfer is in process
	} while (IBCR0_BER == 1); // retry if Bus-Error detected

	while (IBSR0_LRB == 1) // no acknowledge means device not ready
	{ // maybe last write cycle not ended yet
		IBCR0_SCC = 1; // try restart (= continue)
		while (IBCR0_INT == 0)
			; // wait that transfer is finished
	}
}

void I2C_BUS0_Continue(uint8_t slave_address, uint8_t write) {
	if (!write)
		slave_address |= 0x01;

	IDAR0 = slave_address; // slave_address is sent out with start condition
	IBCR0_SCC = 1; // restart (= continue)
	while (IBCR0_INT == 0)
		; // wait that transfer is finished
}

void I2C_BUS0_Stop(void) {
	while (IBCR0_INT == 0)
		; // wait that transfer is finished
	IBCR0_MSS = 0; // change to slave and release stop condition
	IBCR0_INT = 0; // clear transfer end interrupt flag
	while (IBSR0_BB)
		; // wait till bus free
}

void I2C_BUS0_Write(uint8_t value) {
	IDAR0 = value; // load data or address in to register
	IBCR0_INT = 0; // clear transfer end intrerupt flag
	while (IBCR0_INT == 0)
		; // look if transfer is in process
	I2C_BUS0_Acknowlegde(); // wait for Acknowledge
}

uint8_t I2C_BUS0_Read(void) {
	IBCR0_ACK = 1; // acknowledge has to be send
	IBCR0_INT = 0; // clear transfer end interrupt flag
	while (IBCR0_INT == 0)
		; // wait that transfer is finished
	return (IDAR0); // read received data out
}

uint8_t I2C_BUS0_LastRead(void) {
	IBCR0_ACK = 0; // no acknowledge has to be sent after last byte
	IBCR0_INT = 0; // clear transfer end interrupt flag
	while (IBCR0_INT == 0)
		; // wait that transfer is finished
	return (IDAR0); // read received data out
}

void I2C_BUS1_Init(void) {
	PIER04_IE6 = 1; // SDA0 pin to input (QFP-100)
	PIER04_IE7 = 1; // SCL0 pin to input (QFP-100)
	PUCR04_PU6 = 1;
	PUCR04_PU7 = 1;

	ICCR1_EN = 0; // stop I2C interface
	ICCR1_CS4 = 1; // CS4..0 : set prescaler
	ICCR1_CS3 = 1;
	ICCR1_CS2 = 1;
	ICCR1_CS1 = 1;
	ICCR1_CS0 = 1;
	ICCR1_EN = 1; // enable I2C interface

	IDAR1 = 0; // clear data register

	IBCR1_BER = 0; // clear bus error interrupt flag
	IBCR1_BEIE = 0; // bus error interrupt disabled
	IBCR1_ACK = 0; // no acknowledge generated
	IBCR1_GCAA = 0; // no call acknowledge is generated
	IBCR1_INTE = 0; // disable interrupt
	IBCR1_INT = 0; // clear transfer interrupt request flag
}

void I2C_BUS1_Acknowlegde(void) {
	while (IBSR1_LRB == 1)
		; // no anwser from slave, program stucks here
		  // a timeout mechanism should be implemented here
}

uint8_t I2C_BUS1_CheckAddressPresence(uint8_t slave_address) {
	uint8_t result;
	do {
		IBCR1_BER = 0; // clear bus error interrupt flag
		ICCR1_EN = 1; // enable I2C interface
		IDAR1 = slave_address; // slave_address is sent out with start condition

		IBCR1_MSS = 1; // set master mode and set start condition
		IBCR1_INT = 0; // clear transfer end interrupt flag

		while (IBCR1_INT == 0)
			; // look if transfer is in process
	} while (IBCR1_BER == 1); // retry if Bus-Error detected

	if (IBSR1_LRB == 1)
		result = 0;
	else
		result = 1;

	IBCR1_MSS = 0; // change to slave and release stop condition
	IBCR1_INT = 0; // clear transfer end interrupt flag
	while (IBSR1_BB)
		; // wait till bus free

	return result;
}

void I2C_BUS1_Start(uint8_t slave_address, uint8_t write) {
	if (!write)
		slave_address |= 0x01;

	do {
		IBCR1_BER = 0; // clear bus error interrupt flag
		ICCR1_EN = 1; // enable I2C interface
		IDAR1 = slave_address; // slave_address is sent out with start condition

		IBCR1_MSS = 1; // set master mode and set start condition
		IBCR1_INT = 0; // clear transfer end interrupt flag

		while (IBCR1_INT == 0)
			; // look if transfer is in process
	} while (IBCR1_BER == 1); // retry if Bus-Error detected

	while (IBSR1_LRB == 1) // no acknowledge means device not ready
	{ // maybe last write cycle not ended yet
		IBCR1_SCC = 1; // try restart (= continue)
		while (IBCR1_INT == 0)
			; // wait that transfer is finished
	}
}

void I2C_BUS1_Continue(uint8_t slave_address, uint8_t write) {
	if (!write)
		slave_address |= 0x01;

	IDAR1 = slave_address; // slave_address is sent out with start condition
	IBCR1_SCC = 1; // restart (= continue)
	while (IBCR1_INT == 0)
		; // wait that transfer is finished
}

void I2C_BUS1_Stop(void) {
	while (IBCR1_INT == 0)
		; // wait that transfer is finished
	IBCR1_MSS = 0; // change to slave and release stop condition
	IBCR1_INT = 0; // clear transfer end interrupt flag
	while (IBSR1_BB)
		; // wait till bus free
}

void I2C_BUS1_Write(uint8_t value) {
	IDAR1 = value; // load data or address in to register
	IBCR1_INT = 0; // clear transfer end intrerupt flag
	while (IBCR1_INT == 0)
		; // look if transfer is in process
	I2C_BUS1_Acknowlegde(); // wait for Acknowledge
}

uint8_t I2C_BUS1_Read(void) {
	IBCR1_ACK = 1; // acknowledge has to be send
	IBCR1_INT = 0; // clear transfer end interrupt flag
	while (IBCR1_INT == 0)
		; // wait that transfer is finished
	return (IDAR1); // read received data out
}

uint8_t I2C_BUS1_LastRead(void) {
	IBCR1_ACK = 0; // no acknowledge has to be sent after last byte
	IBCR1_INT = 0; // clear transfer end interrupt flag
	while (IBCR1_INT == 0)
		; // wait that transfer is finished
	return (IDAR1); // read received data out
}

