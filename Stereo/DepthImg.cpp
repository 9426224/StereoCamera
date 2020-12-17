#include "DepthImg.h"

using namespace cv;

/// <summary>
/// 析构函数
/// </summary>
DepthImg::~DepthImg()
{

}

/// <summary>
/// 深度图像暂存变量创建
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
	//GaussianBlur(depthBuf, depthBuf, Size(5, 5), 0, 0); //高斯滤波

	//imshow("Depth Image", depthBuf);
	DepthImageProcess(depthBuf);

	int key = waitKey(1);

	if (key == 's')
	{
		std::string Img_Name = "D:\\Picture\\Depth" + std::to_string(k++) + ".bmp";
		imwrite(Img_Name, depthBuf);
	}
}

void DepthImg::DepthImageProcess(Mat img)
{
	imshow("Depth",img);
}