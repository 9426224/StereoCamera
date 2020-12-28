#include "opencv2/opencv.hpp"

#include "stdafx.h"

#include "PDI_DM.H"


class Device {
public:
	Device() : hLenaDDI(nullptr) {}
	~Device();

	// 下为可更改的参数
	int depthResolution = 0; // 深度分辨率 0-> 1280*720  1-> 640*480 -1-> 不输出
	int colorResolution = 0; //彩色分辨率 YUV: 0:1280 * 720, 1:2560 * 720, 2 : 2560 * 960, 3 : 1280 * 480, 4 : 640 * 480, 5 : 1280 * 720MJPG, 6 : 2560 * 720MJPG, 7 : 2560 * 960MJPG
	int depthOption = 11; // 11-> 11bit 视差图  14-> 14bit 深度图 /*8-> 8bit 视差图*/  USB2.0:8bit 视差图
	
	
	int USBType; //0:USB2.0 1:USB3.0
	LenaDDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];
	LenaDDI_ImgCallbackFn callbackFn;

	bool Init();
	
private:
	std::vector<DEVINFORMATIONEX> devInfo; //设备信息链
	int fps; //视频帧率
	BOOL isImgRGB = true;
	void* hLenaDDI; //设备初始化时使用的虚拟设备指针,用于获取设备信息
	DEVSELINFO pDevSelInfo; //设备选择指针
	void* pHandleLenaDDI; //设备指针

	void Release(void *dev);
	bool GetLenaDDIDevice();
};