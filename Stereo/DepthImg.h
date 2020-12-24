#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) 
	{
		depthSource = cv::Mat(width, height, CV_16UC1);
	}
	~DepthImg();

	void Init();
	void Play(unsigned char *buf);
	cv::Mat QuickDomainAnalysis(cv::Mat depth);
	cv::Mat SplitWater(cv::Mat depth);

	int maxDistance = 40000, minDistance = 5000;
	int fxAndBaseLine = 12821030;
	int width, height, type;
	cv::Mat depthBuf,depthSource; //Depth Buffer
	mutable std::shared_mutex depthMutex;

private:
	void BufferD11ConvertToGray(unsigned char* buf);
	void BufferZ14ConvertToGray(unsigned char* buf);
	
	unsigned char* pDepthBuf; //Depth Buffer Pointer
};