#include "opencv2/opencv.hpp"

#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);
	void DepthImageProcess(cv::Mat img); //深度图像处理函数

	int width, height, type;

private:
	int k = 1;
	unsigned char* pDepthBuf; //深度流指针
	cv::Mat depthBuf; //深度流
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];
};