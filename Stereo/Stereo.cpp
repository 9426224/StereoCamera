#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include "PDI_DM.H"
#include <mutex>

using namespace cv;

#pragma region ParameterDefine
void* hLenaDDI; //设备初始化使用的指针
void* pHandleLenaDDI; //设备指针
DEVSELINFO pDevSelInfo; //设备选择信息
std::vector<DEVINFORMATIONEX> devInfo; //设备信息链
LenaDDI_STREAM_INFO pStreamInfoColor[LenaDDI_MAX_STREAM_COUNT]; //彩色流信息
LenaDDI_STREAM_INFO pStreamInfoDepth[LenaDDI_MAX_STREAM_COUNT]; //深度流信息
int colorOption = 7;  //彩色图像分辨率选项： 0：1280 * 720, 1:2560 * 720, 2 : 2560 * 960, 3 : 1280 * 480, 4 : 640 * 480, 5 : 1280 * 720MJPG, 6 : 2560 * 720MJPG, 7 : 2560 * 960MJPG
Mat imgBuffer; //图像缓存流
Mat img; //输出图像
std::mutex imgMutex; //互斥锁，确保同一时间只有一个线程访问对象
int flag = 0; //辅助互斥锁
#pragma endregion

/// <summary>
/// 获取设备信息
/// </summary>
/// <param name="device">设备信息链</param>
/// <returns></returns>
bool GetLenaDDIDevice(std::vector<DEVINFORMATIONEX>& device)
{
	if (LenaDDI_Init(&hLenaDDI, false) < 0)
	{
		if (hLenaDDI != NULL)
		{
			LenaDDI_Release(&hLenaDDI);
			hLenaDDI = NULL;
		}
		return false;
	}
	if (hLenaDDI != NULL)
	{
		int deviceCount = LenaDDI_GetDeviceNumber(hLenaDDI);
		if (deviceCount > 0)
		{
			device.clear();
			for (int i = 0; i < deviceCount; i++)
			{
				DEVSELINFO pdevSelInfo;
				pdevSelInfo.index = i;
				DEVINFORMATIONEX pdevinfo;
				LenaDDI_GetDeviceInfoEx(hLenaDDI, &pdevSelInfo, &pdevinfo);
				device.push_back(pdevinfo);
			}
			return true;
		}
	}
	return false;
}

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
		imgMutex.lock();
		if (flag == 0)
		{
			imgBuffer = Mat(Size(width, height), CV_8UC3, imgBuf);
			flag = 1;
		}
		imgMutex.unlock();
	}
	else
	{
		printf_s("Unknown Image Type\n");
	}
}

void ImgCalc(Mat img)
{
	Mat left = img(Rect(0, 0, img.cols / 2, img.rows / 2));

}

int main()
{
	pHandleLenaDDI = nullptr;

	if (!GetLenaDDIDevice(devInfo))
	{
		printf_s("Get Device Failed!\n");
		return -1;
	}

	for (size_t i = 0; i < devInfo.size(); i++) //LenaCV设备的对应编号查找
	{
		if (strcmp(devInfo[i].strDevName, "Lena3d:vid_0211 pid_5838") == 0)
		{
			pDevSelInfo.index = i;
		}
	}

	LenaDDI_Init2(&pHandleLenaDDI, false, true); //设备初始化

	int ret = LenaDDI_GetDeviceResolutionList(pHandleLenaDDI, &pDevSelInfo, LenaDDI_MAX_STREAM_COUNT, pStreamInfoColor, LenaDDI_MAX_STREAM_COUNT, pStreamInfoDepth);

	int colorStreamOptionCount = (int)((BYTE*)(&ret))[1];
	int depthStreamOptionCount = (int)((BYTE*)(&ret))[0];

	int fps = (LenaDDI_IsInterleaveDevice(pHandleLenaDDI, &pDevSelInfo) ? 60 : 30);

	int iRet = LenaDDI_OpenDeviceEx(pHandleLenaDDI, &pDevSelInfo, colorOption, true, -1, 0, ImgCallback, NULL, &fps, 0); //打开设备

	while (true)
	{
		imgMutex.lock();
		if (flag)
		{
			flip(imgBuffer, img, 0);
			flag = 0;
			imgMutex.unlock();
			//ImgCalc(img);
			imshow("Color Image", img);
		}
		else
		{
			imgMutex.unlock();
		}
		int key = waitKey(1);
	}

	if (pHandleLenaDDI != nullptr)
	{
		LenaDDI_CloseDevice(pHandleLenaDDI, &pDevSelInfo);
		LenaDDI_Release(&pHandleLenaDDI);
		pHandleLenaDDI = nullptr;
	}
	return 0;
}