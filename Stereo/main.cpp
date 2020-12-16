#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

DepthImg depthImg(0, 0, 0);
ColorImg colorImg(0, 0);

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