#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);

	int width, height, type;
	cv::Mat depthBuf; //�����
	mutable std::shared_mutex depthMutex;

private:
	unsigned char* pDepthBuf; //�����ָ��
	
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];
};