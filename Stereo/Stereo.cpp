#define CVUI_IMPLEMENTATION
#include "cvui.h"
#include "pcl/io/io.h"
#include "pcl/io/pcd_io.h"

#include <thread>
#include "pcl/visualization/cloud_viewer.h"

#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

#define WINDOW_NAME "Depth"

using namespace cv;
using namespace pcl;

DepthImg* depthImg = nullptr;
ColorImg* colorImg = nullptr;

//图像回调函数
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

void viewerOneOff(visualization::PCLVisualizer& viewer)
{
	viewer.setBackgroundColor(0.0, 0.0, 0.0);
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
	visualization::CloudViewer viewer("Cloud Viewer");
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
		    cvui::window(depth, 900, 60, 300, 350, "Settings");
			cvui::checkbox(depth, 915, 80, "Use Split Water", &UseSplitWater);
			cvui::trackbar(depth, 915, 120, 270, &depthImg->minDistance, 600, 10000);
			cvui::trackbar(depth, 915, 180, 270, &depthImg->maxDistance, 11000, 50000);
			cvui::trackbar(depth, 915, 240, 270, &depthImg->h, 300, 3000);
			cvui::trackbar(depth, 915, 300, 270, &depthImg->angle , (float)1.50, (float)10.0);
			cvui::trackbar(depth, 915, 360, 270, &depthImg->USVHeightChange, -10000, 10000);
			cvui::update();

			imshow(WINDOW_NAME, depth);
		}

		//if(!depthBit16.empty() && !color.empty())
		if(false)
		{
			PointCloud<PointXYZRGB>::Ptr cloud(new PointCloud<PointXYZRGB>);
			for (int i = 0; i < depthBit16.rows; i++)
			{
				ushort* d = depthBit16.ptr<ushort>(i);
				uchar* c = color.ptr<uchar>(i);
				for (int j = 0; j < depthBit16.cols; j++)
				{
					if (d[j] == 0)
						continue;
					PointXYZRGB s;
					s.z = d[j];
					s.x = i;
					s.y = j;

					s.b = c[j * 3];
					s.g = c[j * 3 + 1];
					s.r = c[j * 3 + 2];

					cloud->points.push_back(s);

				}
			}

			cloud->height = depthBit16.rows;
			cloud->width = depthBit16.cols;
			cloud->points.resize(cloud->height * cloud->width);

			viewer.showCloud(cloud);
			viewer.runOnVisualizationThreadOnce(viewerOneOff);
		}
	}

	return 0;
}