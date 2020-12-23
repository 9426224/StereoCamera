#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);
	cv::Mat QuickDomainAnalysis(cv::Mat depth);

	int maxDistance = 40000, minDistance = 5000;
	int fxAndBaseLine = 12821030;
	int width, height, type;
	cv::Mat depthBuf; //Depth Buffer
	mutable std::shared_mutex depthMutex;

private:
	void BufferD11ConvertToGray(unsigned char* buf);
	void BufferZ14ConvertToGray(unsigned char* buf);
	
	unsigned char* pDepthBuf; //Depth Buffer Pointer
};