#include "opencv2/opencv.hpp"

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}
	~ColorImg();

	void Play(unsigned char* buf);

	int width, height;

private:
	int k = 1;
	cv::Mat colorBuf; //²ÊÉ«Á÷
};