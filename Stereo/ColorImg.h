#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}

	void Play(unsigned char* buf);

	int width, height; //彩色视频的宽度与高度
	cv::Mat colorBuf; //彩色视频图像存储数组
	mutable std::shared_mutex colorMutex; //用于彩色图像输出时进行锁定防止访问冲突

private:
	
};