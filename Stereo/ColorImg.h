#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}

	void Play(unsigned char* buf);

	int width, height; //��ɫ��Ƶ�Ŀ����߶�
	cv::Mat colorBuf; //��ɫ��Ƶͼ��洢����
	mutable std::shared_mutex colorMutex; //���ڲ�ɫͼ�����ʱ����������ֹ���ʳ�ͻ

private:
	
};