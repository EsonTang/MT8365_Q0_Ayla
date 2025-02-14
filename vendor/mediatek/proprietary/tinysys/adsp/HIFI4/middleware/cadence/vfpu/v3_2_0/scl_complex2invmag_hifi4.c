/*
 *    Mediatek HiFi 4 Redistribution Version  <0.0.1>
 */

/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_HIFI4_VFPU)
DISCARD_FUN(float32_t ,scl_complex2invmag ,(complex_float x))
#else
/*===========================================================================
  Scalar matematics:
  scl_complex2invmag             Complex magnitude (reciprocal) 
===========================================================================*/
/*-------------------------------------------------------------------------
  Complex magnitude
  routines compute complex magnitude or its reciprocal

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input complex data
  N     length of vector

  Restriction:
  none
-------------------------------------------------------------------------*/
float32_t  scl_complex2invmag (complex_float x)
{
  /*
  * float32_t x_re,x_im;
  * float32_t mnt_re, mnt_im, mnt_abs;
  * int exp_re, exp_im, exp_abs;
  * const int minexp = FLT_MIN_EXP - FLT_MANT_DIG;
  * 
  * x_re = fabsf( crealf(x) );
  * x_im = fabsf( cimagf(x) );
  * 
  * exp_re = ( x_re != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_re) ) : minexp );
  * exp_im = ( x_im != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_im) ) : minexp );
  * 
  * exp_abs = ( exp_re > exp_im ? exp_re : exp_im );
  * 
  * mnt_re = STDLIB_MATH(ldexpf)( x_re, -exp_abs );
  * mnt_im = STDLIB_MATH(ldexpf)( x_im, -exp_abs );
  * 
  * mnt_abs = 1.f/sqrtf( mnt_re*mnt_re + mnt_im*mnt_im );
  * 
  * return STDLIB_MATH(ldexpf)( mnt_abs, -exp_abs );
  * 
  */
  xtfloatx2 x0, x1, y0;
  complex_float x_tmp;
  xtfloat xf;
  ae_int32x2 t0, nsa, u0;
  ae_int32x2 e0, e1;
  ae_int32x2 nsa0;
  xtbool2 b0, b1;
  x_tmp = x;
  x0 = XT_LSX2I((xtfloatx2 *)&x_tmp, 0);
  x0 = XT_ABS_SX2(x0);

  t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(x0);

  u0 = AE_SRLI32(t0, 23);
  u0 = AE_AND32(u0, 0xFF);
  e0 = AE_SUB32(u0, 127);
  e1 = AE_SEL32_LH(e0, e0);
  nsa = AE_MAX32(e0, e1);
  nsa = AE_MIN32(nsa, 127);
 
  e0 = AE_SUB32(127, nsa);
  b0 = AE_EQ32(e0, 0);
  nsa0 = AE_SLLI32(e0, 23);
  AE_MOVT32X2(nsa0, 0x00400000, b0);
  y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
  x0 = XT_MUL_SX2(x0, y0);
  x0 = XT_MUL_SX2(x0, x0);
  x1 = XT_SEL32_LH_SX2(x0, x0);
  x0 = XT_ADD_SX2(x0, x1);

  y0 = XT_RSQRT_SX2(x0);
  b0 = XT_OEQ_SX2(x0, 0.0f);
  b1 = XT_OEQ_SX2(x0, XT_AE_MOVXTFLOATX2_FROMINT32X2(0x7f800000));
  t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(y0);
  AE_MOVT32X2(t0, 0x7f800000, b0);
  AE_MOVT32X2(t0, 0x0, b1);
  y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(t0);
  x0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
  y0 = XT_MUL_SX2(y0, x0);
  xf = XT_LOW_S(y0);
  return xf;
} /* scl_complex2invmag() */
#endif
