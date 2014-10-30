/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file mysin.h
 * @author TEAM::GPS
 * @brief contains a lookup table of sinus values and functions to get sinus and cosinus
 * 			all values are normed to uint16_t
 *
 **/

#ifndef TRIGONOMETRY_H_
#define TRIGONOMETRY_H_

#include "base_type.h"
#include "math.h"

uint16_t sin_(uint16_t);
uint16_t cos_(uint16_t);

/**
 * sin mit Faktor 100 Multipliziert
 */
int32_t sin_Taylor(int32_t in);

/**
 * cos mit Faktor 100 Multipliziert
 */
int32_t cos_Taylor(int32_t in);

uint16_t AnglePlus(uint16_t angle_in, uint16_t plus);
uint16_t AngleMinus(uint16_t angle_in, uint16_t minus);

/**
 * Convert an integer represented angle (range 0 to 65535) to the corresponding angle in degrees (-180 to +180).
 * @param angle_in Value to convert
 * @return Degrees
 */
int16_t uint16AngleToDegree(uint16_t angle_in);
int16_t uint16DegreeRelativeToX(uint16_t angle_in);
int angleToDegree(int angle_in);
uint16_t DegreeToUint16(int16_t angle_in);

#endif
