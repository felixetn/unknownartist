/*
 * sign.c
 *
 * Code generation for function 'sign'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "SLAM_Algorithm.h"
#include "sign.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */

/*
 *
 */
void b_sign(double *x)
{
  if (*x > 0.0) {
    *x = 1.0;
  } else if (*x < 0.0) {
    *x = -1.0;
  } else {
    if (*x == 0.0) {
      *x = 0.0;
    }
  }
}

/* End of code generation (sign.c) */
