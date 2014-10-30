/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file i2c.h
 * @author mschnaubelt
 * @brief I2C bus driver
 **/

#ifndef __I2C_H__
#define __I2C_H__

#include "base_type.h"

void I2C_BUS0_Init(void);
uint8_t I2C_BUS0_CheckAddressPresence(uint8_t slave_address);
void I2C_BUS0_Start(uint8_t slave_address, uint8_t write);
void I2C_BUS0_Continue(uint8_t slave_address, uint8_t write);
void I2C_BUS0_Stop(void);
void I2C_BUS0_Write(uint8_t value);
uint8_t I2C_BUS0_Read(void);
uint8_t I2C_BUS0_LastRead(void);

void I2C_BUS1_Init(void);
uint8_t I2C_BUS1_CheckAddressPresence(uint8_t slave_address);
void I2C_BUS1_Start(uint8_t slave_address, uint8_t write);
void I2C_BUS1_Continue(uint8_t slave_address, uint8_t write);
void I2C_BUS1_Stop(void);
void I2C_BUS1_Write(uint8_t value);
uint8_t I2C_BUS1_Read(void);
uint8_t I2C_BUS1_LastRead(void);

#endif
