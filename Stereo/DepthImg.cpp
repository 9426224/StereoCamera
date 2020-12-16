#include "DepthImg.h"

using namespace cv;

DepthImg::~DepthImg()
{

}

/// <summary>
/// 深度图创建
/// </summary>
void DepthImg::Init()
{
	if (type == 8)
	{
		pDepthBuf = (unsigned char*)realloc(pDepthBuf, width * height * 2 * sizeof(unsigned char));
	}
	else
	{
		pDepthBuf = (unsigned char*)realloc(pDepthBuf, width * height * 3 * sizeof(unsigned char));
		switch (type)
		{
		case 11: 
			DmColorMode11(ColorPaletteD11, 0);
			break;
		case 14:
			DmColorMode14(ColorPaletteZ14, 0);
			break;
		default:
			printf("Wrong Depth Type");
			break;
		}
	}
}

/// <summary>
/// 读取视差图或深度距离图
/// </summary>
/// <param name="buf">深度图像流</param>
void DepthImg::Play(unsigned char* buf)
{
	if (type == 8)
	{
		Mat img(Size(width * 2, height), CV_8UC1, buf);
		depthBuf = img;
	}
	else
	{
		switch (type) 
		{
		case 11:
			UpdateD11DisplayImage_DIB24(ColorPaletteD11, buf, pDepthBuf, width, height);
			break;
		case 14:
			UpdateZ14DisplayImage_DIB24(ColorPaletteZ14, buf, pDepthBuf, width, height);
			break;
		default:
			break;
		}
		Mat img(Size(width, height), CV_8UC3, pDepthBuf);
		depthBuf = img;
	}
	if (zoom != 1.0)
	{
		resize(depthBuf, depthBuf, Size(depthBuf.cols * zoom, depthBuf.rows * zoom), 0, 0, INTER_LINEAR);
	}
	//GaussianBlur(depthBuf, depthBuf, Size(5, 5), 0, 0); //高斯滤波

	imshow("Depth Image", depthBuf);

	key = waitKey(1);

	depthMutex.lock();
	if (key == 'S')
	{
		zoom = (zoom == 1.0) ? 0.5 : 1.0;
	}
	depthMutex.unlock();

}

