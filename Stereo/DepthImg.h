#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) 
	{
		pDepthBuf = (unsigned char*)realloc(pDepthBuf, sizeof(unsigned char) * width * height *3);
		pDepthSource = (unsigned short*)realloc(pDepthSource, sizeof(unsigned short) * width * height * 3);
	}

	void Play(unsigned char *);
	void QuickDomainAnalysis(cv::Mat);
	void SplitWater(cv::Mat);

	int h = 930; //表示摄像头距离水面的高度,单位mm
	float nearestWater = /*6.3138*/ 10 * h; //最低像素区域距离船只的实际距离
	int farthestWater = 50000; //最远像素区域距离船只的实际距离
	int pixel = 200;

	int width, height, type;
	int maxDistance = 50000, minDistance = 5000;
	cv::Mat depthBuf,depthSource; //Depth Buffer
	mutable std::shared_mutex depthMutex;

private:
	int fxAndBaseLine = 12821030;
	unsigned char* pDepthBuf;
	unsigned short* pDepthSource;

	void BufferReader(unsigned char* buf);
};