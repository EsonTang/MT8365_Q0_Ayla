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
#include "atan_table.h"
#include "common.h"

/*===========================================================================
  Vector matematics:
  vec_atan          Arctangent        
===========================================================================*/

/*-------------------------------------------------------------------------
  Arctangent 
  Functions calculate arctangent of number. Fixed point functions 
  scale output to pi so it is always in range -0x20000000 : 0x20000000 
  which corresponds to the real phases +pi/4 and represent input and output 
  in Q31
  NOTE:
  1.  Scalar floating point function is compatible with standard ANSI C
      routines and sets errno and exception flags accordingly

  Accuracy:
  32 bit version: 42    (2.0e-8)
  floating point: 2 ULP

  Precision: 
  32x32  32-bit inputs, 32-bit output.
  f      floating point
 
  Input:
  x[N]   input data, Q31 or floating point
  N      length of vectors
  Output:
  z[N]   result, Q31 or floating point

  Restriction:
  x,z should not overlap

  Scalar versions:
  ----------------
  return result, Q31 or floating point
-------------------------------------------------------------------------*/

#include "common.h"

void vec_atan32x32 (int32_t * restrict z, 
              const int32_t * restrict x, 
              int N )
{
  int n,ofa,ofb;
        ae_int32x2 * restrict pz = (      ae_int32x2 *)z;
  const ae_int32x2 * restrict px = (const ae_int32x2 *)x;
        ae_int32 * py = (      ae_int32 *)atan_table;
        ae_int32 * pq = (      ae_int32 *)atan_table;
  ae_valign x_align, z_align;

  ae_int32x2  vnw,vkw,vlw,vzw,vxw,vdw,vyw,vaw,vbw,vsw,vcw;
  ae_int32x2  vya;
  ae_f32x2    vd1,vd2,vd3;
  ae_f64      vha,vhb;
  ae_int32x2  va0,va1,va2,vb0,vb1,vb2,vxx,vyy,vxt;
  ae_int64    vxh,vyh;
  xtbool2     x_sgn;
  if(N<=0) return;
  vnw = AE_MOVDA32X2(0x1FFFFFFF, 0x1FFFFFFF); //
  vsw = AE_MOVDA32X2(0xFFFFFFFC, 0xFFFFFFFC); //
  vkw = AE_MOVDA32X2(0x20000000, 0x20000000); //
  vlw = AE_MOVDA32X2(0x40000000, 0x40000000); //
  vzw = AE_MOVI(0);
  x_align = AE_LA64_PP(px);
  z_align = AE_ZALIGN64();

  __Pragma("concurrent");
  for (n=0;n<N-1;n+=2)
  {
    AE_LA32X2_IP(vxw, x_align, px);
    x_sgn = AE_LT32(vxw, vzw);
    vxt = AE_NEG32S(vxw);
    AE_MOVT32X2(vxw, vxt, x_sgn);

    vyw = AE_SRAI32(vxw,24); //
    vdw = AE_SLAI32(vxw,5);
    
    vaw = AE_AND32(vdw,vnw);//d0
    vbw = AE_SUB32(vaw,vkw);//d1
    vcw = AE_SUB32(vaw,vlw);//d2
    
    ofa = AE_MOVAD32_H(vyw);
    ofb = AE_MOVAD32_L(vyw);

    py+=(ofa);
    pq+=(ofb);

    AE_L32_IP(va0, py, sizeof(*py));
    AE_L32_IP(va1, py, sizeof(*py));
    AE_L32_IP(va2, py, 0);

    AE_L32_IP(vb0, pq, sizeof(*pq));
    AE_L32_IP(vb1, pq, sizeof(*pq));
    AE_L32_IP(vb2, pq, 0);

    vd1 = AE_MULFP32X2RAS(vcw, vbw);//d1*d2
    vd2 = AE_MULFP32X2RAS(vaw, vbw);//d0*d1
    vd3 = AE_MULFP32X2RAS(vaw, vcw);//d0*d2

    vha = AE_MULF32S_HH(vd1,va0);//t*y0
    vhb = AE_MULF32S_LH(vd1,vb0);//t*y0

    AE_MULAF32S_HH(vha,vd2,va2);
    AE_MULAF32S_LH(vhb,vd2,vb2);

    vha = AE_SRAI64(vha,1);
    vhb = AE_SRAI64(vhb,1);

    AE_MULSF32S_HH(vha,vd3,va1);
    AE_MULSF32S_LH(vhb,vd3,vb1);
    vxh = AE_SRAI64(vha, 29);
    vyh = AE_SRAI64(vhb, 29);
    vxx = AE_MOVINT32X2_FROMINT64(vxh);
    vyy = AE_MOVINT32X2_FROMINT64(vyh);
    
    vxx = AE_SEL32_LL(vxx,vyy);
    vya = AE_NEG32S(vxx);

    AE_MOVT32X2(vxx, vya, x_sgn);

    py = (ae_int32 *)atan_table;
    pq = (ae_int32 *)atan_table;
       
    AE_SA32X2_IP(vxx, z_align, pz);//put answer
  }
  AE_SA64POS_FP(z_align, pz);
  if (N&1)
  {
    AE_L32_IP(vxw, castxcc(ae_int32,px), 0);
    x_sgn = AE_LT32(vxw, vzw);
    vxt = AE_NEG32S(vxw);
    AE_MOVT32X2(vxw, vxt, x_sgn);

    vyw = AE_SRAI32(vxw,24); //
    vdw = AE_SLAI32(vxw,5);
    
    vaw = AE_AND32(vdw,vnw);//d0
    vbw = AE_SUB32(vaw,vkw);//d1
    vcw = AE_SUB32(vaw,vlw);//d2
    
    ofa = AE_MOVAD32_H(vyw);
    ofb = AE_MOVAD32_L(vyw);

    py+=(ofa);
    pq+=(ofb);

    AE_L32_IP(va0, py, sizeof(*py));
    AE_L32_IP(va1, py, sizeof(*py));
    AE_L32_IP(va2, py, 0);

    AE_L32_IP(vb0, pq, sizeof(*pq));
    AE_L32_IP(vb1, pq, sizeof(*pq));
    AE_L32_IP(vb2, pq, 0);

    vd1 = AE_MULFP32X2RAS(vcw, vbw);//d1*d2
    vd2 = AE_MULFP32X2RAS(vaw, vbw);//d0*d1
    vd3 = AE_MULFP32X2RAS(vaw, vcw);//d0*d2

    AE_MULAF32S_HH(vha,vd2,va2);
    AE_MULAF32S_LH(vhb,vd2,vb2);

    vha = AE_SRAI64(vha,1);
    vhb = AE_SRAI64(vhb,1);

    AE_MULSF32S_HH(vha,vd3,va1);
    AE_MULSF32S_LH(vhb,vd3,vb1);

    vxh = AE_SRAI64(vha, 29);
    vyh = AE_SRAI64(vhb, 29);
    vxx = AE_MOVINT32X2_FROMINT64(vxh);
    vyy = AE_MOVINT32X2_FROMINT64(vyh);
    
    vxx = AE_SEL32_LL(vxx,vyy);
    vya = AE_NEG32S(vxx);

    AE_MOVT32X2(vxx, vya, x_sgn);

    vha = AE_MULF32S_HH(vd1,va0);//t*y0
    AE_MULAF32S_HH(vha,vd2,va2);
    vha = AE_SRAI64(vha,1);
    AE_MULSF32S_HH(vha,vd3,va1);
    vxh = AE_SRAI64(vha, 29);
    vaw = AE_MOVINT32X2_FROMINT64(vxh);
    vyw = AE_SEL32_LL(vaw,vaw);
    vya = AE_NEG32S(vyw);
    AE_MOVT32X2(vyw, vya, x_sgn);
    vyw = AE_SEL32_HH(vyw,vyw);
    AE_S32_L_I(vyw, (ae_int32 *)pz, 0);
  }
}
