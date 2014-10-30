/*
 * rtGetNaN.c
 *
 * Code generation for function 'SLAM_Algorithm'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

/*
 * Abstract:
 *       MATLAB for code generation function to initialize non-finite, NaN
 */
#include "rtGetNaN.h"
#define NumBitsPerChar	8U

/* Function: rtGetNaN ==================================================
 * Abstract:
 * Initialize rtNaN needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
double rtGetNaN(void)
{
  size_t bitsPerReal = sizeof(double) * (NumBitsPerChar);
  double nan = 0.0;
  if (bitsPerReal == 32U) {
    nan = rtGetNaNF();
  } else {
  uint16_t one = 1U;
  enum {
    LittleEndian,
    BigEndian
  } machByteOrder = (*((uint8_t *) &one) == 1U) ? LittleEndian : BigEndian;
  switch (machByteOrder) {
   case LittleEndian:
    {
      union {
        LittleEndianIEEEDouble bitVal;
        double fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF80000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      nan = tmpVal.fltVal;
      break;
    }

   case BigEndian:
    {
        union {
          BigEndianIEEEDouble bitVal;
          double fltVal;
        } tmpVal;

        tmpVal.bitVal.words.wordH = 0x7FFFFFFFU;
        tmpVal.bitVal.words.wordL = 0xFFFFFFFFU;
        nan = tmpVal.fltVal;
        break;
      }
    }
  }

  return nan;
}

/* Function: rtGetNaNF ==================================================
 * Abstract:
 * Initialize rtNaNF needed by the generated code.
 * NaN is initialized as non-signaling. Assumes IEEE.
 */
double rtGetNaNF(void)
{
  IEEESingle nanF = { { 0 } };
  uint16_t one = 1U;
  enum {
    LittleEndian,
    BigEndian
  } machByteOrder = (*((uint8_t *) &one) == 1U) ? LittleEndian : BigEndian;
  switch (machByteOrder) {
   case LittleEndian:
    {
      nanF.wordL.wordLuint = 0xFFC00000U;
      break;
    }

   case BigEndian:
    {
      nanF.wordL.wordLuint = 0x7FFFFFFFU;
      break;
    }
  }

  return nanF.wordL.wordLreal;
}

/* End of code generation (rtGetNaN.c) */
