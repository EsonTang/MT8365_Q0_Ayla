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

/*
    tables for expf(x) approximation
*/
#ifndef EXPFTBL_H__
#define EXPFTBL_H__

#include "NatureDSP_types.h"
#include "common.h"

/* 
   polynomial coefficients for 2^x in range 0...1

   derived by MATLAB code:
   order=6;
   x=(0:pow2(1,-16):1);
   y=2.^x;
   p=polyfit(x,y,6);
   p(order+1)=1;
   p(order)=p(order)-(sum(p)-2);
*/
externC const int32_t expftbl_Q30[8]; 
externC const union ufloat32uint32 expfminmax[2];  /* minimum and maximum arguments of expf() input */
externC const int32_t invln2_Q30; /* 1/ln(2), Q30 */

#endif /* EXPFTBL_H__ */
