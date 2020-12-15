/*! \file eSPDI_Common.h
  	\brief LenaD SDK API export functions, data structure and variable definition
  	\copyright This file copyright (C) 2017 by LenaCV company

	\details An unpublished work.  All rights reserved.
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of LenaCV company.
 */
#pragma once

#ifndef LenaDDI_API
#ifdef __WEYE__
#define LenaDDI_API
#else
#ifdef LenaDDI_EXPORTS
#define LenaDDI_API __declspec(dllexport)
#else
#define LenaDDI_API __declspec(dllimport)
#endif
#endif
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif //BYTE

#ifndef WORD
typedef unsigned short WORD;
#endif //WORD

#ifndef WCHAR
typedef wchar_t WCHAR;
#endif //WCHAR

#ifndef BOOL
typedef signed int BOOL;
#endif //BOOL

//
// C++ compatibility
//
#ifdef  __cplusplus
extern "C" {
#endif

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif //CALLBACK


#define LenaDDI_MAX_STREAM_COUNT 64
#define LenaDDI_MAX_DEPTH_STREAM_COUNT 8
#pragma pack(push, 1)
typedef struct tagLenaDDI_STREAM_INFO {
	int		nWidth;
	int		nHeight;
	BOOL	bFormatMJPG;
} LenaDDI_STREAM_INFO, *PLenaDDI_STREAM_INFO;
#pragma pack(pop)

#ifndef WM_MYMSG_NOTICE_CAPTURE
#define WM_MYMSG_NOTICE_CAPTURE	(WM_USER+101)
#endif

#include "eSPDI_ErrCode.h"

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

#define FG_Address_1Byte 0x01
#define FG_Address_2Byte 0x02
#define FG_Value_1Byte   0x10
#define FG_Value_2Byte   0x20

// For Depth Data Type - 2016/12/14 by Sean
#define LenaDDI_DEPTH_DATA_DEFAULT				0
#define LenaDDI_DEPTH_DATA_OFF_RAW				0 // raw (depth off, only raw color)
#define LenaDDI_DEPTH_DATA_8_BITS				1 // rectify
#define LenaDDI_DEPTH_DATA_14_BITS				2 // rectify
#define LenaDDI_DEPTH_DATA_8_BITS_x80			3 // rectify
#define LenaDDI_DEPTH_DATA_11_BITS				4 // rectify
#define LenaDDI_DEPTH_DATA_OFF_RECTIFY			5 // rectify (depth off, only rectify color)
#define LenaDDI_DEPTH_DATA_8_BITS_RAW			6 // raw
#define LenaDDI_DEPTH_DATA_14_BITS_RAW			7 // raw
#define LenaDDI_DEPTH_DATA_8_BITS_x80_RAW		8 // raw
#define LenaDDI_DEPTH_DATA_11_BITS_RAW			9 // raw
#define LenaDDI_DEPTH_DATA_11_BITS_COMBINED_RECTIFY     13// multi-baseline


// For Flash Read/Write
// Firmware (size in KBytes)
#define LenaDDI_READ_FLASH_TOTAL_SIZE			128
#define LenaDDI_READ_FLASH_FW_PLUGIN_SIZE		104
#define LenaDDI_WRITE_FLASH_TOTAL_SIZE			128

// PlugIn data (size in bytes)
#define LenaDDI_Y_OFFSET_FILE_ID_0				30
#define LenaDDI_Y_OFFSET_FILE_SIZE			    256
#define LenaDDI_RECTIFY_FILE_ID_0				40
#define LenaDDI_RECTIFY_FILE_SIZE				1024
#define LenaDDI_ZD_TABLE_FILE_ID_0				50
#define LenaDDI_ZD_TABLE_FILE_SIZE_8_BITS		512
#define LenaDDI_ZD_TABLE_FILE_SIZE_11_BITS		4096
#define LenaDDI_CALIB_LOG_FILE_ID_0				240
#define LenaDDI_CALIB_LOG_FILE_SIZE				4096
#define LenaDDI_USER_DATA_FILE_ID_0				200
#define LenaDDI_USER_DATA_FILE_SIZE_0			1024
#define LenaDDI_USER_DATA_FILE_SIZE_1			4096
#define LenaDDI_USER_DATA_FILE_SIZE_2			128
#define LenaDDI_USER_DATA_FILE_SIZE_3			1024
#define LenaDDI_USER_DATA_FILE_SIZE_4			4096

typedef struct tagZDTableInfo
{
    int nIndex;
    int nDataType;
} ZDTABLEINFO, *PZDTABLEINFO;

/**
 * \brief eSPCtrl_RectLogData
 *
 * Rectified log data structure
 */
typedef struct eSPCtrl_RectLogData
{
    union {
        BYTE uByteArray[1024];
        struct {
            WORD	InImgWidth;
            WORD	InImgHeight;
            WORD	OutImgWidth;
            WORD	OutImgHeight;
            int		RECT_ScaleEnable;
            int		RECT_CropEnable;
            WORD	RECT_ScaleWidth;
            WORD	RECT_ScaleHeight;
            float	CamMat1[9];
            float	CamDist1[8];
            float	CamMat2[9];
            float	CamDist2[8];
            float	RotaMat[9];
            float	TranMat[3];
            float	LRotaMat[9];
            float	RRotaMat[9];
            float	NewCamMat1[12];
            float	NewCamMat2[12];
            WORD	RECT_Crop_Row_BG;
            WORD	RECT_Crop_Row_ED;
            WORD	RECT_Crop_Col_BG_L;
            WORD	RECT_Crop_Col_ED_L;
            BYTE	RECT_Scale_Col_M;
            BYTE	RECT_Scale_Col_N;
            BYTE	RECT_Scale_Row_M;
            BYTE	RECT_Scale_Row_N;
            float	RECT_AvgErr;
            WORD	nLineBuffers;
            float ReProjectMat[16];
        };
    };
} eSPCtrl_RectLogData;

/**
 * \brief ParaLUT
 *
 * Spherical look-up table conversion parameters
 */
typedef struct ParaLUT
{
    long long file_ID_header;       //!< [00]-[000] File ID header : 2230
    long long file_ID_version;      //!< [01]-[008] File ID version : 4
    double    FOV;                  //!< [02]-[016] Field of view with degree
    long long semi_FOV_pixels;      //!< [03]-[024] Pixels for semi-FOV
    long long img_src_cols;         //!< [04]-[032] Width for source image (single image)
    long long img_src_rows;         //!< [05]-[040] Height for source image
    double    img_L_src_col_center; //!< [06]-[048] Center of width for L side source image
    double    img_L_src_row_center; //!< [07]-[056] Center of height for L side source image
    double    img_R_src_col_center; //!< [08]-[064] Center of width for R side source image
    double    img_R_src_row_center; //!< [09]-[072] Center of height for R side source image
    double    img_L_rotation;       //!< [10]-[080] Rotation for L side image
    double    img_R_rotation;       //!< [11]-[088] Rotation for R side image
    double    spline_control_v1;    //!< [12]-[096] Spline control value for row = DIV x 0 pixel, DIV = rows/6
    double    spline_control_v2;    //!< [13]-[104] Spline control value for row = DIV x 1 pixel, DIV = rows/6
    double    spline_control_v3;    //!< [14]-[112] Spline control value for row = DIV x 2 pixel, DIV = rows/6
    double    spline_control_v4;    //!< [15]-[120] Spline control value for row = DIV x 3 pixel, DIV = rows/6
    double    spline_control_v5;    //!< [16]-[128] Spline control value for row = DIV x 4 pixel, DIV = rows/6
    double    spline_control_v6;    //!< [17]-[136] Spline control value for row = DIV x 5 pixel, DIV = rows/6
    double    spline_control_v7;    //!< [18]-[144] Spline control value for row = DIV x 6 pixel, DIV = rows/6
    long long img_dst_cols;         //!< [19]-[152] Width for output image (single image), according to "Original" parameters  
    long long img_dst_rows;         //!< [20]-[160] Height for output image, according to "Original" parameters
    long long img_L_dst_shift;      //!< [21]-[168] Output L side image shift in row 
    long long img_R_dst_shift;      //!< [22]-[176] Output R side image shift in row  
    long long img_overlay_LR;       //!< [23]-[184] Overlay between L/R in pixels, far field, (YUV must be even) 
    long long img_overlay_RL;       //!< [24]-[192] Overlay between R/L in pixels, far field, (YUV must be even) 
    long long img_stream_cols;      //!< [25]-[200] Output image stream of cols 
    long long img_stream_rows;      //!< [26]-[208] Output image stream of rows 
    long long video_stream_cols;    //!< [27]-[216] Output video stream of cols 
    long long video_stream_rows;    //!< [28]-[224] Output video stream of rows 
    long long usb_type;             //!< [29]-[232] 2 for usb2, 3 for usb3
    long long img_type;             //!< [30]-[240] 1 for yuv422, 2 for BGR, 3 for RGB
    long long lut_type;             //!< [31]-[248] Output LUT tye @ref eys::LutModes
    long long blending_type;        //!< [32]-[256] 0 for choosed by function, 1 for alpha-blending, 2 for Laplacian pyramid blending
    double    overlay_ratio;        //!< [33]-[264] far field overlay value is equal to img_overlay_LR(RL)  = overlay_value + overlay_ratio 

    long long serial_number_date0;  //!< [34]-[272] 8 bytes, yyyy-mm-dd
    long long serial_number_date1;  //!< [35]-[280] 8 bytes, hh-mm-ss-xxx, xxx for machine number

    double    unit_sphere_radius;   //!< [36]-[288] Original : Unit spherical radius for dewarping get x and y
    double    min_col;              //!< [37]-[296] Original : Parameters of min position of image width
    double    max_col;              //!< [38]-[304] Original : Parameters of max position of image width
    double    min_row;              //!< [39]-[312] Original : Parameters of min position of image height
    double    max_row;              //!< [40]-[320] Original : Parameters of max position of image height

    long long AGD_LR;               //!< [41]-[328] Err : Average gray-level value discrepancy at LR boundary
    long long AGD_RL;               //!< [42]-[336] Err : Average gray-level value discrepancy at RL boundary

    long long out_img_resolution;   //!< [43]-[344] Set output resolution @ref eys::ImgResolutionModes 
    long long out_lut_cols;         //!< [44]-[352] Output side-by-side lut width, according to the set of out_img_resolution
    long long out_lut_rows;         //!< [45]-[360] Output lut height, according to the set of out_img_resolution
    long long out_lut_cols_eff;     //!< [46]-[368] Output effective pixels in out_lut_cols, 0 is for all
    long long out_lut_rows_eff;     //!< [47]-[376] Output effecitve pixels in out_lut_rows, 0 is for all
    long long out_img_cols;         //!< [48]-[384] Output side-by-side image width after dewarping and stitching, according to the set of out_img_resolution
    long long out_img_rows;         //!< [49]-[392] Output image height, according to the set of out_img_resolution
    long long out_overlay_LR;       //!< [50]-[340] Output L/R overlay value, according to the set of out_img_resolution
    long long out_overlay_RL;       //!< [51]-[408] Output R/L overlay value, according to the set of out_img_resolution
    long long reserve[44];          //!< [52]-[416] Reserve 44 parameter to use
    BYTE      serial_number[256];
} PARALUT, *PPARALUT;

struct LenaDDIImageType
{
    enum Value
    {
        IMAGE_UNKNOWN = -1,
        COLOR_YUY2 = 0,
        COLOR_RGB24, 
        COLOR_MJPG, 
        DEPTH_8BITS = 100,
        DEPTH_8BITS_0x80,
        DEPTH_11BITS,
        DEPTH_14BITS
    };

    static bool IsImageColor(LenaDDIImageType::Value type)
    {
        return (type == COLOR_YUY2 || type == COLOR_RGB24 || type == COLOR_MJPG);
    }

    static bool IsImageDepth(LenaDDIImageType::Value type)
    {
        return (type != IMAGE_UNKNOWN && !IsImageColor(type));
    }

    static LenaDDIImageType::Value DepthDataTypeToDepthImageType(WORD dataType)
    {
        switch (dataType)
        {
        case LenaDDI_DEPTH_DATA_8_BITS:
        case LenaDDI_DEPTH_DATA_8_BITS_RAW:
            return LenaDDIImageType::DEPTH_8BITS;
        case LenaDDI_DEPTH_DATA_8_BITS_x80:
        case LenaDDI_DEPTH_DATA_8_BITS_x80_RAW:
            return LenaDDIImageType::DEPTH_8BITS_0x80;
        case LenaDDI_DEPTH_DATA_11_BITS:
        case LenaDDI_DEPTH_DATA_11_BITS_RAW:
        case LenaDDI_DEPTH_DATA_11_BITS_COMBINED_RECTIFY:
            return LenaDDIImageType::DEPTH_11BITS;
        case LenaDDI_DEPTH_DATA_14_BITS:
        case LenaDDI_DEPTH_DATA_14_BITS_RAW:
            return LenaDDIImageType::DEPTH_14BITS;
        default: return LenaDDIImageType::IMAGE_UNKNOWN;
        }
    }
};

struct LenaDDIDepthSwitch
{
    enum Value
    {
        Depth0 = 0x01, 
        Depth1 = 0x02, 
        Depth2 = 0x04
    };

    static bool IsOn(LenaDDIDepthSwitch::Value target, int setting)
    {
        return ((target & setting) != 0);
    }
};
/*! \fn void(*LenaDDI_ImgCallbackFn)(LenaDDIImageType::Value imgType, int imgId, unsigned char* imgBuf, int imgSize,
    int width, int height, int serialNumber, void* pParam)
\brief Callback function when video or data is ready
\param pid	product id of the USB device
\param vid	vender id of the USB device
\param bAttached TRUE if this is a USB device attached event, otherwise, it is a detached event.
\param pData user defined data to pass to the callback function
\return none
*/
typedef void(*LenaDDI_ImgCallbackFn)(LenaDDIImageType::Value imgType, int imgId, unsigned char* imgBuf, int imgSize,
    int width, int height, int serialNumber, void* pParam);
	
/**
 * \enum LenaDDI_DEVICE_TYPE
 *
 * chip enum value
 */
typedef enum {
    OTHERS = 0,		/**< Other */
    AXES1,			/**< AXIS1 */
    PUMA,			/**< PUMA */
    PLUM
}LenaDDI_DEVICE_TYPE;

struct LenaDDI_SensorMode
{
    enum Value
    {
        Sensor1 = 0,
        Sensor2 = 1,
        SensorAll = 2, 
        Sensor3 = 3, 
        Sensor4 = 4
    };
};
/**
 * \brief DEVINFORMATION
 *
 * device information
 */
typedef struct tagDEVINFORMATION {
    unsigned short wPID;			/**< product ID */
									/**<	<table>
									<caption id="multi_row">PID List</caption>
									<tr><th>Chip Name      <th>Chip ID             <th>PID
									<tr><td rowspan="5">AXES1 <td td rowspan="5">0x18 <td>0x0568
									<tr> <td>0x0668
									<tr> <td>0x0113
									<tr> <td>0x0115
									<tr> <td>0x0116
									<tr><td>KIWI <td>0x1C <td>0x0118
									<tr><td rowspan="2">PUMA <td td rowspan="2">0x15 <td>0x0112
									<tr> <td>0x0120
									</table>
									*/
    unsigned short wVID;			/**< vender ID, 0x1E4E for LenaDDI device */
    char *strDevName;				/**< pointer to device name stored inside the SDK*/
    unsigned short  nChipID;		/**< chip ID, 0x18 for AXES1, 0x1C for KIWI, 0x15 for PUMA */
    unsigned short  nDevType;		/**< chip enum value, @see LenaDDI_DEVICE_TYPE */
} DEVINFORMATION;


/**
 * \class DEVINFORMATIONEX
 *
 * extended device information class
 */
class DEVINFORMATIONEX 
{
public:
    DEVINFORMATIONEX()
    {
        wPID = wVID = nChipID = 0;
        nDevType = OTHERS;
        strDevName[0] = '\0';
    }

    DEVINFORMATIONEX& operator=(const DEVINFORMATIONEX& rhs)
    {
        wPID = rhs.wPID;
        wVID = rhs.wVID;
        strcpy_s(strDevName, rhs.strDevName);
        nChipID = rhs.nChipID;
        nDevType = rhs.nDevType;

        return *this;
    }

    DEVINFORMATIONEX& operator=(const DEVINFORMATION& rhs)
    {
        wPID = rhs.wPID;
        wVID = rhs.wVID;
        strcpy_s(strDevName, rhs.strDevName);
        nChipID = rhs.nChipID;
        nDevType = rhs.nDevType;

        return *this;
    }

    DEVINFORMATIONEX(const DEVINFORMATIONEX& rhs)
    {
        *this = rhs;
    }

    unsigned short wPID;		/**< product ID */
									/**<	<table>
									<caption id="multi_row">PID List</caption>
									<tr><th>Chip Name      <th>Chip ID             <th>PID
									<tr><td rowspan="5">AXES1 <td td rowspan="5">0x18 <td>0x0568
									<tr> <td>0x0668
									<tr> <td>0x0113
									<tr> <td>0x0115
									<tr> <td>0x0116
									<tr><td>KIWI <td>0x1C <td>0x0118
									<tr><td rowspan="2">PUMA <td td rowspan="2">0x15 <td>0x0112
									<tr> <td>0x0120
									</table>
									*/
    unsigned short wVID;		/**< vender ID, 0x1E4E for LenaDDI device */
    char strDevName[512];		/**< device name */
    unsigned short nChipID;		/**< chip ID, 0x18 for AXES1, 0x1C for KIWI, 0x15 for PUMA */
    unsigned short nDevType;	/**< chip enum value, see LenaDDI_DEVICE_TYPE */
};
/*! \struct DEVSELINFO*/
typedef struct tagDEVSEL
{
  int index;					/**< device index */
} DEVSELINFO, *PDEVSELINFO;

/*! \enum User data section id */
typedef enum
{
  USERDATA_SECTION_0 = 0,	/**< Section 0 */
  USERDATA_SECTION_1,		/**< Section 1 */
  USERDATA_SECTION_2,		/**< Section 2 */
  USERDATA_SECTION_3,		/**< Section 3 */
  USERDATA_SECTION_4,		/**< Section 4 */
  USERDATA_SECTION_5,		/**< Section 5 */
  USERDATA_SECTION_6,		/**< Section 6 */
  USERDATA_SECTION_7,		/**< Section 7 */
  USERDATA_SECTION_8,		/**< Section 8 */
  USERDATA_SECTION_9,		/**< Section 9 */
  USERDATA_SECTION_10,		/**< Section 10 */
  USERDATA_SECTION_NUM      /**< Total Section Number */
} USERDATA_SECTION_INDEX;

// for total and fw+plugin read/write +
typedef enum
{
    Total = 0,
    FW_PLUGIN
} FLASH_DATA_TYPE;

typedef struct tagKEEP_DATA_CTRL {
	bool  bIsSerialNumberKeep;
	bool  bIsSensorPositionKeep;
	bool  bIsRectificationTableKeep;
	bool  bIsZDTableKeep;
	bool  bIsCalibrationLogKeep;
} KEEP_DATA_CTRL;
// for total and fw+plugin read/write -

/*! \fn int LenaDDI_Init(
		void **ppHandleLenaDDI,
		bool bIsLogEnabled)
	\brief entry point of LenaD camera SDK. This API allocates resource and find all the eSPI camera devices connected to the system.
	\param ppHandleLenaDDI	a pointer of pointer to receive LenaDDI SDK instance
	\param bIsLogEnabled	set to true to generate log file, named log.txt in current folder
	\return success: none negative integer to indicate numbers of devices found in the system.
			
*/
int  LenaDDI_API LenaDDI_Init( void **ppHandleLenaDDI, bool bIsLogEnabled);

/*! \fn int LenaDDI_Init2(
		void **ppHandleLenaDDI,
		bool bIsLogEnabled,
		bool bEnableAutoRestart)
	\brief entry point of LenaD camera SDK. This API allocates resource and find all the eSPI camera devices connected to the system.
	\param ppHandleLenaDDI	a pointer of pointer to receive LenaDDI SDK instance
	\param bIsLogEnabled	set to true to generate log file, named log.txt in current folder
	\param bEnableAutoRestart	set true to auto-restart the device if the device was detached and attached again.
	\return success: none negative integer to indicate numbers of devices found in the system.
	\note Calls LenaDDI_Init or LenaDDI_Init2 to initilize the LenaDDI SDK. LenaDDI_Init2 adds the auto-restart function to the initilization options.
	      If you call LenaDDI_Init, the bEnableAutoRestart is set as disabled.
*/
int  LenaDDI_API LenaDDI_Init2( void **ppHandleLenaDDI, bool bIsLogEnabled, bool bAutoRestart);

/*! \fn void (*LenaDDI_DeviceEventFn)(UINT pid, UINT vid, BOOL bAttached, void* pData)
\brief Callback function to receive any USB capture device attachment or detachment events
\param pid	product id of the USB device
\param vid	vender id of the USB device
\param bAttached TRUE if this is a USB device attached event, otherwise, it is a detached event.
\param pData user defined data to pass to the callback function
\return none
*/
#ifndef LenaDDI_DeviceEventFn_
typedef void(*LenaDDI_DeviceEventFn)(UINT pid, UINT vid, BOOL bAttached, void* pData);
#define LenaDDI_DeviceEventFn_
#endif

/*! \fn int LenaDDI_RegisterDeviceEvents(
		void *pHandleLenaDDI,
		LenaDDI_DeviceEventFn cbFunc, void *pData)
	\brief Register the USB device plug or unplug events. Any USB capture device attachment or detachment events will call the callback function cbFunc
	\param pHandleLenaDDI	a pointer to LenaDDI SDK instance
	\param cbFunc a callback function of type #LenaDDI_DeviceEventFn that will receive USB cappure device events when the device is attached or detached.
	\param pData user defined data which will send to the callback function
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int  LenaDDI_API LenaDDI_RegisterDeviceEvents(void *pHandleLenaDDI, LenaDDI_DeviceEventFn cbFunc, void *pData);

/*! \fn void LenaDDI_Release(
		void **ppHandleLenaDDI)
	\brief release all resource that LenaDDI_Init had allocated
	\param ppHandleLenaDDI	pointer of the pointer to the initilized LenaDDI SDK instance.
	\return none
	\note the pointer to ppHandleLenaDDI will be set to NULL when this call returns successfully. 
*/
void LenaDDI_API LenaDDI_Release( void **ppHandleLenaDDI);

/*! \fn int LenaDDI_FindDevice(
		void *pHandleLenaDDI)
	\brief find out all LenaD USB devices by PID, VID and ChipID, also remember device types
	\param pHandleLenaDDI the pointer to the initilized LenaDDI SDK instance
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
 */
int  LenaDDI_API LenaDDI_FindDevice( void *pHandleLenaDDI);

/*! \fn int LenaDDI_RefreshDevice(
		void *pHandleLenaDDI)
	\brief refresh all LenaD UVC devices
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
 */
int  LenaDDI_API LenaDDI_RefreshDevice( void *pHandleLenaDDI);

/*! \fn int LenaDDI_GetDeviceNumber(
		void *pHandleLenaDDI)
	\brief get LenaD USB device numbers
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\return number of LenaD device
*/
int  LenaDDI_API LenaDDI_GetDeviceNumber( void *pHandleLenaDDI);

/*! \fn int LenaDDI_GetDeviceInfo(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		DEVINFORMATION* pdevinfo)
	\brief get informations of LenaD UVC devices, see @DEVINFORMATION
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param pdevinfo	pointer of device information
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int  LenaDDI_API LenaDDI_GetDeviceInfo( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo ,DEVINFORMATION* pdevinfo);

/*! \fn int LenaDDI_GetDeviceInfoEx(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		DEVINFORMATIONEX* pdevinfo)
	\brief get target device info,
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param pdevinfo	pointer of buffer to store DEVINFORMATIONEX
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int  LenaDDI_API LenaDDI_GetDeviceInfoEx( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo ,DEVINFORMATIONEX* pdevinfo);

// register APIs +
/*! \fn int LenaDDI_GetSensorRegister(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nId,
		unsigned short address,
		unsigned short *pValue,
		int flag,
		int nSensorMode)
	\brief get value from sensor register
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nId	sensor slave address. see #SENSOR_TYPE_NAME enum definition
	\param address	register address
	\param pValue	pointer of value got from register address
	\param flag	address and value data length(2 or 1 byte)
		ie FG_Address_2Byte | FG_Value_2Byte is 2 byte address and 2 byte value
		#define FG_Address_1Byte 0x01
		#define FG_Address_2Byte 0x02
		#define FG_Value_1Byte   0x10
		#define FG_Value_2Byte   0x20
	\param nSensorMode	sensor mode(sensor A, B or Both)
		A is 0, B is 1, Both is 2
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetSensorRegister ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nId, unsigned short address, unsigned short *pValue, int flag, int nSensorMode);

/*! \fn int LenaDDI_SetSensorRegister(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nId,
		unsigned short address,
		unsigned short *pValue,
		int flag,
		int nSensorMode)
	\brief set value from sensor register
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nId	sensor slave address. see #SENSOR_TYPE_NAME enum definition
	\param address	register address
	\param pValue	pointer of value got from register address
	\param flag	address and value data length(2 or 1 byte)
		ie FG_Address_2Byte | FG_Value_2Byte is 2 byte address and 2 byte value
		#define FG_Address_1Byte 0x01
		#define FG_Address_2Byte 0x02
		#define FG_Value_1Byte   0x10
		#define FG_Value_2Byte   0x20
	\param nSensorMode	sensor mode(sensor A, B or Both)
		A is 0, B is 1, Both is 2
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetSensorRegister ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nId, unsigned short address, unsigned short nValue,  int flag, int nSensorMode);

/*! \fn int LenaDDI_GetFWRegister(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		unsigned short address,
		unsigned short *pValue,
		int flag)
	\brief get firmware register value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param address	register address
	\param pValue	pointer of value got from register address
	\param flag	address and value data length(2 or 1 byte)
		ie FG_Address_2Byte | FG_Value_2Byte is 2 byte address and 2 byte value
		#define FG_Address_1Byte 0x01
		#define FG_Address_2Byte 0x02
		#define FG_Value_1Byte   0x10
		#define FG_Value_2Byte   0x20
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetFWRegister     ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short address, unsigned short *pValue, int flag);

/*! \fn int LenaDDI_SetFWRegister(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		unsigned short address,
		unsigned short nValue,
		int flag)
	\brief set firmware register value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param address	register address
	\param nValue	register value to set
	\param flag	address and value data length(2 or 1 byte)
		ie FG_Address_1Byte | FG_Value_1Byte is 1 byte address and 1 byte value
		#define FG_Address_1Byte 0x01
		#define FG_Address_2Byte 0x02
		#define FG_Value_1Byte   0x10
		#define FG_Value_2Byte   0x20
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetFWRegister     ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short address, unsigned short nValue,  int flag);

/*! \fn int LenaDDI_GetHWRegister(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		unsigned short address,
		unsigned short *pValue,
		int flag)
	\brief get hardware register value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param address	register address
	\param pValue	pointer of value got from register address
	\param flag	address and value data length(2 or 1 byte)
		ie FG_Address_2Byte | FG_Value_2Byte is 2 byte address and 2 byte value
		#define FG_Address_1Byte 0x01
		#define FG_Address_2Byte 0x02
		#define FG_Value_1Byte   0x10
		#define FG_Value_2Byte   0x20
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetHWRegister     ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short address, unsigned short *pValue, int flag);

/*! \fn int LenaDDI_SetHWRegister(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		unsigned short address,
		unsigned short nValue,
		int flag)
	\brief set hardware register
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param address	register address
	\param nValue	register value to set
	\param flag	address and value data length(2 or 1 byte)
		ie FG_Address_1Byte | FG_Value_1Byte is 1 byte address and 1 byte value
		#define FG_Address_1Byte 0x01
		#define FG_Address_2Byte 0x02
		#define FG_Value_1Byte   0x10
		#define FG_Value_2Byte   0x20
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetHWRegister     ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short address, unsigned short nValue,  int flag);
// register APIs -

// File ID +
/*! \fn int LenaDDI_GetFwVersion(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		char *pszFwVersion,
		int nBufferSize,
		int *pActualLength)
	\brief get the firmware version of device, the version is a string
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param pszFwVersion	firmware version string
	\param nBufferSize	input buffer length to receive FW version
	\param pActualLength	the actual length of FW version in byte
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetFwVersion    ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *pszFwVersion, int nBufferSize, int *pActualLength);

/*! \fn int LenaDDI_GetPidVid(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		unsigned short *pPidBuf,
		unsigned short *pVidBuf)
	\brief get PID(product ID) and VID(vendor ID) of device
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param pPidBuf	4 byte buffer to store PID value
	\param pVidBuf	4 byte buffer to store VID value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetPidVid       ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short *pPidBuf, unsigned short *pVidBuf );

/*! \fn int LenaDDI_SetPidVid(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		unsigned short *pPidBuf,
		unsigned short *pVidBuf)
	\brief set PID and VID to device
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param pPidBuf	4 byte PID value buffer to set
	\param pVidBuf	4 byte VID value buffer to set
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetPidVid       ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short *pPidBuf, unsigned short *pVidBuf );

/*! \fn int LenaDDI_GetSerialNumber( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *pSerialNum, int nBufferSize, int *pActualLength)
	\brief get the module serial number (the devixe trace code, 14 or 17 digits of ASCII code)
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param pSerialNum	a pre-allocated buffer to store the serial number.
	\param nBufferSize	length of pBuf
	\param pActualLength	actual numbers of byte copied o the buffer	
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
	
int LenaDDI_API LenaDDI_GetSerialNumber( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *pSerialNum, int nBufferSize, int *pACtualSNLenByByte);

/*! \fn int LenaDDI_SetSerialNumber( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *pSerialNum, int nBufferSize)
	\brief set the module serial number (the device trace code, 14 or 17 digits of ASCII code)
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param pSerialNum	a pre-allocated buffer to store the serial number.
	\param nBufferSize	length of pBuf
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/	
int LenaDDI_API LenaDDI_SetSerialNumber( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *pSerialNum, int nBufferSize);
/*! \fn int LenaDDI_GetLogData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE *buffer,
		int BufferLength,
		int *pActualLength,
		int index)
	\brief get log data from flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store log data
	\param BufferLength	input buffer length
	\param pActualLength	actual length has written to buffer
	\param index	index to identify log data for corresponding depth
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetLogData      ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index);

/*! \fn int LenaDDI_GetUserData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE *buffer,
		int BufferLength,
		#USERDATA_SECTION_INDEX usi)
	\brief get user data from flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store user data
	\param BufferLength	input buffer length
	\param usi	which user index data to select
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetUserData     ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, USERDATA_SECTION_INDEX usi);

/*! \fn int LenaDDI_SetLogData(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index)
	\brief set log data to flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store log data
	\param BufferLength	input buffer length
	\param pActualLength	actual length has written to buffer
	\param index	index to identify log data for corresponding depth
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetLogData      ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index);

/*! \fn int  LenaDDI_SetUserData(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, USERDATA_SECTION_INDEX usi)
	\brief set user data to flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store user data
	\param BufferLength	input buffer length
	\param usi	which user index data to select
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetUserData     ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, USERDATA_SECTION_INDEX usi);


/*! \fn int LenaDDI_ReadFlashData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		FLASH_DATA_TYPE fdt,
		BYTE *pBuffer,
		unsigned long int nLengthOfBuffer,
		unsigned long int *pActualBufferLen)
	\brief read firmware code(.bin) form flash
		The firmware code is the combination of boot loader, firmware body and plug-in data.
		This input buffer length has to match with the flash data type
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param fdt	segment type of flash be read
	\param pBuffer	buffer to store firmware code
	\param nLengthOfBuffer	input buffer length 
	\param pActualBufferLen	actual length has written to pBuffer
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_ReadFlashData   ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, FLASH_DATA_TYPE fdt, BYTE *pBuffer, 
                                          unsigned long int nLengthOfBuffer, unsigned long int *pActualBufferLen);

										  
										  int LenaDDI_API LenaDDI_WriteFlashData  ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, FLASH_DATA_TYPE fdt, BYTE *pBuffer, 
                                          unsigned long int nLengthOfBuffer, BOOL bIsDataVerify, KEEP_DATA_CTRL kdc);

/*! \fn int  LenaDDI_GetFileData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nID, int DataSize,
		BYTE *lpData)
	\brief get file data from flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param nID	file ID
	\param DataSize	data size
	\param lpData	data stored buffer
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/                                          
int LenaDDI_API LenaDDI_GetFileData     (void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nID, int DataSize, BYTE *lpData);

/*! \fn int  LenaDDI_SetFileData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nID, int DataSize,
		BYTE *lpData)
	\brief set file data to flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param nID	file ID
	\param DataSize	data size
	\param lpData	data stored buffer
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetFileData     (void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nID, int DataSize, BYTE *lpData);                                          
// File ID -

// Image +
/*! \fn int LenaDDI_OpenDevice( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nWidth, 
                        int nHeight, bool bImageL, bool bDepth,
                        void *phWndNotice, BYTE bCtrlMode)
	\brief open camera device with color stream only, check with LenaDDI_OpenDevice2 API
*/
int LenaDDI_API LenaDDI_OpenDevice( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
                                    int nWidth, int nHeight, bool bImageL, bool bDepth,
                                    void *phWndNotice=NULL, BYTE bCtrlMode=0);
/*! \fn int LenaDDI_CloseDevice(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief close device and stop video render
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\return success:LenaDDI_OK, others:see eSPDI_ErrCode.h
*/                                     
int LenaDDI_API LenaDDI_CloseDevice( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn int LenaDDI_GetImage(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *pBuf, unsigned long int *pImageSize, int *pSerial)
	\brief get color image, see LenaDDI_Get2Image for details
*/
int LenaDDI_API LenaDDI_GetImage ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
                                   BYTE *pBuf, unsigned long int *pImageSize, int *pSerial = NULL);

/*! \fn int LenaDDI_Get2Image(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE *pImageLBuf,
		BYTE *pDepthBuf,
		unsigned long int *pColorImageSize,
		unsigned long int *pDepthImageSize,
		int *pSerial,
		int *pSerial2)
	\brief get color and depth images
		set the SetCallback function of ISampleGrabberCB to recieive oncoming samples
		The ISampleGrabber interface is exposed by the Sample Grabber filter. It enables
		an application to retrieve individual media samples as they move through the filter
		graph.
		The SetCallback method specifies a callback method to call on incoming samples.
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param pImageLBuf	buffer to store color image
	\param pDepthBuf	buffer to store depth image
	\param pColorImageSize	actual output size of pImageLBuf
	\param pDepthImageSize	actual output size of pDepthBuf
	\param pSerial	 pointer of color serial(or timestamp) number
	\param pSerial2	pointer of depth serial(or timestamp) number
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/                                   
int LenaDDI_API LenaDDI_Get2Image( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
                                   BYTE *pImageLBuf, BYTE *pDepthBuf, 
                                   unsigned long int *pColorImageSize, unsigned long int *pDepthImageSize, 
                                   int *pSerial=NULL, int *pSerial2 = NULL);

/*! \fn int LenaDDI_Get2ImageEx(
		void* pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE* imgBuf,
		unsigned long int* imgSize,
		int* imgSerial,
		int depthCount,
		BYTE** depthBuf,
		unsigned long int* depthSize,
		int* depthSerial)
	\brief get color and depth (can be more than 1, ie 2 or 3) images
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param imgBuf	buffer to store color image
	\param imgSize	actual output size of imgBuf
	\param imgSerial	
	\param depthCount	number of depth image to get
	\param depthBuf		buffer to store depth image
	\param depthSize	actual output size of pDepthBuf
	\param depthSerial  pointer of depth serial(or timestamp) number
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h 
*/
int LenaDDI_API LenaDDI_Get2ImageEx(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo,
                                    BYTE* imgBuf, unsigned long int* imgSize, int* imgSerial, 
                                    int depthCount, BYTE** depthBuf, unsigned long int* depthSize, int* depthSerial);

/*! \fn int LenaDDI_OpenDevice2(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nEP0Width,
		int nEP0Height,
		BOOL bEP0MJPG,
		int nEP1Width,
		int nEP1Height,
		BOOL bIsOutputRGB24,
		void *phWndNotice,
		int *nFPS,
		BYTE bCtrlMode)
	\brief open LenaD camera device
		1. open input resolution mode
		2. start video render processes by IGraphBuilder(inherits from IFilterGraph) interface of MSFT's DDraw
		3. This interface provides methods that enable an application to build a filter graph.
		  The Filter Graph Manager implements this interface.
		  The IGraphBuilder interface inherits from the IFilterGraph interface. IFilterGraph provides basic operations,
		  such as adding a filter to the graph or connecting two pins. IGraphBuilder adds further methods that
		  construct graphs from partial information. For example, the IGraphBuilder::RenderFile method builds a graph
		  for file playback, given the name of the file. The IGraphBuilder::Render method renders data from an output
		  pin by connecting new filters to the pin.
		  Using these methods, an application does not need to specify every filter and pin connection in the graph.
		  Instead, the Filter Graph Manager selects filters that are registered on the user's system, adds them to the
		  graph, and connects them. For more information, see Intelligent Connect.
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param nEP0Width	width of endpoint1(color) resolution
	\param nEP0Height	height of endpoint 1(color) resolution
	\param bEP0MJPG	Is endpoint1 output is MJPEG ?
	\param nEP1Width	width of endpoint2(depth) resolution
	\param nEP1Height	height of endpoint2(depth) resolution
	\param bIsOutputRGB24	output image is RGB format
	\param phWndNotice	window handle to receive frame notified message if assigned
	\param nFPS	pointer to the desired frame rate, returns the actual frame rate.
	\param ctrlMode image display options of the specified stream
<table>
<caption id="multi_row">Image Control Mode</caption>
<tr><th>Mode        <th>Description
<tr><td>0x01 <td>color and depth frame output synchrously, for depth map module only 
<tr><td>0x02 <td>enable post-process, for Depth Map module only
<tr><td>0x04 <td>stitch images if this bit is set, for fisheye spherical module only 
<tr><td>0x08 <td>use OpenCL in stitching. This bit effective only when bit-2 is set.
</table>
	\return success:LenaDDI_OK, others:see eSPDI_ErrCode.h
 */
int LenaDDI_API LenaDDI_OpenDevice2( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
                                     int nEP0Width, int nEP0Height, BOOL bEP0MJPG, 
                                     int nEP1Width, int nEP1Height,
                                     BOOL bIsOutputRGB24=FALSE, void *phWndNotice=NULL, 
                                     int *nFPS=NULL, BYTE bCtrlMode=0);

									 
/*! \fn int LenaDDI_OpenDeviceEx(
		void* pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int colorStreamIndex,
		bool toRgb,
		int depthStreamIndex,
		int depthStreamSwitch,
		LenaDDI_ImgCallbackFn callbackFn,
		void* pCallbackParam,
		int* pFps,
		BYTE ctrlMode)
	\brief open camera device with image callback support, check with LenaDDI_OpenDevice2 API
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param colorStreamIndex	index of the desired color stream
	\param toRgb	set as true if the color stream is RGB format, otherwise, the callback will recived original color format from device. 
	\param depthStreamIndex index of the desired sdepth tream 
	\param depthStreamSwitch	depth switch for S0, S1 or S2
	\param callbackFn	set image callback function
	\param pCallbackParam	the data to associate with the callback function
	\param pFps pointer to the desired frame rate, returns the actual frame rate.
	\param ctrlMode image display options of the specified stream
<table>
<caption id="multi_row">Image Control Mode</caption>
<tr><th>Mode        <th>Description
<tr><td>0x01 <td>color and depth frame output synchrously, for depth map module only 
<tr><td>0x02 <td>enable post-process, for Depth Map module only
<tr><td>0x04 <td>stitch images if this bit is set, for fisheye spherical module only 
<tr><td>0x08 <td>use OpenCL in stitching. This bit effective only when bit-2 is set.
</table>
	\return success:LenaDDI_OK, others:see eSPDI_ErrCode.h
 */
int LenaDDI_API LenaDDI_OpenDeviceEx(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
                                     int colorStreamIndex, bool toRgb, int depthStreamIndex, int depthStreamSwitch, 
                                     LenaDDI_ImgCallbackFn callbackFn, void* pCallbackParam, int* pFps = nullptr, BYTE ctrlMode = 0);

/*! \fn int LenaDDI_GetDeviceResolutionList(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nMaxCount0,
		LenaDDI_STREAM_INFO *pStreamInfo0, 
             int nMaxCount1,
             LenaDDI_STREAM_INFO *pStreamInfo1)
	\brief get the device resolution list
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param nMaxCount0	max count of endpoint1 resolutions
	\param pStreamInfo0	resolution infos of endpoint1
	\param nMaxCount1	max count of endpoint2 resolutions
	\param pStreamInfo1	resolutions infos of endpoint2
	\return success: nCount0*256+nCount1, others: see eSPDI_ErrCode.h
 */									 
int LenaDDI_API LenaDDI_GetDeviceResolutionList( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
                                                 int nMaxCount0, LenaDDI_STREAM_INFO *pStreamInfo0, 
                                                 int nMaxCount1, LenaDDI_STREAM_INFO *pStreamInfo1);

/*! \fn bool LenaDDI_Is360Device(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief check module is spherical device or not
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\return true: module support 360, false: not support
*/
bool LenaDDI_API LenaDDI_Is360Device(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);
// Image -                                                  

// for callback function +
typedef struct tagCallbackFunction {
  int nColorSerialNumber;
  int nDepthSerialNumber;
} CALLBACKFUNCTION_PARAMETER;

typedef void (CALLBACK *fnCallBackFunc)(CALLBACKFUNCTION_PARAMETER CallBackFuncPara);

/*! \fn int LenaDDI_RegisterFrameNoticeCBFun(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, fnCallBackFunc FrameNoticeFunc)
	\brief register the frame callback function for 'LenaDDI_ImgCallbackFn callbackFn' of LenaDDI_OpenDeviceEx function
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param FrameNoticeFunc	set frame callback function
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_RegisterFrameNoticeCBFun( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, fnCallBackFunc FrameNoticeFunc);
// for callback function -

/*! \fn int LenaDDI_GetResolution_ColorAndDepth( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int *pcxColor, int *pcyColor, int *pcxDepth, int *pcyDepth)
	\brief get color and depth resluton from rectify table, only for AXES chip
*/	
int LenaDDI_API LenaDDI_GetResolution_ColorAndDepth( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int *pcxColor, int *pcyColor, int *pcxDepth, int *pcyDepth);

/*! \fn int LenaDDI_BuildMappingTable_FromColorToRect(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int cxColor, int cyColor, int cxDepth, int cyDepth, short *pMapAddrX, short *pMapAddrY)
	\brief buid a look up table from color to rectify, only for AXES chip
*/	
int LenaDDI_API LenaDDI_BuildMappingTable_FromColorToRect( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int cxColor, int cyColor, int cxDepth, int cyDepth, short *pMapAddrX, short *pMapAddrY);

/*! \fn int LenaDDI_BuildColorSize_Depthmap(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *pDstColorDepth, BYTE *pSrcDepth, int cxColor, int cyColor, int cxDepth, int cyDepth, short *pMapAddrX, short *pMapAddrY)
	\brief depth map remap to color
*/
int LenaDDI_API LenaDDI_BuildColorSize_Depthmap( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *pDstColorDepth, BYTE *pSrcDepth, int cxColor, int cyColor, int cxDepth, int cyDepth, short *pMapAddrX, short *pMapAddrY);

/*! \fn int LenaDDI_MergeRB_FromColorAndDepthA( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *lpszFileNameColor, char *lpszFileNameDepth, 
		char *lpszFileNameDstRB, char *lpszErrText, int nErrTextBufLen)
	\brief legacy function		
*/
int LenaDDI_API LenaDDI_MergeRB_FromColorAndDepthA( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *lpszFileNameColor, char *lpszFileNameDepth, 
                                                    char *lpszFileNameDstRB, char *lpszErrText, int nErrTextBufLen);

/*! int LenaDDI_MergeRB_FromColorAndDepthW( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WCHAR *lpwszFileNameColor, WCHAR *lpwszFileNameDepth, 
		WCHAR *lpwszFileNameDstRB, WCHAR *lpszErrText, int nErrTextBufLen)
	\brief legacy function		
*/													
int LenaDDI_API LenaDDI_MergeRB_FromColorAndDepthW( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WCHAR *lpwszFileNameColor, WCHAR *lpwszFileNameDepth, 
                                                    WCHAR *lpwszFileNameDstRB, WCHAR *lpszErrText, int nErrTextBufLen);

/*! \fn int LenaDDI_GetSerialNumberFromLog( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *pSerialNum, int nBufferSize, int *pActualLength)
	\brief get the module serial number 
*/
int LenaDDI_API LenaDDI_GetSerialNumberFromLog( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, char *pSerialNum, int nBufferSize, int *pActualLength);

// IR support
/*! \fn int LenaDDI_SetCurrentIRValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD wType)
	\brief set current infrared radiation(IR) value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param wType	value to set
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetCurrentIRValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD wType);

/*! \fn int LenaDDI_GetCurrentIRValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType)
	\brief get current infrared radiation(IR) value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index	
	\param pwType	value of current IR
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h	
*/
int LenaDDI_API LenaDDI_GetCurrentIRValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType);

/*! \fn int LenaDDI_GetIRMinValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType)
	\brief get minimum IR value the module support
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index	
	\param pwType	pointer strors minimum IR value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h	
*/
int LenaDDI_API LenaDDI_GetIRMinValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType);

/*! \fn int LenaDDI_GetIRMaxValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType)
	\brief get maximum IR value the module support
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index	
	\param pwType	pointer strors maximum IR value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h	
*/
int LenaDDI_API LenaDDI_GetIRMaxValue(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType);

/*! \fn int LenaDDI_SetIRMode(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD wType)
	\brief set IR mode, left, right or both
*/
int LenaDDI_API LenaDDI_SetIRMode(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD wType);

/*! \fn int LenaDDI_GetIRMode(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType)
	\brief set IR mode, left, right or both
*/
int LenaDDI_API LenaDDI_GetIRMode(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType);

// for config control mode +

/*! \fn int LenaDDI_ConfigCtrlMode(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE bCtrlMode)
	\brief config for image render,
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param BYTE ctrlMode options of this stream:
		0x01	color and depth frame output synchrously, for depth map module only
		0x02 enable post-process, for Depth Map module only
		0x04 stitch images if this bit is set, for fisheye spherical module only
		0x08 use OpenCL in stitching. This bit effective only when bit-2 is set.
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_ConfigCtrlMode(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE bCtrlMode);
// for config control mode -

// for sensorif
/*! \fn int LenaDDI_EnableSensorIF( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool bIsEnable)
	\brief turn on/off sensor IF function
*/
int LenaDDI_API LenaDDI_EnableSensorIF( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool bIsEnable);

 
//int LenaDDI_API LenaDDI_GetMotorCurrentState( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool* bIsRunning);

// for Gyro

typedef enum
{
    DPS_245 = 0,
    DPS_500,
    DPS_2000
} SENSITIVITY_LEVEL_L3G;
 
// bPowerMode :
//     true  : Normal
//     false : Power Down
// bIsZEnable :
//     true  : Enable
//     false : Disable
// bIsYEnable :
//     true  : Enable
//     false : Disable
// bIsXEnable :
//     true  : Enable
//     false : Disable

//typedef struct GyroTag
//{
//	short x; 		
//  short y;
//  short z;
//} GYRO_ANGULAR_RATE_DATA;


/*! \enum sensor model name */
typedef enum
{
    LenaDDI_SENSOR_TYPE_H22 = 0,	/**< H22 */
    LenaDDI_SENSOR_TYPE_OV7740,		/**< OV7740 */			
    LenaDDI_SENSOR_TYPE_AR0134,		/**< AR0134 */
    LenaDDI_SENSOR_TYPE_AR0135		/**< AR0135 */
} SENSOR_TYPE_NAME; 

//
// Sensor Mode: Left, Right, or Both
//
#define ESPAEAWB_SENSOR_MODE_LEFT		0
#define ESPAEAWB_SENSOR_MODE_RIGHT      1
#define ESPAEAWB_SENSOR_MODE_BOTH		2
//
// PU Property ID
//
#define    PU_PROPERTY_ID_BRIGHTNESS                0
#define    PU_PROPERTY_ID_CONTRAST                  1
#define    PU_PROPERTY_ID_HUE                       2
#define    PU_PROPERTY_ID_SATURATION                3
#define    PU_PROPERTY_ID_SHARPNESS                 4
#define    PU_PROPERTY_ID_GAMMA                     5
#define    PU_PROPERTY_ID_COLORENABLE               6
#define    PU_PROPERTY_ID_WHITEBALANCE              7
#define    PU_PROPERTY_ID_BACKLIGHT_COMPENSATION    8
#define    PU_PROPERTY_ID_GAIN                      9
#define    PU_PROPERTY_ID_DIGITAL_MULTIPLIER        10
#define    PU_PROPERTY_ID_DIGITAL_MULTIPLIER_LIMIT  11
#define    PU_PROPERTY_ID_WHITEBALANCE_COMPONENT    12
#define    PU_PROPERTY_ID_POWERLINE_FREQUENCY       13
//
// CT Property ID
//
#define    CT_PROPERTY_ID_PAN               		   0
#define    CT_PROPERTY_ID_TILT                         1
#define    CT_PROPERTY_ID_ROLL                         2
#define    CT_PROPERTY_ID_ZOOM                         3
#define    CT_PROPERTY_ID_EXPOSURE                     4
#define    CT_PROPERTY_ID_IRIS                         5
#define    CT_PROPERTY_ID_FOCUS                        6
#define    CT_PROPERTY_ID_SCANMODE                     7
#define    CT_PROPERTY_ID_PRIVACY                      8
#define    CT_PROPERTY_ID_PANTILT                      9
#define    CT_PROPERTY_ID_PAN_RELATIVE                 10
#define    CT_PROPERTY_ID_TILT_RELATIVE                11
#define    CT_PROPERTY_ID_ROLL_RELATIVE                12
#define    CT_PROPERTY_ID_ZOOM_RELATIVE                13
#define    CT_PROPERTY_ID_EXPOSURE_RELATIVE            14
#define    CT_PROPERTY_ID_IRIS_RELATIVE                15
#define    CT_PROPERTY_ID_FOCUS_RELATIVE               16
#define    CT_PROPERTY_ID_PANTILT_RELATIVE             17
#define    CT_PROPERTY_ID_AUTO_EXPOSURE_PRIORITY       19 

/*! \fn int LenaDDI_SetSensorTypeName(
		void *pHandleLenaDDI,
		SENSOR_TYPE_NAME stn)
	\brief select which sensor to operate
	\param pHandleLenaDDI	CLenaDDI handler
	\param stn	sensor type
	\return LenaDDI_OK
*/                        
int LenaDDI_API LenaDDI_SetSensorTypeName( void *pHandleLenaDDI, SENSOR_TYPE_NAME stn);

/*! \fn int LenaDDI_EnableAE(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief enable auto exposure function of ISP
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_EnableAE  ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn int LenaDDI_DisableAE(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief disable auto exposure function of ISP
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_DisableAE ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn int LenaDDI_EnableAWB(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief enable auto white balance function of ISP
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_EnableAWB ( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn int LenaDDI_DisableAWB(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief disable auto white balance of ISP
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_DisableAWB( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);


/*! \fn int LenaDDI_GetExposureTime(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nSensorMode,
		float *pfExpTimeMS)
	\brief get exposure time of ISP setting in millisecond
		the target sensor type was set in LenaDDI_SetSensorTypeName()
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nSensorMode	which sensor(sensor A, B or Both) to get
		A is 0, B is 1, Both is 2
	\param pfExpTimeMS	pointer of getting exposure time in millisecond
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetExposureTime( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nSensorMode, float *pfExpTimeMS);

/*! \fn int LenaDDI_SetExposureTime(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nSensorMode,
		float fExpTimeMS)
	\brief set exposure time of ISP sensor setting
		the target sensor type was set in LenaDDI_SetSensorTypeName()
	\param *pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nSensorMode	which sensor(sensor A, B or Both) to set
		A is 0, B is 1, Both is 2
	\param fExpTimeMS	pointer of setting exposure time in millisecond
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetExposureTime( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nSensorMode, float fExpTimeMS);

/*! \fn int LenaDDI_GetGlobalGain(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nSensorMode,
		float *pfGlobalGain)
	\brief get global gain of ISP setting
		the target sensor type was set in LenaDDI_SetSensorTypeName()
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nSensorMode	which sensor(sensor A, B or Both) to get
		A is 0, B is 1, Both is 2
	\param pfGlobalGain	pointer of global gain value
		see source code for value calculation of different sensor type
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetGlobalGain( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nSensorMode, float *pfGlobalGain);

/*! \fn int LenaDDI_SetGlobalGain(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nSensorMode,
		float fGlobalGain)
	\brief set global gain of ISP sensor setting
		the target sensor type was set in LenaDDI_SetSensorTypeName()
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nSensorMode	which sensor(sensor A, B or Both) to get
		A is 0, B is 1, Both is 2
	\param fGlobalGain	pointer of global gain value
		see source code for value calculation of different sensor type
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetGlobalGain( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nSensorMode, float fGlobalGain);

/*! \fn int LenaDDI_GetColorGain(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nSensorMode,
		float *pfGainR,
		float *pfGainG,
		float *pfGainB)
	\brief get color gain of ISP setting
		the target sensor type was set in LenaDDI_SetSensorTypeName()
	\param *pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nSensorMode	which sensor(sensor A, B or Both) to get
		A is 0, B is 1, Both is 2
	\param pfGainR	pointer of red gain value of ISP setting
	\param pfGainG	pointer of green gain value of ISP setting
	\param pfGainB	pointer of blue gain value of ISP setting
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetColorGain( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nSensorMode, float *pfGainR, float *pfGainG, float *pfGainB);

/*! \fn int LenaDDI_SetColorGain(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nSensorMode,
		float fGainR,
		float fGainG,
		float fGainB)
	\brief set color gain of ISP
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nSensorMode	which sensor(sensor A, B or Both) to get
		A is 0, B is 1, Both is 2
	\param fGainR	Red channel color gain value
	\param fGainG	Green channel color gain value
	\param fGainB	Blue channel color gain value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetColorGain( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nSensorMode, float fGainR, float fGainG, float fGainB);

/*! \fn int LenaDDI_GetGPIOValue(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nGPIOIndex,
		BYTE *pValue)
	\brief get general purpose IO value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index	
	\param nGPIOIndex	GPIO index, 1 or 2 is valid
	\param pValue	pointer of GPIO value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetGPIOValue( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nGPIOIndex, BYTE *pValue);

/*! \fn int LenaDDI_SetGPIOValue(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nGPIOIndex,
		BYTE nValue)
	\brief set GPIO value
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nGPIOIndex	GPIO index, 1 or 2 is valid
	\param nValue	GPIO value to set
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetGPIOValue( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nGPIOIndex, BYTE nValue);

/*! \fn int LenaDDI_SetGPIOCtrl(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nGPIOIndex, BYTE nValue)
	\brief set GPIO control address
	\param nGPIOIndex	index of GPIO (1 ~ 4)
	\param nValue	register value to set
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetGPIOCtrl(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nGPIOIndex, BYTE nValue);

int LenaDDI_API LenaDDI_GetAccMeterValue( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int *pX, int *pY, int *pZ);

/*! \fn int LenaDDI_GetPUPropVal(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nId,
		int *pValue)
	\brief get processing unit property value
		https://msdn.microsoft.com/en-us/library/windows/hardware/ff568122(v=vs.85).aspx
		The PROPSETID_VIDCAP_VIDEOPROCAMP property set controls devices that can adjust
		image color attributes of analog or digital signals.

		The KSPROPERTY_VIDCAP_VIDEOPROCAMP enumeration in ksmedia.h specifies the
		properties of this set.

		Support for this property set is optional and should be implemented only by devices that
		allow adjustment of brightness, contrast, hue, and other image quality settings.

		Prior to USB video class, this enumeration contained the following property items:
		KSPROPERTY_VIDEOPROCAMP_BACKLIGHT_COMPENSATION
		KSPROPERTY_VIDEOPROCAMP_BRIGHTNESS
		KSPROPERTY_VIDEOPROCAMP_COLORENABLE
		KSPROPERTY_VIDEOPROCAMP_CONTRAST
		KSPROPERTY_VIDEOPROCAMP_GAMMA
		KSPROPERTY_VIDEOPROCAMP_HUE
		KSPROPERTY_VIDEOPROCAMP_SATURATION
		KSPROPERTY_VIDEOPROCAMP_SHARPNESS
		KSPROPERTY_VIDEOPROCAMP_WHITEBALANCE
		KSPROPERTY_VIDEOPROCAMP_GAIN

		https://msdn.microsoft.com/en-us/library/windows/hardware/ff566089(v=vs.85).aspx
		The KSPROPERTY_VIDEOPROCAMP_S structure describes filter-based property settings
		in the PROPSETID_VIDCAP_VIDEOPROCAMP property set.
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nId	specifies the member of the property set
	\param pValue	pointer of store PU property value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetPUPropVal( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nId, int *pValue);

/*! \fn int LenaDDI_SetPUPropVal(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nId,
		int nValue)
	\brief get processing unit property value
		https://msdn.microsoft.com/en-us/library/windows/hardware/ff568122(v=vs.85).aspx
		https://msdn.microsoft.com/en-us/library/windows/hardware/ff566089(v=vs.85).aspx
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nId	specifies the member of the property set
	\param nValue	value to set
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetPUPropVal( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nId, int nValue);

/*! \fn int LenaDDI_GetCTPropVal(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nId,
		int *pValue)
	\brief set control terminal property value
		https://msdn.microsoft.com/en-us/library/windows/hardware/ff567802(v=vs.85).aspx
		The PROPSETID_VIDCAP_CAMERACONTROL property set controls camera device settings.
		The controls it provides are a subset of the ITU T.RDC standard.

		The KSPROPERTY_VIDCAP_CAMERACONTROL enumeration in Ksmedia.h specifies the
		properties of this set.

		Support for this property set is optional and should be implemented only by minidrivers of
		devices that offer camera control settings. For more information, see the ITU website.

		Prior to USB video class, this enumeration contained the following properties:
		KSPROPERTY_CAMERACONTROL_EXPOSURE
		KSPROPERTY_CAMERACONTROL_FOCUS
		KSPROPERTY_CAMERACONTROL_IRIS
		KSPROPERTY_CAMERACONTROL_ZOOM
		KSPROPERTY_CAMERACONTROL_PAN
		KSPROPERTY_CAMERACONTROL_ROLL
		KSPROPERTY_CAMERACONTROL_TILT

		https://msdn.microsoft.com/en-us/library/windows/hardware/ff566089(v=vs.85).aspx
	\param *pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nId	specifies the member of the property set
	\param pValue	pointer of store CT property value
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetCTPropVal( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nId, int *pValue);

/*! \fn int LenaDDI_SetCTPropVal(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int nId,
		int nValue)
	\brief get control terminal property value
		https://msdn.microsoft.com/en-us/library/windows/hardware/ff567802(v=vs.85).aspx
		https://msdn.microsoft.com/en-us/library/windows/hardware/ff566089(v=vs.85).aspx
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param nId	specifies the member of the property set
	\param nValue	CT property value to set
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetCTPropVal( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int nId, int nValue);

// For AEAWB - 2015/01/28 by Wolf

int LenaDDI_API LenaDDI_EncryptMP4(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, const char* filename);
int LenaDDI_API LenaDDI_DecryptMP4(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, const char* filename);
int LenaDDI_API LenaDDI_RetrieveMp4ExtraData(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
    const char* filename, char* dataBuf, int* dataSize);
int LenaDDI_API LenaDDI_FlushMp4ExtraData(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, 
    const char* filename, const char* dataBuf, int dataSize);
/*! \fn int LenaDDI_GetAutoExposureMode(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short* mode)
	\brief misc function : get auto exposure mode
*/	
int LenaDDI_API LenaDDI_GetAutoExposureMode(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short* mode);

/*! \fn int LenaDDI_SetAutoExposureMode(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short mode)
	\brief misc function : set auto exposure mode
*/
int LenaDDI_API LenaDDI_SetAutoExposureMode(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short mode);

/*! \fn int LenaDDI_GetFlexibleGyroData(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo,
	    int length, BYTE *pGyroData)
	\brief get IMU(Gyro) data
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param length	length of IMU data to read, should be get from LenaDDI_GetFlexibleGyroLength
	\param pGyroData	data buffer to store IMU data
*/
int LenaDDI_API LenaDDI_GetFlexibleGyroData(void * pHandleLenaDDI, PDEVSELINFO pDevSelInfo,
    int length, BYTE *pGyroData);

/*! \fn int LenaDDI_GetFlexibleGyroLength(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short* GyroLen)
	\brief get the IMU(Gyro) data length
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param GyroLen	pointer to store IMU data length
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/	
int LenaDDI_API LenaDDI_GetFlexibleGyroLength(void* pHandleLenaDDI, PDEVSELINFO pDevSelInfo, unsigned short* GyroLen);

/*! \fn int LenaDDI_SetHuffmanTableData(
	void *pHandleLenaDDI,
	PDEVSELINFO pDevSelInfo,
	const char *filename,
	bool bLogFile)
	\brief set huffman table data for jpeg encode
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param filename	huffman table file, see jh_vga_422.dat sample file
	\param bLogFile	if true then puma_htable.dat file is generated
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetHuffmanTableData(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, const char *filename, bool bLogFile);

/*! \fn int LenaDDI_SetQuantizationTableData(
	void *pHandleLenaDDI,
	PDEVSELINFO pDevSelInfo,
	const char *filename)
	\brief set quantication table data for jpeg encode
	\param pHandleLenaDDI	CLenaDDI handler
	\param pDevSelInfo	pointer of device select index
	\param filename	quantization table file, see FS_DEF_010.txt sample file
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetQuantizationTableData(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, const char *filename);

/*! \fn int LenaDDI_EnableImageMirror( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool enable)
\brief turn on/off image mirror
*/
int LenaDDI_API LenaDDI_EnableImageMirror(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool enable);

#ifdef __cplusplus
}
#endif
