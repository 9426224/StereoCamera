#include "opencv2/opencv.hpp"

#include <mutex>

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}
	~ColorImg();

	void Play(unsigned char* buf);

	int width, height;

private:
	std::mutex colorMutex; //��ɫͼ�����������
	cv::Mat colorBuf; //��ɫ��
};