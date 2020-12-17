#include "ColorImg.h"

using namespace cv;

ColorImg::~ColorImg()
{

}

void ColorImg::Play(unsigned char* buf)
{
	Mat img(Size(width, height), CV_8UC3, buf);
	flip(img, colorBuf, 0);
	
	//cvtColor(colorBuf, colorBuf, CV_BGR2GRAY); //�Ҷ�ͼ��ת��
	//Canny(img, img, 10, 100, 3); //Canny���ӱ�Ե���

	imshow("Color Img", colorBuf);

	waitKey(1);
} 