#ifndef US_I2C_H_
#define US_I2C_H_

#include "api.h"

#define MODULE_I2C_BASE_ADDRESS	0xE0

#define MODULE_INIT_AMPLIFICATION	0x00 //23
#define MODULE_INIT_RANGE	0x46 //8C

void I2CUs_Systick(void);

extern void I2CUs_Init(void);

#endif /* US_I2C_H_ */
