#include <mutex>
#include "opencv2/opencv.hpp"
#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	int width, height, type;

	void Init();
	void Play(unsigned char *buf);

private:
	unsigned char* pDepthBuf; //�����ָ��
	
	std::mutex depthMutex; //���ͼ�����������
	float zoom; //ͼ������ϵ��
	int key; //������������ַ�
	cv::Mat depthBuf; //�����
	
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];

};