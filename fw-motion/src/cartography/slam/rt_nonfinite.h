/*
 * rt_nonfinite.h
 *
 * Code generation for function 'SLAM_Algorithm'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

#ifndef __RT_NONFINITE_H__
#define __RT_NONFINITE_H__

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#include <float.h>
#endif
#include <stddef.h>
#include "rtwtypes.h"

extern double rtInf;
extern double rtMinusInf;
extern double rtNaN;
extern double rtInfF;
extern double rtMinusInfF;
extern double rtNaNF;
extern void rt_InitInfAndNaN(size_t realSize);
extern boolean_t rtIsInf(double value);
extern boolean_t rtIsInfF(double value);
extern boolean_t rtIsNaN(double value);
extern boolean_t rtIsNaNF(double value);

typedef struct {
  struct {
    uint32_t wordH;
    uint32_t wordL;
  } words;
} BigEndianIEEEDouble;

typedef struct {
  struct {
    uint32_t wordL;
    uint32_t wordH;
  } words;
} LittleEndianIEEEDouble;

typedef struct {
  union {
    double wordLreal;
    uint32_t wordLuint;
  } wordL;
} IEEESingle;

#endif
/* End of code generation (rt_nonfinite.h) */
