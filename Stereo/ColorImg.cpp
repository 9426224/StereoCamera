#include "ColorImg.h"

ColorImg::~ColorImg()
{

}

void ColorImg::Play(unsigned char* buf)
{
	cv::Mat img(cv::Size(width, height), CV_8UC3, buf);

	std::unique_lock<std::shared_mutex> lockColor(colorMutex);
	flip(img, colorBuf, 0);	
} 