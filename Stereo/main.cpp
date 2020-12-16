#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

DepthImg depthImg(0, 0, 0);
ColorImg colorImg(0, 0);

/// <summary>
/// 图像回调函数
/// </summary>
/// <param name="imgType">图像类型</param>
/// <param name="imgId"></param>
/// <param name="imgBuf">图像缓存流</param>
/// <param name="imgSize"></param>
/// <param name="width">图像宽度</param>
/// <param name="height">图像高度</param>
/// <param name="serialNumber"></param>
/// <param name="pParam"></param>
void ImgCallback(LenaDDIImageType::Value imgType, int imgId, unsigned char* imgBuf, int imgSize, int width, int height, int serialNumber, void* pParam)
{
	if (LenaDDIImageType::IsImageColor(imgType))
	{
		colorImg.Play(imgBuf);
	}
	else if (LenaDDIImageType::IsImageDepth(imgType))
	{
		depthImg.Play(imgBuf);
	}
	else
	{
		printf_s("Unknown Image Type\n");
	}
}

int main()
{
	Device device;
	device.Init();

	if (device.depthResolution != -1)
	{
		depthImg.width = device.pStreamDepthInfo[device.depthResolution].nWidth;
		depthImg.height = device.pStreamDepthInfo[device.depthResolution].nHeight;
		depthImg.type = device.USBType ? device.depthOption : 8;
	}
	
	depthImg.Init();

	LenaDDI_OpenDeviceEx(
		device.pHandleLenaDDI, 
		&device.pDevSelInfo, 
		device.colorResolution,
		colorImg.isImgRGB, 
		device.depthResolution,
		LenaDDIDepthSwitch::Depth1, 
		ImgCallback, 
		NULL, 
		&device.fps, 
		0
	);

	while (1)
	{

	}
}