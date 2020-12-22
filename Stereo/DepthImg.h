#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);

	int maxDistance = 120000, minDistance = 500;
	int fxAndBaseLine = 1282103;
	int width, height, type;
	cv::Mat depthBuf; //Depth Buffer
	mutable std::shared_mutex depthMutex;

private:
	void BufferD11ConvertToGray(unsigned char* buf);
	void BufferZ14ConvertToGray(unsigned char* buf);
	
	unsigned char* pDepthBuf; //Depth Buffer Pointer
};