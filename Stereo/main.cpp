#include <thread>
#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

DepthImg *depthImg = nullptr;
ColorImg *colorImg = nullptr;

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
		colorImg->Play(imgBuf);
	}
	else if (LenaDDIImageType::IsImageDepth(imgType))
	{
		depthImg->Play(imgBuf);
	}
	else
	{
		printf_s("Unknown Image Type\n");
	}
}

int main()
{
	Device device;
	device.callbackFn = ImgCallback;
	device.Init();

	colorImg = new ColorImg(0, 0);

	if (device.depthResolution != -1)
	{
		depthImg = new DepthImg(device.pStreamDepthInfo[device.depthResolution].nWidth,
			device.pStreamDepthInfo[device.depthResolution].nHeight,
			device.USBType ? device.depthOption : 8);
	}

	depthImg->Init();

	Sleep(-1);

	return 0;
}