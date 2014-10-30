/*
 * SLAM_Algorithm_initialize.c
 *
 * Code generation for function 'SLAM_Algorithm_initialize'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "SLAM_Algorithm.h"
#include "SLAM_Algorithm_initialize.h"
#include "SLAM_Algorithm_data.h"
#include "../../api/api.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
static uint32_t Pk0_dirty;
static uint32_t zold_dirty;

/* Function Declarations */

/* Function Definitions */
void SLAM_Algorithm_initialize(void)
{
  int16_t i3;
  static const double dv1[9] = { 0.001, 0.0, 0.0, 0.0, 0.001, 0.0, 0.0, 0.0,
    0.002 };

  for (i3 = 0; i3 < 9; i3++) {
    Pk0[i3] = dv1[i3];
  }

  rt_InitInfAndNaN(8U);
  zold_dirty = 0UL;
//  for (i3 = 0; i3 < 3; i3++) {
//    zold[i3] = 111.0 + 111.0 * (double)i3;
//  }
  zold[0] = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
  zold[1] = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
  zold[2] = us_getSensorMapData(FRONT_SENSOR).Front_Distance;

  Pk0_dirty = 0UL;
}

/* End of code generation (SLAM_Algorithm_initialize.c) */
