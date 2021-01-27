#include "Device.h"

Device::~Device()
{
	if (pHandleEtronDI != nullptr)
	{
		pHandleEtronDI = nullptr;
	}
}

bool Device::Init()
{
	if (!GetEtronDIDevice())
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
  
	EtronDI_Init2(&pHandleEtronDI, false, true); //设备初始化
	
	EtronDI_GetDeviceResolutionList(pHandleEtronDI, &pDevSelInfo, ETronDI_MAX_STREAM_COUNT, pStreamColorInfo, ETronDI_MAX_STREAM_COUNT, pStreamDepthInfo);

	USBType = (pStreamDepthInfo[0].nWidth == 1280) ? 1 : 0;

	if (depthResolution != -1)
	{
		EtronDI_SetDepthDataType(pHandleEtronDI, &pDevSelInfo, USBType ? (depthOption == 14 ? ETronDI_DEPTH_DATA_14_BITS : (depthOption == 11 ? ETronDI_DEPTH_DATA_11_BITS : ETronDI_DEPTH_DATA_8_BITS)) : ETronDI_DEPTH_DATA_8_BITS);
	}

	fps = EtronDI_IsInterleaveDevice(pHandleEtronDI, &pDevSelInfo) ? 60 : 30;

	EtronDI_OpenDeviceEx(pHandleEtronDI, &pDevSelInfo, colorResolution, isImgRGB, depthResolution, depthResolution != -1 ? EtronDIDepthSwitch::Depth1 : 0, callbackFn, NULL, &fps, 0);
	
	return true;
}

void Device::Release(void *dev)
{
	if (dev != nullptr)
	{
		EtronDI_CloseDevice(dev, &pDevSelInfo);
		EtronDI_Release(&dev);
		dev = nullptr;
	}
}

bool Device::GetEtronDIDevice()
{
	if (EtronDI_Init(&hEtronDI, false) < 0)
	{
		Release(hEtronDI);
	}
	if (hEtronDI != NULL)
	{
		int deviceCount = EtronDI_GetDeviceNumber(hEtronDI);
		if (deviceCount > 0)
		{
			devInfo.clear();
			for (int i = 0; i < deviceCount; i++)
			{
				DEVSELINFO pdevSelInfo;
				pdevSelInfo.index = i;
				DEVINFORMATIONEX pdevinfo;
				EtronDI_GetDeviceInfoEx(hEtronDI, &pdevSelInfo, &pdevinfo);
				devInfo.push_back(pdevinfo);
			}
			Release(hEtronDI);
			return true;
		}
	}
	return false;
}