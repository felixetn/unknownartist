/*
 * SLAM_Algorithm.c
 *
 * Code generation for function 'SLAM_Algorithm'
 *
 * C source code generated on: Tue Feb 26 09:50:36 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "SLAM_Algorithm.h"
#include "mpower.h"
#include "mrdivide.h"
#include "sign.h"
#include "SLAM_Algorithm_rtwutil.h"
#include "SLAM_Algorithm_data.h"
#include "../../api/api.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  double d1;
  double d2;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d1 = fabs(u0);
    d2 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d1 == 1.0) {
        y = rtNaN;
      } else if (d1 > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d2 == 0.0) {
      y = 1.0;
    } else if (d2 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if (u1 == 0.5 && u0 >= 0.0) {
      y = sqrt(u0);
    } else if (u0 < 0.0 && u1 > floor(u1)) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/*
 * function [ E0 ] = SLAM_Algorithm( x,y,theta,u,dist )
 */
void SLAM_Algorithm(int32_t x, int32_t y, uint16_t theta, double u, int32_t dist,
                    double E0[3])
{
#ifdef SLAM_SCENARIO
  double A[9];
  double d0;
  uint16_t u0;
  uint32_t bku;
  int32_t i0;
  int16_t i;
  static const int8_t iv0[3] = { 0, 0, 1 };

  uint16_t left = 0, right = 0, front = 0;

  double b_A[9];
  int16_t i1;
  int16_t i2;
  double Pk[9];
  double b;
  int32_t q0;
  int32_t q1;
  int32_t qY;
  int32_t hx[3];
  int32_t ak;
  int32_t c;
  int32_t exitg11;
  int32_t exitg10;
  int32_t Hk[9];
  int32_t exitg9;
  int32_t exitg8;
  int32_t b_c;
  int32_t exitg7;
  int32_t c_c;
  int32_t exitg6;
  int32_t d_c;
  int32_t exitg5;
  int32_t e_c;
  int32_t exitg4;
  int32_t f_c;
  int32_t exitg3;
  int32_t g_c;
  int32_t exitg2;
  int32_t h_c;
  int32_t exitg1;
  double b_Pk[9];
  double b_Hk[9];
  double dv0[3];
  double c_A[3];
  int8_t I[9];

  /* 'SLAM_Algorithm:3' coder.inline('never'); */
  /* Pk0 = [0.001 0 0; 0 0.001 0; 0 0 0.002]; */
  /* zold = [z1;z2;z3]; */
  /* 'SLAM_Algorithm:9' E1 = [double(x + int32(cos(double((uint16(u)+theta)*pi/180))*dist)); double(y + int32(sin(double((uint16(u)+theta)*pi/180))*dist)); double(theta + uint16(u))]; */
  /* 'SLAM_Algorithm:11' A = [1 0 double(-sin(double((uint16(u)+theta)*pi/180))*dist); 0 1 double(cos(double((uint16(u)+theta)*pi/180))*dist); 0 0 1]; */
  A[0] = 1.0;
  A[3] = 0.0;
  d0 = u;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 65536.0) {
    if (d0 >= 0.0) {
      u0 = (uint16_t)d0;
    } else {
      u0 = 0U;
    }
  } else if (d0 >= 65536.0) {
    u0 = MAX_uint16_t;
  } else {
    u0 = 0U;
  }

  bku = (uint32_t)u0 + (uint32_t)theta;
  if ((int32_t)bku > 65535L) {
    bku = 65535UL;
  }

  d0 = (double)bku * 3.1415926535897931;
  d0 = floor(d0 + 0.5);
  i0 = (int32_t)d0;
  if (i0 < 65536L) {
    u0 = (uint16_t)i0;
  } else {
    u0 = MAX_uint16_t;
  }

  d0 = (double)u0 / 180.0;
  d0 = floor(d0 + 0.5);
  i = (int16_t)d0;
  u0 = (uint16_t)i;
  d0 = -sin((double)u0) * (double)dist;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 2.147483648E+9) {
    i0 = (int32_t)d0;
  } else {
    i0 = MAX_int32_t;
  }

  A[6] = (double)i0;
  A[1] = 0.0;
  A[4] = 1.0;
  d0 = u;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 65536.0) {
    if (d0 >= 0.0) {
      u0 = (uint16_t)d0;
    } else {
      u0 = 0U;
    }
  } else if (d0 >= 65536.0) {
    u0 = MAX_uint16_t;
  } else {
    u0 = 0U;
  }

  bku = (uint32_t)u0 + (uint32_t)theta;
  if ((int32_t)bku > 65535L) {
    bku = 65535UL;
  }

  d0 = (double)bku * 3.1415926535897931;
  d0 = floor(d0 + 0.5);
  i0 = (int32_t)d0;
  if (i0 < 65536L) {
    u0 = (uint16_t)i0;
  } else {
    u0 = MAX_uint16_t;
  }

  d0 = (double)u0 / 180.0;
  d0 = floor(d0 + 0.5);
  i = (int16_t)d0;
  u0 = (uint16_t)i;
  d0 = cos((double)u0) * (double)dist;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 2.147483648E+9) {
    i0 = (int32_t)d0;
  } else {
    i0 = MAX_int32_t;
  }

  A[7] = (double)i0;
  for (i = 0; i < 3; i++) {
    A[2 + 3 * i] = (double)iv0[i];
  }

  /* 'SLAM_Algorithm:13' Pk = A*Pk0*transpose(A); */
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      b_A[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        b_A[i + 3 * i1] += A[i + 3 * i2] * Pk0[i2 + 3 * i1];
      }
    }

    for (i1 = 0; i1 < 3; i1++) {
      Pk[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        Pk[i + 3 * i1] += b_A[i + 3 * i2] * A[i1 + 3 * i2];
      }
    }
  }

  /* 'SLAM_Algorithm:15' b = sign(sin(double(u*pi/180)))*sqrt(double((dist/cos(double(u*pi/180)))^2-(dist)^2)); */
  d0 = sin(mrdivide(u * 3.1415926535897931, 180.0));
  b_sign(&d0);
  b = (double)dist / cos(mrdivide(u * 3.1415926535897931, 180.0));
  b = b < 0.0 ? ceil(b - 0.5) : floor(b + 0.5);
  if (b < 2.147483648E+9) {
    if (b >= -2.147483648E+9) {
      i0 = (int32_t)b;
    } else {
      i0 = MIN_int32_t;
    }
  } else if (b >= 2.147483648E+9) {
    i0 = MAX_int32_t;
  } else {
    i0 = 0L;
  }

  q0 = mpower(i0);
  q1 = mpower(dist);
  qY = q0 - q1;
  if (q0 < 0L && (q1 >= 0L && qY >= 0L)) {
    qY = MIN_int32_t;
  } else {
    if (q0 >= 0L && (q1 < 0L && qY < 0L)) {
      qY = MAX_int32_t;
    }
  }

  b = d0 * sqrt((double)qY);

  /* 'SLAM_Algorithm:17' if (u == 0) */
  if (u == 0.0) {
    /* 'SLAM_Algorithm:18' hx = [sin(double((u+90)*pi/180))*zold(1) - b; */
    /* 'SLAM_Algorithm:19'         (sin(double((u-90)*pi/180))*zold(2) - b)*(-1); */
    /* 'SLAM_Algorithm:20'         zold(3) - (cos(double(u*pi/180))*dist - sqrt(double((dist/cos(double(u*pi/180)))^2-(dist)^2))) */
    /* 'SLAM_Algorithm:21'         ]; */
    d0 = sin(mrdivide(282.74333882308139, 180.0)) * zold[0] - b;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    hx[0] = i0;
    d0 = -(sin(mrdivide(-282.74333882308139, 180.0)) * zold[1] - b);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    hx[1] = i0;
    d0 = (double)dist / cos(mrdivide(0.0, 180.0));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    d0 = cos(mrdivide(0.0, 180.0)) * (double)dist;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        ak = (int32_t)d0;
      } else {
        ak = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      ak = MAX_int32_t;
    } else {
      ak = 0L;
    }

    q0 = mpower(i0);
    q1 = mpower(dist);
    qY = q0 - q1;
    if (q0 < 0L && (q1 >= 0L && qY >= 0L)) {
      qY = MIN_int32_t;
    } else {
      if (q0 >= 0L && (q1 < 0L && qY < 0L)) {
        qY = MAX_int32_t;
      }
    }

    d0 = (double)ak - sqrt((double)qY);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    d0 = zold[2] - (double)i0;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    hx[2] = i0;
  } else {
    /* 'SLAM_Algorithm:22' else */
    /* 'SLAM_Algorithm:23' hx = [sin(double((u+90))*pi/180)*zold(1) - b; */
    /* 'SLAM_Algorithm:24'         (sin(double((u-90)*pi/180))*zold(2) - b)*(-1); */
    /* 'SLAM_Algorithm:25'         (cos(double(u*pi/180))*dist - sqrt(double((dist/cos(double(u*pi/180)))^2-(dist)^2))) */
    /* 'SLAM_Algorithm:26'         ]; */
    d0 = (double)dist / cos(mrdivide(u * 3.1415926535897931, 180.0));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        ak = (int32_t)d0;
      } else {
        ak = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      ak = MAX_int32_t;
    } else {
      ak = 0L;
    }

    c = 1L;
    bku = 2UL;
    do {
      exitg11 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        c = mul_s32_s32_s32_sat(ak, c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg11 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg11 == 0UL);

    ak = dist;
    q1 = 1L;
    bku = 2UL;
    do {
      exitg10 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        q1 = mul_s32_s32_s32_sat(ak, q1);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg10 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg10 == 0UL);

    d0 = sin(mrdivide((u + 90.0) * 3.1415926535897931, 180.0)) * zold[0] - b;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    hx[0] = i0;
    d0 = -(sin(mrdivide((u - 90.0) * 3.1415926535897931, 180.0)) * zold[1] - b);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    hx[1] = i0;
    d0 = cos(mrdivide(u * 3.1415926535897931, 180.0)) * (double)dist;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    qY = c - q1;
    if (c < 0L && (q1 >= 0L && qY >= 0L)) {
      qY = MIN_int32_t;
    } else {
      if (c >= 0L && (q1 < 0L && qY < 0L)) {
        qY = MAX_int32_t;
      }
    }

    d0 = (double)i0 - sqrt((double)qY);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    hx[2] = i0;
  }

  /* 'SLAM_Algorithm:29' z = [20;20;20]; */
  /* 'SLAM_Algorithm:30' zold = z; */
  for (i = 0; i < 3; i++) {
    zold[i] = 20.0;
  }

  /* 'SLAM_Algorithm:32' if (u == 0) */
  if (u == 0.0) {
    /* 'SLAM_Algorithm:33' Hk = [ 0 0 cos(double((u+90)*pi/180))*zold(1) - (dist^2)*abs(sin(double(u*pi/180)))/cos(double(u*pi/180))^3 */
    /* 'SLAM_Algorithm:34'            0 0 (-cos(double((u-90)*pi/180))*zold(1)+(dist^2)*abs(sin(double(u*pi/180)))/cos(double(u*pi/180))^3) */
    /* 'SLAM_Algorithm:35'            0 0 -(dist^2)*sin(double(u*pi/180))/(cos(double(u*pi/180))^3)-dist*sin(double(u*pi/180))]; */
    Hk[0] = 0L;
    Hk[3] = 0L;
    d0 = 6.123233995736766E-17 * zold[0];
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    Hk[6] = i0;
    Hk[1] = 0L;
    Hk[4] = 0L;
    d0 = -6.123233995736766E-17 * zold[0];
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    Hk[7] = i0;
    Hk[2] = 0L;
    Hk[5] = 0L;
    Hk[8] = 0L;
  } else {
    /* 'SLAM_Algorithm:36' else */
    /* 'SLAM_Algorithm:37' Hk = [ 0 0 cos(double((u+90)*pi/180))*zold(1) - (dist^2)*abs(sin(double(u*pi/180)))/(sqrt(double((dist^2)/(cos(double(u*pi/180))^2)-dist^2)*cos(double(u*pi/180))^3)) */
    /* 'SLAM_Algorithm:38'            0 0 (-cos(double((u-90)*pi/180))*zold(1)+(dist^2)*abs(sin(double(u*pi/180)))/(sqrt(double((dist^2)/(cos(double(u*pi/180))^2)-dist^2)*cos(double(u*pi/180)^3)))) */
    /* 'SLAM_Algorithm:39'            0 0 -(dist^2)*sin(double(u*pi/180))/(sqrt(double((dist^2)/(cos(double(u*pi/180))^2)-dist^2)*cos(double(u*pi/180))^3))-dist*sin(double(u*pi/180))]; */
    ak = dist;
    c = 1L;
    bku = 2UL;
    do {
      exitg9 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        c = mul_s32_s32_s32_sat(ak, c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg9 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg9 == 0UL);

    ak = dist;
    q1 = 1L;
    bku = 2UL;
    do {
      exitg8 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        q1 = mul_s32_s32_s32_sat(ak, q1);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg8 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg8 == 0UL);

    ak = dist;
    b_c = 1L;
    bku = 2UL;
    do {
      exitg7 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        b_c = mul_s32_s32_s32_sat(ak, b_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg7 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg7 == 0UL);

    ak = dist;
    c_c = 1L;
    bku = 2UL;
    do {
      exitg6 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        c_c = mul_s32_s32_s32_sat(ak, c_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg6 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg6 == 0UL);

    ak = dist;
    d_c = 1L;
    bku = 2UL;
    do {
      exitg5 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        d_c = mul_s32_s32_s32_sat(ak, d_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg5 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg5 == 0UL);

    ak = dist;
    e_c = 1L;
    bku = 2UL;
    do {
      exitg4 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        e_c = mul_s32_s32_s32_sat(ak, e_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg4 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg4 == 0UL);

    ak = dist;
    f_c = 1L;
    bku = 2UL;
    do {
      exitg3 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        f_c = mul_s32_s32_s32_sat(ak, f_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg3 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg3 == 0UL);

    ak = dist;
    g_c = 1L;
    bku = 2UL;
    do {
      exitg2 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        g_c = mul_s32_s32_s32_sat(ak, g_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg2 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg2 == 0UL);

    ak = dist;
    h_c = 1L;
    bku = 2UL;
    do {
      exitg1 = 0UL;
      if ((uint16_t)((int16_t)bku & 1) != 0U) {
        h_c = mul_s32_s32_s32_sat(ak, h_c);
      }

      bku >>= 1;
      if ((int32_t)bku == 0L) {
        exitg1 = 1UL;
      } else {
        ak = mul_s32_s32_s32_sat(ak, ak);
      }
    } while (exitg1 == 0UL);

    Hk[0] = 0L;
    Hk[3] = 0L;
    d0 = (double)q1 / rt_powd_snf(cos(u * 3.1415926535897931 / 180.0), 2.0);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        q0 = (int32_t)d0;
      } else {
        q0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      q0 = MAX_int32_t;
    } else {
      q0 = 0L;
    }

    d0 = (double)c * fabs(sin(u * 3.1415926535897931 / 180.0));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    qY = q0 - b_c;
    if (q0 < 0L && (b_c >= 0L && qY >= 0L)) {
      qY = MIN_int32_t;
    } else {
      if (q0 >= 0L && (b_c < 0L && qY < 0L)) {
        qY = MAX_int32_t;
      }
    }

    d0 = (double)i0 / sqrt((double)qY * rt_powd_snf(cos(u * 3.1415926535897931 /
      180.0), 3.0));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    d0 = cos((u + 90.0) * 3.1415926535897931 / 180.0) * zold[0] - (double)i0;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    Hk[6] = i0;
    Hk[1] = 0L;
    Hk[4] = 0L;
    d0 = (double)d_c / rt_powd_snf(cos(u * 3.1415926535897931 / 180.0), 2.0);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        q0 = (int32_t)d0;
      } else {
        q0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      q0 = MAX_int32_t;
    } else {
      q0 = 0L;
    }

    d0 = (double)c_c * fabs(sin(u * 3.1415926535897931 / 180.0));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    qY = q0 - e_c;
    if (q0 < 0L && (e_c >= 0L && qY >= 0L)) {
      qY = MIN_int32_t;
    } else {
      if (q0 >= 0L && (e_c < 0L && qY < 0L)) {
        qY = MAX_int32_t;
      }
    }

    d0 = (double)i0 / sqrt((double)qY * cos(rt_powd_snf(u * 3.1415926535897931 /
      180.0, 3.0)));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    d0 = -cos((u - 90.0) * 3.1415926535897931 / 180.0) * zold[0] + (double)i0;
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    Hk[7] = i0;
    Hk[2] = 0L;
    Hk[5] = 0L;
    if (f_c <= MIN_int32_t) {
      ak = MAX_int32_t;
    } else {
      ak = -f_c;
    }

    d0 = (double)dist * sin(u * 3.1415926535897931 / 180.0);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        q1 = (int32_t)d0;
      } else {
        q1 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      q1 = MAX_int32_t;
    } else {
      q1 = 0L;
    }

    d0 = (double)g_c / rt_powd_snf(cos(u * 3.1415926535897931 / 180.0), 2.0);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        q0 = (int32_t)d0;
      } else {
        q0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      q0 = MAX_int32_t;
    } else {
      q0 = 0L;
    }

    d0 = (double)ak * sin(u * 3.1415926535897931 / 180.0);
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        i0 = (int32_t)d0;
      } else {
        i0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      i0 = MAX_int32_t;
    } else {
      i0 = 0L;
    }

    qY = q0 - h_c;
    if (q0 < 0L && (h_c >= 0L && qY >= 0L)) {
      qY = MIN_int32_t;
    } else {
      if (q0 >= 0L && (h_c < 0L && qY < 0L)) {
        qY = MAX_int32_t;
      }
    }

    d0 = (double)i0 / sqrt((double)qY * rt_powd_snf(cos(u * 3.1415926535897931 /
      180.0), 3.0));
    d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
    if (d0 < 2.147483648E+9) {
      if (d0 >= -2.147483648E+9) {
        q0 = (int32_t)d0;
      } else {
        q0 = MIN_int32_t;
      }
    } else if (d0 >= 2.147483648E+9) {
      q0 = MAX_int32_t;
    } else {
      q0 = 0L;
    }

    qY = q0 - q1;
    if (q0 < 0L && (q1 >= 0L && qY >= 0L)) {
      qY = MIN_int32_t;
    } else {
      if (q0 >= 0L && (q1 < 0L && qY < 0L)) {
        qY = MAX_int32_t;
      }
    }

    Hk[8] = qY;
  }

  /* 'SLAM_Algorithm:42' Kk = Pk*double(transpose(Hk))*double((double(Hk)*Pk*transpose(double(Hk)))'); */
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      b_A[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        b_A[i + 3 * i1] += (double)Hk[i + 3 * i2] * Pk[i2 + 3 * i1];
      }

      b_Pk[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        b_Pk[i + 3 * i1] += Pk[i + 3 * i2] * (double)Hk[i1 + 3 * i2];
      }
    }
  }

  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      b_Hk[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        b_Hk[i + 3 * i1] += b_A[i1 + 3 * i2] * (double)Hk[i + 3 * i2];
      }
    }
  }

  /* 'SLAM_Algorithm:43' E0 = E1 + Kk*(z-double(hx)); */
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      A[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        A[i + 3 * i1] += b_Pk[i + 3 * i2] * b_Hk[i2 + 3 * i1];
      }
    }

    left = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
    front = us_getSensorMapData(FRONT_SENSOR).Front_Distance;
    right = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;

    if (i0 == 0){
    	if (left < 580)
    		dv0[i] = left - (double)hx[i];
    	else dv0[i] = 0;
    }
    if (i0 == 1){
    	if (right < 580)
    		dv0[i] = right - (double)hx[i];
    	else dv0[i] = 0;
    }
    if (i0 == 2){
    	if (front < 580)
    		dv0[i] = front - (double)hx[i];
    	else dv0[i] = 0;
    }
  }

  d0 = u;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 65536.0) {
    if (d0 >= 0.0) {
      u0 = (uint16_t)d0;
    } else {
      u0 = 0U;
    }
  } else if (d0 >= 65536.0) {
    u0 = MAX_uint16_t;
  } else {
    u0 = 0U;
  }

  bku = (uint32_t)u0 + (uint32_t)theta;
  if ((int32_t)bku > 65535L) {
    bku = 65535UL;
  }

  d0 = (double)bku * 3.1415926535897931;
  d0 = floor(d0 + 0.5);
  i0 = (int32_t)d0;
  if (i0 < 65536L) {
    u0 = (uint16_t)i0;
  } else {
    u0 = MAX_uint16_t;
  }

  d0 = (double)u0 / 180.0;
  d0 = floor(d0 + 0.5);
  i = (int16_t)d0;
  u0 = (uint16_t)i;
  d0 = cos((double)u0) * (double)dist;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 2.147483648E+9) {
    q1 = (int32_t)d0;
  } else {
    q1 = MAX_int32_t;
  }

  qY = x + q1;
  if (x < 0L && (q1 < 0L && qY >= 0L)) {
    qY = MIN_int32_t;
  } else {
    if (x > 0L && (q1 > 0L && qY <= 0L)) {
      qY = MAX_int32_t;
    }
  }

  hx[0L] = qY;
  d0 = u;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 65536.0) {
    if (d0 >= 0.0) {
      u0 = (uint16_t)d0;
    } else {
      u0 = 0U;
    }
  } else if (d0 >= 65536.0) {
    u0 = MAX_uint16_t;
  } else {
    u0 = 0U;
  }

  bku = (uint32_t)u0 + (uint32_t)theta;
  if ((int32_t)bku > 65535L) {
    bku = 65535UL;
  }

  d0 = (double)bku * 3.1415926535897931;
  d0 = floor(d0 + 0.5);
  i0 = (int32_t)d0;
  if (i0 < 65536L) {
    u0 = (uint16_t)i0;
  } else {
    u0 = MAX_uint16_t;
  }

  d0 = (double)u0 / 180.0;
  d0 = floor(d0 + 0.5);
  i = (int16_t)d0;
  u0 = (uint16_t)i;
  d0 = sin((double)u0) * (double)dist;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 2.147483648E+9) {
    q1 = (int32_t)d0;
  } else {
    q1 = MAX_int32_t;
  }

  qY = y + q1;
  if (y < 0L && (q1 < 0L && qY >= 0L)) {
    qY = MIN_int32_t;
  } else {
    if (y > 0L && (q1 > 0L && qY <= 0L)) {
      qY = MAX_int32_t;
    }
  }

  hx[1L] = qY;
  d0 = u;
  d0 = d0 < 0.0 ? ceil(d0 - 0.5) : floor(d0 + 0.5);
  if (d0 < 65536.0) {
    if (d0 >= 0.0) {
      u0 = (uint16_t)d0;
    } else {
      u0 = 0U;
    }
  } else if (d0 >= 65536.0) {
    u0 = MAX_uint16_t;
  } else {
    u0 = 0U;
  }

  bku = (uint32_t)theta + (uint32_t)u0;
  if ((int32_t)bku > 65535L) {
    bku = 65535UL;
  }

  hx[2L] = (int32_t)bku;
  for (i = 0; i < 3; i++) {
    c_A[i] = 0.0;
    for (i1 = 0; i1 < 3; i1++) {
      c_A[i] += A[i + 3 * i1] * dv0[i1];
    }

    E0[i] = (double)hx[i] + c_A[i];
  }

  /* 'SLAM_Algorithm:44' Pk = (eye(3,3)-Kk*double(Hk))*Pk; */
  for (i = 0; i < 9; i++) {
    I[i] = 0;
  }

  for (i = 0; i < 3; i++) {
    I[i + 3 * i] = 1;
  }

  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      d0 = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        d0 += A[i + 3 * i2] * (double)Hk[i2 + 3 * i1];
      }

      b_Pk[i + 3 * i1] = (double)I[i + 3 * i1] - d0;
    }
  }

  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      b_A[i + 3 * i1] = 0.0;
      for (i2 = 0; i2 < 3; i2++) {
        b_A[i + 3 * i1] += b_Pk[i + 3 * i2] * Pk[i2 + 3 * i1];
      }
    }
  }

  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      Pk[i1 + 3 * i] = b_A[i1 + 3 * i];
    }
  }

  zold[0] = us_getSensorMapData(LEFT_SENSOR).Left_Distance;
  zold[1] = us_getSensorMapData(RIGHT_SENSOR).Right_Distance;
  zold[2] = us_getSensorMapData(FRONT_SENSOR).Front_Distance;

  /* 'SLAM_Algorithm:45' Pk0 = Pk; */
  for (i = 0; i < 9; i++) {
    Pk0[i] = Pk[i];
  }
#endif
}

/* End of code generation (SLAM_Algorithm.c) */
