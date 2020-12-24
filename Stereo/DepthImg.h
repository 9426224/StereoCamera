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

	int h = 930; //��ʾ����ͷ����ˮ��ĸ߶�,��λmm
	float nearestWater = /*6.3138*/ 10 * h; //�������������봬ֻ��ʵ�ʾ���
	int farthestWater = 50000; //��Զ����������봬ֻ��ʵ�ʾ���
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