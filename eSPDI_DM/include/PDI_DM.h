 /*! \file eSPDI_DM.h
  	\brief LenaD SDK API export functions, data structure and variable definition for depth map module
  	\copyright This file copyright (C) 2017 by LenaCV company

	\details An unpublished work.  All rights reserved.
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of LenaCV company
 */
#pragma once
#include "eSPDI_Common.h"

// for LenaDDI_PostSetParam/LenaDDI_PostGetParam
#define POSTPAR_HR_MODE        5
#define POSTPAR_HR_CURVE_0     6
#define POSTPAR_HR_CURVE_1     7
#define POSTPAR_HR_CURVE_2     8
#define POSTPAR_HR_CURVE_3     9
#define POSTPAR_HR_CURVE_4    10
#define POSTPAR_HR_CURVE_5    11
#define POSTPAR_HR_CURVE_6    12
#define POSTPAR_HR_CURVE_7    13
#define POSTPAR_HR_CURVE_8    14
#define POSTPAR_HF_MODE       17
#define POSTPAR_DC_MODE       20
#define POSTPAR_DC_CNT_THD    21
#define POSTPAR_DC_GRAD_THD   22
#define POSTPAR_SEG_MODE      23
#define POSTPAR_SEG_THD_SUB   24
#define POSTPAR_SEG_THD_SLP   25
#define POSTPAR_SEG_THD_MAX   26
#define POSTPAR_SEG_THD_MIN   27
#define POSTPAR_SEG_FILL_MODE 28
#define POSTPAR_HF2_MODE      31
#define POSTPAR_GRAD_MODE     34
#define POSTPAR_TEMP0_MODE    37
#define POSTPAR_TEMP0_THD     38
#define POSTPAR_TEMP1_MODE    41
#define POSTPAR_TEMP1_LEVEL   42
#define POSTPAR_TEMP1_THD     43
#define POSTPAR_FC_MODE       46
#define POSTPAR_FC_EDGE_THD   47
#define POSTPAR_FC_AREA_THD   48
#define POSTPAR_MF_MODE       51
#define POSTPAR_ZM_MODE       52
#define POSTPAR_RF_MODE       53
#define POSTPAR_RF_LEVEL      54

//
// C++ compatibility
//
#ifdef  __cplusplus
extern "C" {
#endif

/*! \fn int LenaDDI_GetYOffset(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE *buffer,
		int BufferLength,
		int *pActualLength,
		int index)
	\brief get Y offset data
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store
	\param BufferLength	length of buffer
	\param pActualLength	actual byte of reading
	\param index	index of Y offset file ID
	\return success:LenaDDI_OK, others:see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetYOffset(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index);

/*!	\fn LenaDDI_GetRectifyTable(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE *buffer,
		int BufferLength,
		int *pActualLength,
		int index)
	\brief get rectify values from flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store rectify table data
	\param BufferLength	input buffer length
	\param pActualLength	actual length has written to buffer
	\param index	index to identify rectify table for corresponding depth
	\return success:LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetRectifyTable(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index);

/*! \fn int LenaDDI_GetZDTable(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		BYTE *buffer,
		int BufferLength,
		int *pActualLength,
		PZDTABLEINFO pZDTableInfo)
	\brief get disparity and Z values from flash
		1. if depth data type is LenaDDI_DEPTH_DATA_14_BITS then
			just get Z value from depth buffer 
		2. if depth data type is LenaDDI_ZD_TABLE_FILE_SIZE_11_BITS then
			using depth buffer value as a index to get Z value inside ZD table
		3. see GetZValue() of example.c to get Z value from different depth data type
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	bufer to store ZD table
	\param BufferLength	input buffer length
	\param pActualLength	actual length has written to buffer
	\param pZDTableInfo	index to identify ZD table and data type for corrresponding depth
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetZDTable(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, PZDTABLEINFO pZDTableInfo);

/*! \fn int LenaDDI_SetYOffset(
		void *pHandleLenaDDI, 
		PDEVSELINFO pDevSelInfo, 
		BYTE *buffer, 
		int BufferLength, 
		int *pActualLength, 
		int index)
	\brief set Y offset data
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param buffer	buffer to store
	\param BufferLength	length of buffer
	\param pActualLength	actual byte of reading
	\param index	index of Y offset file ID
	\return success:LenaDDI_OK, others:see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetYOffset(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index);

/*! \fn int LenaDDI_SetRectifyTable(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index)
	\brief get rectify data to flash, see LenaDDI_GetRectifyTable except get
*/
int LenaDDI_API LenaDDI_SetRectifyTable(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, int index);

/*! \fn LenaDDI_SetZDTable(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, PZDTABLEINFO pZDTableInfo)
	\brief set disparity and Z values to flash, see LenaDDI_GetZDTable except get
*/
int LenaDDI_API LenaDDI_SetZDTable(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, BYTE *buffer, int BufferLength, int *pActualLength, PZDTABLEINFO pZDTableInfo);

/*! \fn int LenaDDI_GetRectifyLogData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		eSPCtrl_RectLogData *pData,
		int index)
	\brief get rectify log data from flash
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param pData	 4096 bytes of rectify log data,
		see eSPCtrl_RectLogData for detailed members
	\param index	index to identify rectify log data for corresponding depth
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetRectifyLogData(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, eSPCtrl_RectLogData *pData, int index);

/*! \fn int LenaDDI_GetRectifyMatLogData(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		eSPCtrl_RectLogData *pData,
		int index)
	\brief get rectify log data from flash for Puma IC
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param pData	 rectify log data, its buffer size is 4096 bytes  
		see eSPCtrl_RectLogData for detailed members
	\param index	index to identify rectify log data for corresponding depth
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetRectifyMatLogData(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, eSPCtrl_RectLogData *pData, int index);

/*! \fn int LenaDDI_SaveRectifyLogDataA( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, eSPCtrl_RectLogData *pData, char *lpszFileName)
	\brief save the rectify log data
*/
int LenaDDI_API LenaDDI_SaveRectifyLogDataA(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, eSPCtrl_RectLogData *pData, char *lpszFileName);


/*! \fn int LenaDDI_SaveRectifyLogDataW( void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, eSPCtrl_RectLogData *pData, WCHAR *lpwszFileName)
	\brief save the rectify log data in union-code
*/
int LenaDDI_API LenaDDI_SaveRectifyLogDataW(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, eSPCtrl_RectLogData *pData, WCHAR *lpwszFileName);

/*! \fn LenaDDI_SetDepthDataType(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		WORD wType)
	\brief set depth data type, 11 bit for disparity data, 14 bit for Z data
		notice: only PUMA type IC can support this setting
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param wType	depth data type you want to set,
		see LenaDDI_DEPTH_DATA_xxx in LenaDDI_O.h
	\output success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetDepthDataType(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD wType);

/*! \fn int LenaDDI_GetDepthDataType(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		WORD *pwType)
	\brief get current depth data type setting
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param pwType	pointer of current depth data type in device
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_GetDepthDataType(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, WORD *pwType);

/*! \fn int LenaDDI_SetHWPostProcess(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool enable)
	\brief enable or disable internal chip post processing function
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param enable	set true to enable post-process, or set false to disable post-process
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_SetHWPostProcess(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool enable);

/*! \fn int LenaDDI_GetHWPostProcess(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool* enable)
	\brief get hardware post processing status
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param enable	returns current hardware post-process status
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h	
*/
int LenaDDI_API LenaDDI_GetHWPostProcess(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool* enable);

/*! \fn int LenaDDI_PostProcessInitial( void *pHandleLenaDDI)
	\brief initialize software post processing
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h	
*/
int LenaDDI_API LenaDDI_PostProcessInitial(void *pHandleLenaDDI);

/*! \fn int LenaDDI_PostProcessEnd( void *pHandleLenaDDI)
	\brief release software post processing
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_PostProcessEnd(void *pHandleLenaDDI);

/*! \fn int LenaDDI_ProcessFrame( void *pHandleLenaDDI, unsigned char *pYUY2Buf, unsigned char *pDepthBuf, unsigned char *pOutputBuf, int width, int height)
	\brief by SW post process to enhance depth image
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pYUY2Buf	input YUY2 color image
	\param pDepthBuf	input depth buffer
	\param pOutputBuf	the result image
	\param width	image width
	\param height	image heigh
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_ProcessFrame(void *pHandleLenaDDI, unsigned char *pYUY2Buf, unsigned char *pDepthBuf, unsigned char *pOutputBuf, int width, int height);

/*! \fn int LenaDDI_CreateSwPostProc(int depthBits, void** handle)
	\brief create a software post process class
	\param depthBits	depth bit to set
	\param handle	pointer to the handle of this software post process class
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_CreateSwPostProc(int depthBits, void** handle);

/*! \fn int LenaDDI_ReleaseSwPostProc(void** handle)
	\brief release a software post process class
	\param handle	pointer to the handle of this software post process class
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_ReleaseSwPostProc(void** handle);

/*! \fn int LenaDDI_DoSwPostProc(void* handle, unsigned char* colorBuf, bool isColorRgb24, unsigned char* depthBuf, unsigned char* outputBuf, int width, int height)
	\brief do software post process on a depth buffer
	\param handle	handle of this software post process class
	\param colorBuf	input color buffer
	\param isColorRgb24	is this color buffer RGB888
	\param depthBuf	input depth buffer
	\param outputBuf	output buffer
	\param width	image width
	\param height	image height
	\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_DoSwPostProc(void* handle, unsigned char* colorBuf, bool isColorRgb24, 
    unsigned char* depthBuf, unsigned char* outputBuf, int width, int height);

/*! \fn bool LenaDDI_HasMultiDepth0ColorPlusDepth(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo,
		int colorStreamIndex)
	\brief check module support multi-base line w/ color+depth on depth 0(S0) pin
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\param colorStreamIndex	color stream index to check
	\return true: support S0 w/ color combine with depth, false: not support
*/
bool LenaDDI_API LenaDDI_HasMultiDepth0ColorPlusDepth(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, int colorStreamIndex);

/*! \fn bool LenaDDI_HasMultiDepth2(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief check module support multi-base line w/ depth 2(S2) pin
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\return true: support S2, false: not support
*/
bool LenaDDI_API LenaDDI_HasMultiDepth2(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn bool LenaDDI_IsDeocclusionDevice(
		void *pHandleLenaDDI,
		PDEVSELINFO pDevSelInfo)
	\brief check module support de-occlusion function or not
	\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
	\param pDevSelInfo	pointer of device select index
	\return true: support deocclusion, false: not support
*/
bool LenaDDI_API LenaDDI_IsDeocclusionDevice(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn bool LenaDDI_IsInterleaveDevice(
void *pHandleLenaDDI,
PDEVSELINFO pDevSelInfo)
\brief check module support interleave function or not
\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
\param pDevSelInfo	pointer of device select index
\return true: support interleave, false: not support
*/
bool LenaDDI_API LenaDDI_IsInterleaveDevice(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo);

/*! \fn int LenaDDI_IsInterleaveDevice(
void *pHandleLenaDDI,
PDEVSELINFO pDevSelInfo)
\brief enable or disable interleave function
\param pHandleLenaDDI	 the pointer to the initilized LenaDDI SDK instance
\param pDevSelInfo	pointer of device select index
\param enable	set true to enable interleave, or set false to disable interleave
\return success: LenaDDI_OK, others: see eSPDI_ErrCode.h
*/
int LenaDDI_API LenaDDI_EnableInterleave(void *pHandleLenaDDI, PDEVSELINFO pDevSelInfo, bool enable);

#ifdef __cplusplus
}
#endif