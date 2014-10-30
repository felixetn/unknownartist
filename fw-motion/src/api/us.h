#ifndef US_H_
#define US_H_

#include "api.h"
#include "us_i2c.h"


extern volatile UsSensorData_t Us_Data;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

/**
 * Initializes the UART interface and pins
 */
extern void Us_Init(void);

/**
 * Calibrate the Ultrasonic module to the current sensor data
 * @param module Front (1) or back (2) module or both (0)
 */
extern en_result_t Us_Calibrate(uint8_t module);

/**
 * Set the DDR value for the IOs attached to the US modules
 * @param module Front (1) or back (2) module or both (0)
 * @param ioNr IO number (0 = PG1, 1 = PG2, 2 = P01, 3 = P02)
 * @param ddr Make the IO an output (1) or input (0)
 */
extern en_result_t Us_IO_DDR(uint8_t module, uint8_t ioNr, uint8_t ddr);

/**
 * Control the IOs attached to the US modules
 * @param module Front (1) or back (2) module or both (0)
 * @param ioNr IO number (0 = PG1, 1 = PG2, 2 = P01, 3 = P02)
 * @param value Set output high (1) or low (0)
 */
extern en_result_t Us_IO_Value(uint8_t module, uint8_t ioNr, uint8_t value);

/**
 * Check whether a command is issued which is not yet confirmed
 * @param module Front (1) or back (2) module
 */
extern boolean_t Us_Cmd_Wait(uint8_t module);

//void Us_Systick(void);

#endif /* US_H_ */
