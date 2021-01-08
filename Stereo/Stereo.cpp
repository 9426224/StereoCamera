#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

#define WINDOW_NAME "Depth"

using namespace cv;

DepthImg* depthImg = nullptr;
ColorImg* colorImg = nullptr;

//图像回调函数
void ImgCallback(EtronDIImageType::Value imgType, int imgId, unsigned char* imgBuf, int imgSize, int width, int height, int serialNumber, void* pParam)
{
	if (EtronDIImageType::IsImageColor(imgType))
	{
		if (colorImg != nullptr)
		{
			colorImg->Play(imgBuf);
		}
	}
	else if (EtronDIImageType::IsImageDepth(imgType))
	{
		if (depthImg != nullptr)
		{
			depthImg->Play(imgBuf);
		}
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

	colorImg = new ColorImg(device.pStreamColorInfo[device.colorResolution].nWidth,
		device.pStreamColorInfo[device.colorResolution].nHeight);

	if (device.depthResolution != -1)
	{
		depthImg = new DepthImg(device.pStreamDepthInfo[device.depthResolution].nWidth,
			device.pStreamDepthInfo[device.depthResolution].nHeight,
			device.USBType ? device.depthOption : 8);
	}


	namedWindow(WINDOW_NAME);
	cvui::init(WINDOW_NAME);
	bool UseSplitWater = false;
	Mat color, depth, depthBit16;
	
	while (waitKey(1) != 27)
	{
		
		if (!colorImg->colorBuf.empty())
		{
			{
				std::shared_lock<std::shared_mutex> lockDepth(colorImg->colorMutex);
				color = colorImg->colorBuf;
			}

			//colorImg->ImageProcess(color);

			//imshow("color", color);
		}
		if (!depthImg->depthBuf.empty())
		{
			{
				std::shared_lock<std::shared_mutex> lockColor(depthImg->depthMutex);
				depth = depthImg->depthBuf;
				depthBit16 = depthImg->depthBufBit16;
			}
			/* 深度图像处理 */
			cvtColor(depth, depth, COLOR_BGR2GRAY); //三通道灰度图转单通道灰度图,为后续计算做准备
			cvtColor(depthBit16, depthBit16, COLOR_BGR2GRAY);

			//GaussianBlur(depth, depth, Size(5, 5), 0, 0); //高斯滤波

			//medianBlur(depth, depth, 5); //中值滤波

			//blur(depth, depth, Size(3, 3), Point(-1, -1), 4);

			//快速连通域分析
			//depthImg->QuickDomainAnalysis(depthSource);

			if(UseSplitWater)
				depthImg->SplitWater(depthBit16);

			for (int i = 0; i < depthBit16.rows; i++)
			{
				ushort* p = depthBit16.ptr<ushort>(i);
				uchar* q = depth.ptr<uchar>(i);
				for (int j = 0; j < depthBit16.cols; j++)
				{
					q[j] = p[j] / 256;
					
				}
			}

			//可调节远近距离，单位mm
		    cvui::window(depth, 900, 60, 300, 400, "Settings");
			cvui::checkbox(depth, 915, 80, "Use Split Water", &UseSplitWater);
			cvui::trackbar(depth, 915, 120, 270, &depthImg->minDistance, 600, 10000);
			cvui::trackbar(depth, 915, 180, 270, &depthImg->maxDistance, 11000, 50000);
			cvui::trackbar(depth, 915, 240, 270, &depthImg->h, 300, 3000);
			cvui::trackbar(depth, 915, 300, 270, &depthImg->angle , (float)1.50, (float)10.0);
			cvui::trackbar(depth, 915, 360, 270, &depthImg->USVHeightChange, -10000, 10000);
			cvui::trackbar(depth, 915, 420, 270, &depthImg->USVLeftRightTiltAngle, (float)-360, (float)360);
			cvui::update();

			imshow(WINDOW_NAME, depth);
		}
	}

	return 0;
}