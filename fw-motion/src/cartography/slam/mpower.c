/*
 * mpower.c
 *
 * Code generation for function 'mpower'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "SLAM_Algorithm.h"
#include "mpower.h"
#include "SLAM_Algorithm_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */

/*
 *
 */
int32_t mpower(int32_t a)
{
  int32_t c;
  int32_t ak;
  uint32_t bku;
  int32_t exitg1;
  ak = a;
  c = 1L;
  bku = 2UL;
  do {
    exitg1 = 0UL;
    if ((uint16_t)((int16_t)bku & 1) != 0U) {
      c = mul_s32_s32_s32_sat(ak, c);
    }

    bku >>= 1;
    if ((int32_t)bku == 0L) {
      exitg1 = 1UL;
    } else {
      ak = mul_s32_s32_s32_sat(ak, ak);
    }
  } while (exitg1 == 0UL);

  return c;
}

/* End of code generation (mpower.c) */
