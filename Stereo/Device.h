#include "stdafx.h"
#include "PDI_DM.H"
#include "opencv2/opencv.hpp"

class Device {
public:
	Device() : hLenaDDI(nullptr) {}
	~Device();
	
	int colorResolution = 2; //YUV: 0：1280 * 720, 1:2560 * 720, 2 : 2560 * 960, 3 : 1280 * 480, 4 : 640 * 480, 5 : 1280 * 720MJPG, 6 : 2560 * 720MJPG, 7 : 2560 * 960MJPG
	int depthResolution = 0; // 深度分辨率 0-> 1280*720  1-> 640*480 -1-> No
	int depthOption = 11; // 11-> 11bit视差图  14-> 14bit深度图 /*8-> 8bit视差图*/  USB2.0下：8bit视差图
	void* hLenaDDI = NULL; //设备设备初始化使用的指针
	DEVSELINFO pDevSelInfo; //设备选择信息
	void* pHandleLenaDDI; //设备指针
	std::vector<DEVINFORMATIONEX> devInfo; //设备信息链
	int USBType; //0:USB2.0 1:USB3.0
	int fps; //图像输出的帧率
	LenaDDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];

	bool Init();
	void Release(void *dev);
private:
	bool GetLenaDDIDevice();
};