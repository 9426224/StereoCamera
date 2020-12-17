#include "opencv2/opencv.hpp"

#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);

	int width, height, type;
	cv::Mat depthBuf; //深度流

private:
	int k = 1;
	unsigned char* pDepthBuf; //深度流指针
	
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];
};