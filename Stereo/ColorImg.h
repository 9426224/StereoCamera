#include "opencv2/opencv.hpp"

#include <mutex>

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}
	~ColorImg();

	void Play(unsigned char* buf);

	int width, height;

private:
	std::mutex colorMutex; //彩色图像输出互斥锁
	cv::Mat colorBuf; //彩色流
};