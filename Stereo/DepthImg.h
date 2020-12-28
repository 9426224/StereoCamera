#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>
#include "math.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) 
	{
		auto tempDepthBuf = (unsigned char*)realloc(pDepthBuf, sizeof(unsigned char) * width * height *3);
		auto tempDepthSource = (unsigned short*)realloc(pDepthSource, sizeof(unsigned short) * width * height * 3);

		if (tempDepthBuf != NULL)
			pDepthBuf = tempDepthBuf;
		if (tempDepthSource != NULL)
			pDepthSource = tempDepthSource;
	}

	void Play(unsigned char *);
	cv::Mat QuickDomainAnalysis(cv::Mat);
	void SplitWater(cv::Mat);

	int h = 930; //��ʾ����ͷ����ˮ��ĸ߶�,��λmm
	int angle = 9; // ����ͷ����ڵ������㵽����͵�ĽǶ���Ϣ
	

	int width, height, type; //�����Ƶ�Ŀ����߶ȡ�����
	int maxDistance = 50000, minDistance = 5000; //����������Զ�������������,�ɵ���
	cv::Mat depthBuf,depthSource; //�����Ϣ�洢����,����Buf��8bit,Source��16bit
	mutable std::shared_mutex depthMutex; //�������ͼ�����ʱ����������ֹ���ʳ�ͻ

private:	
	int fxAndBaseLine = 12821030; //��������ߵĳ˻���fx = 3203.2 = 3.75mm * ��ͷʵ�ʽ��� ,BaseLine = 400.257mm
	unsigned char* pDepthBuf;
	unsigned short* pDepthSource;

	void BufferReader(unsigned char* buf);
	float AngleConverter(int,int);
};