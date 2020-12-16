#include <thread>
#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

DepthImg *depthImg = nullptr;
ColorImg *colorImg = nullptr;

/// <summary>
/// ͼ��ص�����
/// </summary>
/// <param name="imgType">ͼ������</param>
/// <param name="imgId"></param>
/// <param name="imgBuf">ͼ�񻺴���</param>
/// <param name="imgSize"></param>
/// <param name="width">ͼ����</param>
/// <param name="height">ͼ��߶�</param>
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