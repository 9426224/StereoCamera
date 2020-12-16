#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include "PDI_DM.H"
#include <mutex>

using namespace cv;
using namespace std;

#pragma region DefaultParameterDefine
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

#pragma region SGBMParameterDefine
Mat MatrixL = (Mat_<double>(3, 3) << 
	3252.272070364151, 0, 0, 
	0, 3236.910134939964, 0, 
	246.556379012332, 747.8802790048338, 1);//Left相机内参矩阵

Mat MatrixR = (Mat_<double>(3, 3) << 
	3264.14258131667, 0, 0,
	0, 3265.420090612704, 0,
	666.2227523335539, 559.3101448292923, 1);//Right相机内参矩阵

Mat distCoeffL = (Mat_<double>(5, 1) << -0.316763588, 6.33384747, 0.00279558433, 0.0000545577158, -61.5157155);//Left相机畸变系数
Mat distCoeffR = (Mat_<double>(5, 1) << -0.316763588, 6.33384747, 0.00279558433, 0.0000545577158, -61.5157155);//Right相机畸变系数

//Mat Rec = (Mat_<double>(3, 1) << -401.9838183195155, -6.260859479290675, 7.779318681506831); //rec旋转向量
Mat Transcation = (Mat_<double>(3, 1) << -401.9838183195155, -6.260859479290675, 7.779318681506831);
Mat Rotation = (Mat_<double>(3, 3) << 
	0.993019189673893, 0.012241022675804, 0.117316010430182, 
	-0.005392354562595, 0.998271761054028, -0.058518489337652,
	-0.117829586486907, 0.057477373338907, 0.991369023170678); //摄像头2相对于摄像头1的旋转矩阵

Size imageSize;// 图像尺寸

Mat Rl, Rr, Pl, Pr, Q; 
Rect validROIL, validROIR; //图像校正后会对图像进行裁剪，这里的validROI指的是裁剪之后的区域
Mat mapLx, mapLy, mapRx, mapRy; //映射表
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
	Mat left = img(Rect(0, 0, img.cols / 2, img.rows));
	Mat right = img(Rect(img.cols / 2, 0, img.cols / 2, img.rows));
	
	imageSize = Size(left.cols, left.rows);

	//Rodrigues(Rec, R); //Rodrigues变换，将Rec旋转向量变换为旋转矩阵
	
	stereoRectify(MatrixL, distCoeffL, MatrixR, distCoeffR, imageSize, Rotation, Transcation, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY,0, imageSize, &validROIL, &validROIR);
	
	initUndistortRectifyMap(MatrixL, distCoeffL, Rl, Pl, imageSize, CV_16SC2, mapLx, mapLy);
	initUndistortRectifyMap(MatrixR, distCoeffR, Rr, Pr, imageSize, CV_16SC2, mapRx, mapRy);

	remap(left, left, mapLx, mapLy, INTER_LINEAR);
	remap(right, right, mapRx, mapRy, INTER_LINEAR);

	imshow("Left", left);
	cvWaitKey(0);


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
			ImgCalc(img);
			//imshow("Color Image", img);
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