/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file regler.h
 * @author Felix Euteneuer
 * @brief TODO - insert a brief description of that interface
 *
 * Implementation of a PID-Controller.
 **/

#ifndef REGLER_H_
#define REGLER_H_

/*
 * Include files
 */
#include "base_type.h"
/*
 * Global pre-processor symbols/macros ('#define')
 */

/*
 * Global type definitions ('typedef')
 */

/*
 * Global and local variable declarations
 */

double Kp, Ki, Kd;

/*
 * Global and local functions
 */

/**
 * Set Kp (K_proportional)
 * @param percent Servo position (-100 left .. +100 right)
 */
extern void Regler_set_Kp(double);
/**
 * Set Ki (K_integral)
 * @param percent Servo position (-100 left .. +100 right)
 */
extern void Regler_set_Ki(double);
/**
 * Set Kd (K_differential)
 * @param percent Servo position (-100 left .. +100 right)
 */
extern void Regler_set_Kd(double);

extern void Regler_output(int16_t);

extern uint16_t Regler_get_sensor();

extern int16_t Regler_get_fehler();

extern int16_t Regler_get_fdif();

/**
 * Get the output of the PID-Controller
 * @param percent Servo position (-100 left .. +100 right)
 */
extern int16_t Regler_pid(int16_t, int16_t);

#endif /* REGLER_H_ */
