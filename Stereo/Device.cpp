#include "Device.h"

Device::~Device()
{
	if (pHandleLenaDDI != nullptr)
	{
		pHandleLenaDDI = nullptr;
	}
}

bool Device::Init()
{
	if (!GetLenaDDIDevice())
	{
		printf_s("Get Device Failed!\n");
		return false;
	}

	for (size_t i = 0; i < devInfo.size(); i++) //LenaCV设备的对应编号查找
	{
		if (strcmp(devInfo[i].strDevName, "Lena3d:vid_0211 pid_5838") == 0)
		{
			pDevSelInfo.index = i;
		}
	}

	LenaDDI_Init2(&pHandleLenaDDI, false, true); //设备初始化
	
	LenaDDI_GetDeviceResolutionList(pHandleLenaDDI, &pDevSelInfo, LenaDDI_MAX_STREAM_COUNT, pStreamColorInfo, LenaDDI_MAX_STREAM_COUNT, pStreamDepthInfo);

	USBType = (pStreamDepthInfo[0].nWidth == 1280) ? 1 : 0;

	LenaDDI_SetDepthDataType(pHandleLenaDDI, &pDevSelInfo, USBType ? (depthOption == 14 ? LenaDDI_DEPTH_DATA_14_BITS : (depthOption == 11 ? LenaDDI_DEPTH_DATA_11_BITS : LenaDDI_DEPTH_DATA_8_BITS)) : LenaDDI_DEPTH_DATA_8_BITS);

	fps = LenaDDI_IsInterleaveDevice(pHandleLenaDDI, &pDevSelInfo) ? 60 : 30;

	LenaDDI_OpenDeviceEx(pHandleLenaDDI, &pDevSelInfo, colorResolution, isImgRGB, depthResolution, LenaDDIDepthSwitch::Depth1, callbackFn, NULL, &fps, 0);
	
	return true;
}

void Device::Release(void *dev)
{
	if (dev != nullptr)
	{
		LenaDDI_CloseDevice(dev, &pDevSelInfo);
		LenaDDI_Release(&dev);
		dev = nullptr;
	}
}

bool Device::GetLenaDDIDevice()
{
	if (LenaDDI_Init(&hLenaDDI, false) < 0)
	{
		Release(hLenaDDI);
	}
	if (hLenaDDI != NULL)
	{
		int deviceCount = LenaDDI_GetDeviceNumber(hLenaDDI);
		if (deviceCount > 0)
		{
			devInfo.clear();
			for (int i = 0; i < deviceCount; i++)
			{
				DEVSELINFO pdevSelInfo;
				pdevSelInfo.index = i;
				DEVINFORMATIONEX pdevinfo;
				LenaDDI_GetDeviceInfoEx(hLenaDDI, &pdevSelInfo, &pdevinfo);
				devInfo.push_back(pdevinfo);
			}
			Release(hLenaDDI);
			return true;
		}
	}
	return false;
}