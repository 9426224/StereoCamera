#include "opencv2/opencv.hpp"

#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);

	int width, height, type;
	cv::Mat depthBuf; //�����

private:
	int k = 1;
	unsigned char* pDepthBuf; //�����ָ��
	
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];
};