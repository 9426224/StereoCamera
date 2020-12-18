#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}
	~ColorImg();

	void Play(unsigned char* buf);

	int width, height;
	cv::Mat colorBuf; //彩色流
	mutable std::shared_mutex colorMutex;

private:
	
};