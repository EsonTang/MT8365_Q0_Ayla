﻿/****************************************************************
 Copyright 2015-2016 Sony Corporation
 ****************************************************************/

#ifndef __SONY_IMX519_PDAF_TRANS_COORD_H__
#define __SONY_IMX519_PDAF_TRANS_COORD_H__

#define D_SONY_PD_LIB_REGDATA_IS_OK     (0)

#define D_SONY_EREG_IMG_ORI_H           (11)
#define D_SONY_EREG_IMG_ORI_V           (12)
#define D_SONY_EREG_HDR_MODE            (21)
#define D_SONY_EREG_HDR_RESO_H          (31)
#define D_SONY_EREG_HDR_RESO_V          (32)
#define D_SONY_EREG_HDR_RESO_SAME       (33)
#define D_SONY_EREG_X_ODD_INC           (41)
#define D_SONY_EREG_X_EVN_INC           (42)
#define D_SONY_EREG_Y_ODD_INC           (51)
#define D_SONY_EREG_Y_EVN_INC           (52)
#define D_SONY_EREG_SCALE_MODE          (61)
#define D_SONY_EREG_SCALE_M_RANGE       (71)
#define D_SONY_EREG_SCALE_M             (72)
#define D_SONY_EREG_BINN_MODE           (81)
#define D_SONY_EREG_BINN_TYPE_H         (91)
#define D_SONY_EREG_BINN_TYPE_V         (92)
#define D_SONY_EREG_ODD_INC_SAME        (101)
#define D_SONY_EREG_FORCE_FDSUM_SET     (111)

#define D_SONY_PD_LIB_COORD_ERROR       (-1)

typedef struct {
    unsigned int reg_addr_0x0101;
    unsigned int reg_addr_0x0220;
    unsigned int reg_addr_0x0221;
    unsigned int reg_addr_0x0344;
    unsigned int reg_addr_0x0345;
    unsigned int reg_addr_0x0346;
    unsigned int reg_addr_0x0347;
    unsigned int reg_addr_0x0348;
    unsigned int reg_addr_0x0349;
    unsigned int reg_addr_0x034A;
    unsigned int reg_addr_0x034B;
    unsigned int reg_addr_0x034C;
    unsigned int reg_addr_0x034D;
    unsigned int reg_addr_0x034E;
    unsigned int reg_addr_0x034F;
    unsigned int reg_addr_0x0381;
    unsigned int reg_addr_0x0383;
    unsigned int reg_addr_0x0385;
    unsigned int reg_addr_0x0387;
    unsigned int reg_addr_0x0401;
    unsigned int reg_addr_0x0404;
    unsigned int reg_addr_0x0405;
    unsigned int reg_addr_0x0408;
    unsigned int reg_addr_0x0409;
    unsigned int reg_addr_0x040A;
    unsigned int reg_addr_0x040B;
    unsigned int reg_addr_0x040C;
    unsigned int reg_addr_0x040D;
    unsigned int reg_addr_0x040E;
    unsigned int reg_addr_0x040F;
    unsigned int reg_addr_0x0900;
    unsigned int reg_addr_0x0901;
    unsigned int reg_addr_0x3F42;
    unsigned int reg_addr_0x3F43;
} SonyPdLibSensorCoordRegData_t;

typedef struct {
    unsigned int img_orientation_h;
    unsigned int img_orientation_v;
    unsigned int hdr_mode_en;
    unsigned int hdr_reso_redu_h;
    unsigned int hdr_reso_redu_v;
    unsigned int x_add_sta;
    unsigned int y_add_sta;
    unsigned int x_add_end;
    unsigned int y_add_end;
    unsigned int x_out_size;
    unsigned int y_out_size;
    unsigned int x_evn_inc;
    unsigned int x_odd_inc;
    unsigned int y_evn_inc;
    unsigned int y_odd_inc;
    unsigned int scale_mode;
    unsigned int scale_m;
    unsigned int dig_crop_x_offset;
    unsigned int dig_crop_y_offset;
    unsigned int dig_crop_image_width;
    unsigned int dig_crop_image_height;
    unsigned int binning_mode;
    unsigned int binning_type_h;
    unsigned int binning_type_v;
    unsigned int binning_type_ext_en;
    unsigned int binning_type_h_ext;
} SonyPdLibSensorCoordSetting_t;

typedef struct {
    signed int x;
    signed int y;
} SonyPdLibPoint_t;

typedef struct {
    SonyPdLibPoint_t sta;
    SonyPdLibPoint_t end;
} SonyPdLibRect_t;

extern int SonyPdLibInterpretRegData1(SonyPdLibSensorCoordRegData_t *p_regData, SonyPdLibSensorCoordSetting_t *p_setting);

extern SonyPdLibPoint_t SonyPdLibTransOutputPointToPdafPoint(SonyPdLibPoint_t onOutImage, SonyPdLibSensorCoordSetting_t *p_setting);

extern SonyPdLibRect_t SonyPdLibTransOutputRectToPdafRect(SonyPdLibRect_t onOutImage, SonyPdLibSensorCoordSetting_t *p_setting);

#endif // __SONY_PDAF_IMX519_TRANS_COORD_H__
