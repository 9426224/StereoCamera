/*! \file eSPDI_ErrCode.h
  	\brief definition of LenaD SDK error code
  	Copyright:
	This file copyright (C) 2017 by

	LenaCV company

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of LenaCV company
 */
 
//define Error Code by Wolf 2013/08/30
#define  LenaDDI_OK                             0
#define  LenaDDI_NoDevice                      -1
#define  LenaDDI_NullPtr                       -2
#define  LenaDDI_ErrBufLen                     -3
#define  LenaDDI_Init_Fail                     -4
#define  LenaDDI_NoZDTable                     -5 
#define  LenaDDI_READFLASHFAIL                 -6
#define  LenaDDI_WRITEFLASHFAIL                -7 
#define  LenaDDI_VERIFY_DATA_FAIL              -8
#define  LenaDDI_KEEP_DATA_FAIL                -9
#define  LenaDDI_RECT_DATA_LEN_FAIL           -10
#define  LenaDDI_RECT_DATA_PARSING_FAIL       -11
#define  LenaDDI_RET_BAD_PARAM                -12
#define  LenaDDI_RET_OPEN_FILE_FAIL           -13
#define  LenaDDI_NO_CALIBRATION_LOG           -14
#define  LenaDDI_POSTPROCESS_INIT_FAIL        -15
#define  LenaDDI_POSTPROCESS_NOT_INIT         -16
#define  LenaDDI_POSTPROCESS_FRAME_FAIL       -17
#define  LenaDDI_NotSupport                   -18
#define  LenaDDI_OpenFileFail                 -19
#define  LenaDDI_READ_REG_FAIL                -20
#define  LenaDDI_WRITE_REG_FAIL               -21
#define  LenaDDI_SET_FPS_FAIL                 -22
#define  LenaDDI_VIDEO_RENDER_FAIL            -23
#define  LenaDDI_OPEN_DEVICE_FAIL             -24
#define  LenaDDI_FIND_DEVICE_FAIL             -25
#define  LenaDDI_GET_IMAGE_FAIL               -26
#define  LenaDDI_NOT_SUPPORT_RES              -27
#define  LenaDDI_CALLBACK_REGISTER_FAIL       -28
#define  LenaDDI_DEVICE_NOT_SUPPORT			  -29

// for 3D Scanner +    
#define  LenaDDI_ILLEGAL_ANGLE                -30
#define  LenaDDI_ILLEGAL_STEP                 -31
#define  LenaDDI_ILLEGAL_TIMEPERSTEP          -32
#define  LenaDDI_MOTOR_RUNNING                -33 
#define  LenaDDI_GETSENSORREG_FAIL            -34
#define  LenaDDI_SETSENSORREG_FAIL            -35
#define  LenaDDI_READ_X_AXIS_FAIL             -36
#define  LenaDDI_READ_Y_AXIS_FAIL             -37
#define  LenaDDI_READ_Z_AXIS_FAIL             -38
#define  LenaDDI_READ_PRESS_DATA_FAIL         -39
#define  LenaDDI_READ_TEMPERATURE_FAIL        -40
#define  LenaDDI_RETURNHOME_RUNNING           -41
#define  LenaDDI_MOTOTSTOP_BY_HOME_INDEX      -42
#define  LenaDDI_MOTOTSTOP_BY_PROTECT_SCHEME  -43
#define  LenaDDI_MOTOTSTOP_BY_NORMAL          -44
#define  LenaDDI_ILLEGAL_FIRMWARE_VERSION     -45
#define  LenaDDI_ILLEGAL_STEPPERTIME          -46
// for 3D Scanner - 

// For AEAWB + 2015/01/28 by Wolf
#define  LenaDDI_GET_PU_PROP_VAL              -50
#define  LenaDDI_SET_PU_PROP_VAL              -51
#define  LenaDDI_GET_CT_PROP_VAL              -52
#define  LenaDDI_SET_CT_PROP_VAL              -53
// For AEAWB - 2015/01/28 by Wolf 

// for Dewarping + Stitching +
#define  LenaDDI_INVALID_USERDATA             -70
#define  LenaDDI_MAP_LUT_FAIL                 -71
#define  LenaDDI_APPEND_TO_FILE_FRONT_FAIL    -72
// for Dewarping + Stitching -

#define LenaDDI_TOO_MANY_DEVICE               -80
#define LenaDDI_ACCESS_MP4_EXTRA_DATA_FAIL    -81