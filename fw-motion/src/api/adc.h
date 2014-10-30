/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file adc.h
 * @author Team Highway
 * @brief Interface for the ADC module for the line sensor.
 **/

#ifndef ADC_H_
#define ADC_H_

#include "base_type.h"

void ADC_Init(void);

int16_t ADC_GetValue(uint8_t pin);

#define ADC_USE_AN00 0
#define ADC_USE_AN01 0
#define ADC_USE_AN02 0
#define ADC_USE_AN03 0
#define ADC_USE_AN04 0
#define ADC_USE_AN05 0
#define ADC_USE_AN06 0
#define ADC_USE_AN07 0

#define ADC_USE_AN08 0
#define ADC_USE_AN09 0
#define ADC_USE_AN10 1 //0 ; changed to measure battery voltage
#define ADC_USE_AN11 1
#define ADC_USE_AN12 1
#define ADC_USE_AN13 1
#define ADC_USE_AN14 1
#define ADC_USE_AN15 1

#define ADC_USE_AN16 0
#define ADC_USE_AN17 0
#define ADC_USE_AN18 0
#define ADC_USE_AN19 0
#define ADC_USE_AN20 0
#define ADC_USE_AN21 0
#define ADC_USE_AN22 0
#define ADC_USE_AN23 0

#endif /* ADC_H_ */
