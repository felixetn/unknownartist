/*
 * SLAM_Algorithm_rtwutil.c
 *
 * Code generation for function 'SLAM_Algorithm_rtwutil'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "SLAM_Algorithm.h"
#include "SLAM_Algorithm_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static void mul_wide_s32(int32_t in0, int32_t in1, uint32_t *ptrOutBitsHi,
  uint32_t *ptrOutBitsLo);

/* Function Definitions */
static void mul_wide_s32(int32_t in0, int32_t in1, uint32_t *ptrOutBitsHi,
  uint32_t *ptrOutBitsLo)
{
  uint32_t absIn0;
  uint32_t absIn1;
  int16_t negativeProduct;
  uint16_t in0Hi;
  uint16_t in0Lo;
  uint16_t in1Hi;
  uint16_t in1Lo;
  uint32_t productLoHi;
  uint32_t productLoLo;
  int16_t carry;
  uint32_t outBitsLo;
  absIn0 = (uint32_t)(in0 < 0L ? -in0 : in0);
  absIn1 = (uint32_t)(in1 < 0L ? -in1 : in1);
  negativeProduct = !(in0 == 0L || (in1 == 0L || in0 > 0L == in1 > 0L));
  in0Hi = (uint16_t)(absIn0 >> 16);
  in0Lo = (uint16_t)absIn0 & MAX_uint16_t;
  in1Hi = (uint16_t)(absIn1 >> 16);
  in1Lo = (uint16_t)absIn1 & MAX_uint16_t;
  absIn0 = (uint32_t)in0Hi * (uint32_t)in1Hi;
  absIn1 = (uint32_t)in0Hi * (uint32_t)in1Lo;
  productLoHi = (uint32_t)in0Lo * (uint32_t)in1Hi;
  productLoLo = (uint32_t)in0Lo * (uint32_t)in1Lo;
  carry = 0;
  outBitsLo = productLoLo + (productLoHi << 16UL);
  if (outBitsLo < productLoLo) {
    carry = 1;
  }

  productLoLo = outBitsLo;
  outBitsLo += absIn1 << 16UL;
  if (outBitsLo < productLoLo) {
    carry++;
  }

  absIn0 = (uint32_t)carry + absIn0 + (productLoHi >> 16) + (absIn1 >> 16);
  if (negativeProduct) {
    absIn0 = ~absIn0;
    outBitsLo = ~outBitsLo;
    outBitsLo++;
    if (outBitsLo == 0UL) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = outBitsLo;
}

int32_t mul_s32_s32_s32_sat(int32_t a, int32_t b)
{
  int32_t result;
  uint32_t u32_clo;
  uint32_t u32_chi;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  if ((int32_t)u32_chi > 0L || u32_chi == 0UL && u32_clo >= 2147483648UL) {
    result = MAX_int32_t;
  } else if ((int32_t)u32_chi < -1L || (int32_t)u32_chi == -1L && u32_clo <
             2147483648UL) {
    result = MIN_int32_t;
  } else {
    result = (int32_t)u32_clo;
  }

  return result;
}

/* End of code generation (SLAM_Algorithm_rtwutil.c) */
