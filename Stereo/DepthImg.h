#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>
#include "math.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) 
	{
		auto tempDepthBuf = (unsigned char*)realloc(pDepthBuf, sizeof(unsigned char) * width * height *3);
		auto tempDepthSource = (unsigned short*)realloc(pDepthBufBit16, sizeof(unsigned short) * width * height * 3);

		if (tempDepthBuf != NULL)
			pDepthBuf = tempDepthBuf;
		if (tempDepthSource != NULL)
			pDepthBufBit16 = tempDepthSource;
	}

	void Play(unsigned char *);
	cv::Mat QuickDomainAnalysis(cv::Mat);
	void SplitWater(cv::Mat);

	int h = 930; //表示摄像头距离水面的高度,单位mm
	float angle = 4.4316; // 摄像头相对于地面拍摄到的最低点的角度信息
	int USVHeightChange = 0; //船只高度变化时进行调整

	int width, height, type; //深度视频的宽度与高度、类型
	int maxDistance = 16560, minDistance = 5000; //相机拍摄的最远距离与最近距离,可调节
	cv::Mat depthBuf,depthBufBit16; //深度信息存储数组,其中Buf是8bit,Source是16bit
	mutable std::shared_mutex depthMutex; //用于深度图像输出时进行锁定防止访问冲突

private:	
	int fxAndBaseLine = 12821030; //焦距与基线的乘积，fx = 3203.2 = 3.75mm * 镜头实际焦距 ,BaseLine = 400.257mm
	unsigned char* pDepthBuf;
	unsigned short* pDepthBufBit16;

	void BufferReader(unsigned char* buf);
	float AngleConverter(float,int);
};