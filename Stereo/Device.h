#include "opencv2/opencv.hpp"

#include "stdafx.h"

#include "PDI_DM.H"


class Device {
public:
	Device() : hLenaDDI(nullptr) {}
	~Device();

	//Parameters which can be changed in there.
	int depthResolution = 0; // Depth Resolution 0-> 1280*720  1-> 640*480 -1-> No
	int colorResolution = 0; //Color Resolution YUV: 0:1280 * 720, 1:2560 * 720, 2 : 2560 * 960, 3 : 1280 * 480, 4 : 640 * 480, 5 : 1280 * 720MJPG, 6 : 2560 * 720MJPG, 7 : 2560 * 960MJPG
	int depthOption = 11; // 11-> 11bit parollex image  14-> 14bit depth image /*8-> 8bit parollex image*/  USB2.0:8bit parollex image
	
	
	int USBType; //0:USB2.0 1:USB3.0
	LenaDDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];
	LenaDDI_ImgCallbackFn callbackFn;

	bool Init();
	
private:
	std::vector<DEVINFORMATIONEX> devInfo; //Device information chain
	int fps; //Frames per second.
	BOOL isImgRGB = true;
	void* hLenaDDI = NULL; //An useless pointer for device intialization.
	DEVSELINFO pDevSelInfo; //Device choice pointer.
	void* pHandleLenaDDI; //Device pointer.

	void Release(void *dev);
	bool GetLenaDDIDevice();
};