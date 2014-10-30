/*
 * rtwtypes.h
 *
 * Code generation for function 'code'
 *
 * C source code generated on: Wed Jan  9 09:24:15 2013
 *
 */

#ifndef __RTWTYPES_H__
#define __RTWTYPES_H__
#ifndef TRUE
# define TRUE (1U)
#endif
#ifndef FALSE
# define FALSE (0U)
#endif
#ifndef __TMWTYPES__
#define __TMWTYPES__

#include <limits.h>
#include "../../api/base_type.h"

/*=======================================================================* 
 * Target hardware information
 *   Device type: Generic->MATLAB Host Computer
 *   Number of bits:     char:   8    short:   16    int:  32
 *                       long:  64      native word size:  64
 *   Byte ordering: LittleEndian
 *   Signed integer division rounds to: Zero
 *   Shift right on a signed integer as arithmetic shift: on
 *=======================================================================*/

/*=======================================================================* 
 * Fixed width word size data types:                                     * 
 *   int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     * 
 *   uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   * 
 *   real32_T, real64_T           - 32 and 64 bit floating point numbers * 
 *=======================================================================*/

//typedef signed char int8_T;
//typedef unsigned char uint8_T;
//typedef short int16_T;
//typedef unsigned short uint16_T;
//typedef int int32_T;
//typedef unsigned int uint32_T;
//typedef long int64_T;
//typedef unsigned long uint64_T;
//typedef float real32_T;
//typedef double real64_T;
//
///*===========================================================================*
// * Generic type definitions: real_T, time_T, boolean_T, int_T, uint_T,       *
// *                           ulong_T, char_T and byte_T.                     *
// *===========================================================================*/
//
//typedef double real_T;
//typedef double time_T;
//typedef unsigned char boolean_T;
//typedef int int_T;
//typedef unsigned uint_T;
//typedef unsigned long ulong_T;
//typedef char char_T;
//typedef char_T byte_T;



/*=======================================================================*
 * Min and Max:                                                          *
 *   int8_T, int16_T, int32_T     - signed 8, 16, or 32 bit integers     *
 *   uint8_T, uint16_T, uint32_T  - unsigned 8, 16, or 32 bit integers   *
 *=======================================================================*/

#define MAX_int8_t  	((int8_t)(127))
#define MIN_int8_t  	((int8_t)(-128))
#define MAX_uint8_t 	((uint8_t)(255))
#define MIN_uint8_t 	((uint8_t)(0))
#define MAX_int16_t 	((int16_t)(32767))
#define MIN_int16_t 	((int16_t)(-32768))
#define MAX_uint16_t	((uint16_t)(65535))
#define MIN_uint16_t	((uint16_t)(0))
#define MAX_int32_t 	((int32_t)(2147483647))
#define MIN_int32_t 	((int32_t)(-2147483647-1))
#define MAX_uint32_t	((uint32_t)(0xFFFFFFFFU))
#define MIN_uint32_t	((uint32_t)(0))
#define MAX_int64_t	((int64_t)(9223372036854775807L))
#define MIN_int64_t	((int64_t)(-9223372036854775807L-1L))
#define MAX_uint64_t	((uint64_t)(0xFFFFFFFFFFFFFFFFUL))
#define MIN_uint64_t	((uint64_t)(0UL))

/* Logical type definitions */
#if !defined(__cplusplus) && !defined(__true_false_are_keywords)
#  ifndef false
#   define false (0U)
#  endif
#  ifndef true
#   define true (1U)
#  endif
#endif

/*
 * MATLAB for code generation assumes the code is compiled on a target using a 2's compliment representation
 * for signed integer values.
 */
#if ((SCHAR_MIN + 1) != -SCHAR_MAX)
#error "This code must be compiled using a 2's complement representation for signed integer values"
#endif

/*
 * Maximum length of a MATLAB identifier (function/variable)
 * including the null-termination character. Referenced by
 * rt_logging.c and rt_matrx.c.
 */
#define TMW_NAME_LENGTH_MAX	64

#endif
#endif
/* End of code generation (rtwtypes.h) */
