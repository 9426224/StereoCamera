#include "DepthImg.h"

DepthImg::~DepthImg()
{

}

/// <summary>
/// ���ͼ����
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
/// ��ȡ�Ӳ�ͼ����Ⱦ���ͼ
/// </summary>
/// <param name="buf">���ͼ����</param>
void DepthImg::Play(unsigned char* buf)
{
	if (type == 8)
	{
		cv::Mat img(cv::Size(width * 2, height), CV_8UC1, buf);
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
		cv::Mat img(cv::Size(width, height), CV_8UC3, pDepthBuf);
		depthBuf = img;
	}
	if (zoom != 1.0)
	{
		cv::resize(depthBuf, depthBuf, cv::Size(depthBuf.cols * zoom, depthBuf.rows * zoom), 0, 0, CV_INTER_LINEAR);
	}
	//cv::GaussianBlur(depthBuf, depthBuf, cv::Size(5, 5), 0, 0); //��˹�˲�

	imshow("Depth Image", depthBuf);

	key = cv::waitKey(1);

	depthMutex.lock();
	if (key == 'S')
	{
		zoom = (zoom == 1.0) ? 0.5 : 1.0;
	}
	depthMutex.unlock();

}

