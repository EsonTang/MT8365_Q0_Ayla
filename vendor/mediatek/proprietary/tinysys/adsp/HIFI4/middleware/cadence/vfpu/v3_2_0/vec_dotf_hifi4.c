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
DISCARD_FUN(float32_t,vec_dotf, (const float32_t * restrict x,const float32_t * restrict y,int N))
#else


/*===========================================================================
  Vector matematics:
  vec_dot              Vector Dot Product
===========================================================================*/
/*-------------------------------------------------------------------------
  Vector Dot product
  Two versions of routines are available: regular versions (without suffix _fast)
  that work with arbitrary arguments, faster versions 
  (with suffix _fast) apply some restrictions.

  Precision: 
  64x32  64x32-bit data, 64-bit output (fractional multiply Q63xQ31->Q63)
  64x64  64x64-bit data, 64-bit output (fractional multiply Q63xQ63->Q63)
  64x64i 64x64-bit data, 64-bit output (low 64 bit of integer multiply)
  32x32  32x32-bit data, 64-bit output
  32x16  32x16-bit data, 64-bit output
  16x16  16x16-bit data, 64-bit output for regular version and 32-bit for 
                        fast version
  f      single precision floating point

  Input:
  x[N]  input data, Q31 or floating point
  y[N]  input data, Q31, Q15, or floating point
  N	length of vectors
  Returns: dot product of all data pairs, Q31 or Q63

  Restrictions:
  Regular versions (without suffix _fast):
  None
  Faster versions (with suffix _fast):
  x,y - aligned on 8-byte boundary
  N   - multiple of 4

  vec_dot16x16_fast utilizes 32-bit saturating accumulator, so, input data 
  should be scaled properly to avoid erroneous results
-------------------------------------------------------------------------*/
#define sz_f32    (int)sizeof(float32_t)
float32_t vec_dotf   (const float32_t * restrict x,const float32_t * restrict y,int N)

{
  int n;

  xtfloatx2 vxf, vyf, vacc;
  xtfloat xf, yf, zf, acc;
  const xtfloatx2 * restrict px = (const xtfloatx2 *)x;
  const xtfloatx2 * restrict py = (const xtfloatx2 *)y;
  ae_valign y_align;
  NASSERT(x);
  NASSERT(y);
  if (N <= 0) return 0;
  vacc = XT_MOV_SX2(0.f);
  zf = XT_MOV_S(0.f);

  if ((((uintptr_t)(x)) & 7))
  {
    ae_int32x2 tmp;
    xf = XT_LSI((const xtfloat *)px, 0);
    yf = XT_LSI((const xtfloat *)py, 0);
    AE_L32_IP(tmp, castxcc(ae_int32, px), sz_f32);
    AE_L32_IP(tmp, castxcc(ae_int32, py), sz_f32);
    zf = XT_MUL_S(xf, yf);
    N--;
  }
  y_align = AE_LA64_PP(py);
  for (n = 0; n<(N>>1); n ++)
  {
    XT_LSX2IP(vxf, px, 2 * sz_f32);
    XT_LASX2IP(vyf, y_align, py);
    vxf = XT_SEL32_HL_SX2(vxf, vxf);
    XT_MADD_SX2(vacc, vxf, vyf);
  }
  if (N & 1)
  {
    xf = XT_LSI((const xtfloat *)px, 0);
    yf = XT_LSI((const xtfloat *)py, 0);
    XT_MADD_S(zf, xf, yf);
  }
  acc = XT_RADD_SX2(vacc);
  acc = XT_ADD_S(acc, zf);

  return acc;
} /* vec_dotf() */
#endif
