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

	int h = 930; //��ʾ����ͷ����ˮ��ĸ߶�,��λmm
	float angle = 4.4316; // ����ͷ����ڵ������㵽����͵�ĽǶ���Ϣ
	int USVHeightChange = 0; //��ֻ�߶ȱ仯ʱ���е���

	int width, height, type; //�����Ƶ�Ŀ����߶ȡ�����
	int maxDistance = 16560, minDistance = 5000; //����������Զ�������������,�ɵ���
	cv::Mat depthBuf,depthBufBit16; //�����Ϣ�洢����,����Buf��8bit,Source��16bit
	mutable std::shared_mutex depthMutex; //�������ͼ�����ʱ����������ֹ���ʳ�ͻ

private:	
	int fxAndBaseLine = 12821030; //��������ߵĳ˻���fx = 3203.2 = 3.75mm * ��ͷʵ�ʽ��� ,BaseLine = 400.257mm
	unsigned char* pDepthBuf;
	unsigned short* pDepthBufBit16;

	void BufferReader(unsigned char* buf);
	float AngleConverter(float,int);
};