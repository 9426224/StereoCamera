#include <mutex>
#include "opencv2/opencv.hpp"

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}
	~ColorImg();

	bool isImgRGB = true;
	int width, height;

	void Play(unsigned char* buf);

private:
	std::mutex colorMutex; //��ɫͼ�����������
	cv::Mat colorBuf; //��ɫ��
};