#include <mutex>
#include "opencv2/opencv.hpp"
#include "Utility.h"

class DepthImg {
public:
	DepthImg(int width,int height,int type): width(width), height(height), type(type) {}
	~DepthImg();

	int width, height, type;

	void Init();
	void Play(unsigned char *buf);

private:
	unsigned char* pDepthBuf; //深度流指针
	
	std::mutex depthMutex; //深度图像输出互斥锁
	float zoom; //图像缩放系数
	int key; //捕获键盘输入字符
	cv::Mat depthBuf; //深度流
	
	RGBQUAD ColorPaletteD11[2048];
	RGBQUAD ColorPaletteZ14[16384];

};