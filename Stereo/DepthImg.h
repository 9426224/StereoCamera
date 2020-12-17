#include "opencv2/opencv.hpp"

#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);
	void DepthImageProcess(cv::Mat img); //���ͼ������

	int width, height, type;

private:
	int k = 1;
	unsigned char* pDepthBuf; //�����ָ��
	cv::Mat depthBuf; //�����
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];
};