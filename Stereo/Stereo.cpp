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
		depthImg->Init();
	}

	int i = 9;

	while (true)
	{
		Mat color, depth;
		if (colorImg->colorBuf.rows != 0)
		{
			{
				std::shared_lock<std::shared_mutex> lockDepth(colorImg->colorMutex);
				color = colorImg->colorBuf;
			}

			/* 彩色图处理 */
			//cvtColor(color, color, COLOR_BGR2GRAY); //彩色图像转为灰度图像

			//GaussianBlur(color, color, Size(5, 5), 0, 0); //高斯滤波

			//threshold(color, color, 0, 255, THRESH_OTSU);

			//Canny(color, color, 50, 500, 3); //Canny算子边缘检测

			//std::vector<std::vector<Point>> contours;
			//std::vector<Vec4i> hierarchy;
			//findContours(color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			//drawContours(depth, contours, -1, (0, 0, 255), 3);
			
			Mat left = color(Rect(0, 0, color.cols / 2, color.rows));

			Mat right = color(Rect(color.cols/2, 0, color.cols / 2, color.rows));

			namedWindow("left", 0);
			namedWindow("right", 0);

			//cvResizeWindow("left", 960, 540);
			//cvResizeWindow("right", 960, 540);

			imshow("left", left);
			imshow("right", right);

			int key = waitKey(1);
			if (key == 's')
			{
				std::string strleft = "left" + std::to_string(i) + ".jpg";
				std::string strright = "right" + std::to_string(i) + ".jpg";
				imwrite(strleft, left);
				imwrite(strright, right);
				i++;
			}
			
		}
		if (device.depthResolution != -1 && depthImg->depthBuf.rows) {
			{
				std::shared_lock<std::shared_mutex> lockColor(depthImg->depthMutex);
				depth = depthImg->depthBuf;
			}
			/* 深度图处理 */
			cvtColor(depth, depth, COLOR_BGR2GRAY); //三通道灰度图转单通道灰度图,为后续计算做准备

			//GaussianBlur(depth, depth, Size(5, 5), 0, 0); //高斯滤波

			medianBlur(depth, depth, 5); //中值滤波

			//threshold(depth, depth, 0, 255, THRESH_OTSU); //大津法 二值化阈值处理

			imshow("depth", depth);
		}

		
	}

	return 0;
}