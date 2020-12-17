#include "ColorImg.h"

ColorImg::~ColorImg()
{

}

void ColorImg::Play(unsigned char* buf)
{
	cv::Mat img(cv::Size(width, height), CV_8UC3, buf);
	flip(img, colorBuf, 0);
} 