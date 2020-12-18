#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);

	int width, height, type;
	cv::Mat depthBuf; //深度流
	mutable std::shared_mutex depthMutex;

private:
	void BufferD11ConvertToGray(unsigned char* buf);
	void BufferZ14ConvertToGray(unsigned char* buf);
	
	unsigned char* pDepthBuf; //深度流指针
};