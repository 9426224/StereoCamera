#include <thread>

#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

using namespace cv;

DepthImg* depthImg = nullptr;
ColorImg* colorImg = nullptr;

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

	colorImg = new ColorImg(device.pStreamColorInfo[device.colorResolution].nWidth,
		device.pStreamColorInfo[device.colorResolution].nHeight);

	if (device.depthResolution != -1)
	{
		depthImg = new DepthImg(device.pStreamDepthInfo[device.depthResolution].nWidth,
			device.pStreamDepthInfo[device.depthResolution].nHeight,
			device.USBType ? device.depthOption : 8);
	}

	depthImg->Init();

	while (true)
	{
		if (depthImg->depthBuf.rows != 0 && colorImg->colorBuf.rows != 0)
		{
			Mat color,depth;
			{
				std::shared_lock<std::shared_mutex> lockColor(depthImg->depthMutex);
				depth = depthImg->depthBuf;
				std::shared_lock<std::shared_mutex> lockDepth(colorImg->colorMutex);
				color = colorImg->colorBuf;
			}
			

			/* 深度图处理 */
			//cvtColor(depth, depth, COLOR_BGR2GRAY); //三通道灰度图转单通道灰度图,为后续计算做准备

			//GaussianBlur(depth, depth, Size(5, 5), 0, 0); //高斯滤波

			//medianBlur(depth, depth, 5); //中值滤波

			//threshold(depth, depth, 0, 255, THRESH_OTSU); //大津法 二值化阈值处理

			imshow("depth", depth);
			imshow("color", color);

			waitKey(1);




			/* 彩色图处理 */
			////threshold(color, colorCanny, 0, 255, THRESH_OTSU);
			//GaussianBlur(color, color, Size(5, 5), 0, 0); //高斯滤波
			//Canny(color, colorCanny, 200, 400, 3); //Canny算子边缘检测
			//
			//std::vector<std::vector<Point>> contours;
			//std::vector<Vec4i> hierarchy;
			//findContours(colorCanny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			//drawContours(depthFilter, contours, -1, (0, 0, 255), 3);

		}
	}

	return 0;
}