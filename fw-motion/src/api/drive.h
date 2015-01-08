#ifndef DRIVE_H_
#define DRIVE_H_

#include "base_type.h"

/**
 * Current servo control value (-100 .. +100)
 * !Read only!
 */
extern volatile int8_t Drive_Servo;

/**
 * Current motor control value (-3 .. +3)
 * !Read only!
 */
extern volatile int8_t Drive_Speed;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

/**
 * Initialize PPGs used for motor and servo control
 */
extern void Drive_Init(void);

/**
 * Set motor power level
 * @param power Power level (-5 .. +5)
 */
extern void Drive_SetMotor(int8_t power);

/**
 * Set motor power level and drive for the given distance
 * @param power Power level (-5 .. +5)
 * @param distance Distance to drive in millimeters (internally divided by
 *   25 to get measurement accuracy of roughly 2.5 cm)
 */
extern void Drive_SetMotorForDistance(int8_t power, uint32_t distance);

/**
 * Set servo level
 * @param percent Servo position (-100 left .. +100 right)
 */

extern void Drive_SetServo(int16_t percent);
/**
 * Get the total driven distance since boot in millimeters
 * Komplett neues Idiotenkommentar
 */
extern uint32_t Drive_GetTotalDrivenDistance(void);

#endif /* DRIVE_H_ */
