/******************************************************************************
 /* (C) Fujitsu Microelectronics Europe GmbH                                  */
/*                                                                           */
/* The following software deliverable is intended for and must only be       */
/* used in an evaluation laboratory environment.                             */
/* It is provided without charge and therefore provided on an as-is          */
/* basis, subject to alterations.                                            */
/*                                                                           */
/* The software deliverable is to be used exclusively in connection          */
/* with FME products.                                                        */
/* In the event the software deliverable includes the use of open            */
/* source components, the provisions of the governing open source            */
/* license agreement shall apply with respect to such software               */
/* deliverable.                                                              */
/* FME does not warrant that the deliverables do not infringe any            */
/* third party intellectual property right (IPR). In the event that          */
/* the deliverables infringe a third party IPR it is the sole                */
/* responsibility of the customer to obtain necessary licenses to            */
/* continue the usage of the deliverable.                                    */
/*                                                                           */
/* To the maximum extent permitted by applicable law FME disclaims all       */
/* warranties, whether express or implied, in particular, but not            */
/* limited to, warranties of merchantability and fitness for a               */
/* particular purpose for which the deliverable is not designated.           */
/*                                                                           */
/* To the maximum extent permitted by applicable law, FME�s liability        */
/* is restricted to intention and gross negligence.                          */
/* FME is not liable for consequential damages.                              */
/*                                                                           */
/* (Disclaimer V1.2)                                                         */
/*****************************************************************************/
/** \file util.h
 **
 ** Utility routines
 **
 ** History:
 **   - 2009-12-17  1.00  CIl  First version
 *****************************************************************************/

#ifndef UTIL_H_
#define UTIL_H_

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_type.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/**
 * Calculate the number of elements in an array
 */
#define ARRAY_SIZE(X) ( sizeof(X) / sizeof(*(X)) )

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

/**
 * Calculate the absolute value of an integer.
 * @param integer value
 * @return absolute value of the integer
 */
extern int8_t abs8(int8_t val);
extern int16_t abs16(int16_t val);
extern int32_t abs32(int32_t val);

/**
 * Convert a given integer to its decimal (ASCII) representation
 * @param data Value to convert
 * @param digits Number of decimal digits to format the value to
 * @param fillChar Character used to fill leading zeros
 * @param str Destination string buffer
 * @return Result status
 */
extern en_result_t intToStr(uint32_t data, uint8_t digits, char_t fillChar,
		char_t* str);

/**
 * Wait a number of 'for' loops
 * @param del Number of for loops
 */
extern void wait(uint32_t del);

/**
 * Wait a number of microseconds
 * @param delay_us Number of microseconds
 */
extern void wait1us(uint16_t delay_us);

/**
 * Wait a number of milliseconds
 * @param delay_ms Number of milliseconds
 */
extern void wait1ms(uint16_t delay_ms);

#endif /* UTIL_H_ */
