#include "us_i2c.h"
#include "us.h"
#include "i2c.h"
#include "printf_stdarg.h"

static uint8_t moduleEnabled1 = 0;
static uint8_t moduleEnabled2 = 0;
static uint8_t moduleEnabled3 = 0;

/*
 * Force the modules to start a measurement vie a broadcast call
 */
static void I2CUs_startMeasurement(void) {
	I2C_BUS0_Start(0, 1); // address broadcast
	I2C_BUS0_Write(0x00); // command register
	I2C_BUS0_Write(0x52); // request result in µs
	I2C_BUS0_Stop();
}

/*
 * Set the amplification and range, see module documentation
 */
static void I2CUs_setConfiguration(uint8_t module, uint8_t amplification, uint8_t range) {
	module = MODULE_I2C_BASE_ADDRESS + (module << 1);
	I2C_BUS0_Start(module, 1);
	I2C_BUS0_Write(0x01); // first config register
	I2C_BUS0_Write(amplification);
	I2C_BUS0_Write(range);
	I2C_BUS0_Stop();
}

static int16_t I2CUs_getMeasurement(uint8_t module, uint8_t echo) {
	uint16_t result;

	if (echo > 16)
		return -1;
	module = MODULE_I2C_BASE_ADDRESS + (module << 1);
	echo = 0x02 + (echo << 1);
	I2C_BUS0_Start(module, 1);
	I2C_BUS0_Write(echo);
	I2C_BUS0_Continue(module, 0);

	result = I2C_BUS0_Read();
	result = (result << 8);
	result |= I2C_BUS0_LastRead();

	//Measurement µs -> mm @unknownartist
	result = result / 6.8 ;

	I2C_BUS0_Stop();
	return result;
}

static uint16_t I2CUs_getSoftwareRevisionAddr(uint8_t addr) {
	uint16_t result;

	I2C_BUS0_Start(addr, 1);
	I2C_BUS0_Write(0x00);
	I2C_BUS0_Continue(addr, 0);
	result = I2C_BUS0_LastRead();
	I2C_BUS0_Stop();
	return result;
}

/*
 * Write new address to module. WARNING!!!: See documentation before use!
 * WARNING: CAN ONLY BE USED WITHIN A THREAD (otherwise os_wait will not work)
 * If successful, the red LED of the module will be on continuously
 */
static void I2CUs_writeNewBusAddress(uint8_t oldAddress, uint8_t newAddress) {
	if (newAddress == oldAddress)
		return;
	if (newAddress < 0xE0)
		return;
	if (newAddress & 0x01) // Bit 0 (read/write) must not be set!
		return;

#ifdef SIDEUS_DEBUG
	printf("Software Revision of Module: %d \r\n",
			(int) I2CUs_getSoftwareRevisionAddr(oldAddress));
#endif
	os_wait(200);
#ifdef SIDEUS_DEBUG
	printf("Starting to set new address - old addr: %x - new addr: %x \r\n",
			(int) oldAddress, (int) newAddress);
#endif
	I2C_BUS0_Start(oldAddress, 1);
	I2C_BUS0_Write(0x00);
	I2C_BUS0_Write(0xA0);
	I2C_BUS0_Stop();
	os_wait(60);

	I2C_BUS0_Start(oldAddress, 1);
	I2C_BUS0_Write(0x00);
	I2C_BUS0_Write(0xAA);
	I2C_BUS0_Stop();
	os_wait(60);

	I2C_BUS0_Start(oldAddress, 1);
	I2C_BUS0_Write(0x00);
	I2C_BUS0_Write(0xA5);
	I2C_BUS0_Stop();
	os_wait(60);

	I2C_BUS0_Start(oldAddress, 1);
	I2C_BUS0_Write(0x00);
	I2C_BUS0_Write(newAddress);
	I2C_BUS0_Stop();
	os_wait(200);
#ifdef SIDEUS_DEBUG
	printf("Finished setting new address! \r\n");
#endif
}

void I2CUs_Init(void) {
	I2C_BUS0_Init();

	moduleEnabled1 = I2C_BUS0_CheckAddressPresence(MODULE_I2C_BASE_ADDRESS + (0 << 1));
	if (moduleEnabled1)
		I2CUs_setConfiguration(0, MODULE_INIT_AMPLIFICATION, MODULE_INIT_RANGE);

	moduleEnabled2 = I2C_BUS0_CheckAddressPresence(MODULE_I2C_BASE_ADDRESS + (1 << 1));
	if (moduleEnabled2)
		I2CUs_setConfiguration(1, MODULE_INIT_AMPLIFICATION, MODULE_INIT_RANGE);

	moduleEnabled3 = I2C_BUS0_CheckAddressPresence(MODULE_I2C_BASE_ADDRESS + (2 << 1));
	if (moduleEnabled3)
		I2CUs_setConfiguration(2, MODULE_INIT_AMPLIFICATION, MODULE_INIT_RANGE);
}

void I2CUs_Systick(void) {
	uint16_t result;

	if (moduleEnabled1 || moduleEnabled2 || moduleEnabled3)
		I2CUs_startMeasurement();

	os_wait(25); // wait for US modules

	//left sensor
	if (moduleEnabled1) {
		result = I2CUs_getMeasurement(0, 0);
		Us_Data.Left_DataValid = TRUE;
		if (result > 0)
			Us_Data.Left_Distance = result;
		else
			Us_Data.Left_Distance = 3000;
	}

	os_wait(5); // To allow other tasks some time, reading measurements is a quite slow process

	//right sensor
	if (moduleEnabled2) {
		result = I2CUs_getMeasurement(1, 0);
		Us_Data.Front_DataValid = TRUE;
		if (result > 0)
			Us_Data.Front_Distance = result;
		else
			Us_Data.Front_Distance = 3000;
	}

	os_wait(5); // To allow other tasks some time, reading measurements is a quite slow process

	//front sensor
	if (moduleEnabled3) {
		result = I2CUs_getMeasurement(2, 0);
		Us_Data.Right_DataValid = TRUE;
		if (result > 0)
			Us_Data.Right_Distance = result;
		else
			Us_Data.Right_Distance = 3000;
	}

	os_wait(5); // To allow other tasks some time, reading measurements is a quite slow process
}

