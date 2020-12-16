#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

int colorResolution = 2; //YUV: 0��1280 * 720, 1:2560 * 720, 2 : 2560 * 960, 3 : 1280 * 480, 4 : 640 * 480, 5 : 1280 * 720MJPG, 6 : 2560 * 720MJPG, 7 : 2560 * 960MJPG
int depthResolution = 0; // ��ȷֱ��� 0-> 1280*720  1-> 640*480 -1-> No
int depthOption = 11; // 11-> 11bit�Ӳ�ͼ  14-> 14bit���ͼ 8-> 8bit�Ӳ�ͼ  USB2.0�£�8bit�Ӳ�ͼ

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

	if (depthResolution != -1)
	{
		depthImg.width = device.pStreamDepthInfo[depthResolution].nWidth;
		depthImg.height = device.pStreamDepthInfo[depthResolution].nHeight;
		depthImg.type = device.USBType ? depthOption : 8;
	}
	
	depthImg.Init();

	LenaDDI_OpenDeviceEx(
		device.pHandleLenaDDI, 
		&device.pDevSelInfo, 
		colorResolution, 
		colorImg.isImgRGB, 
		depthResolution, 
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