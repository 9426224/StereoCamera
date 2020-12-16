#include "PDI_DM.H"
#include "opencv2/opencv.hpp"

class Device {
public:
	Device() : hLenaDDI(nullptr) {}
	~Device();
	
	void* hLenaDDI; //设备设备初始化使用的指针
	DEVSELINFO pDevSelInfo; //设备选择信息
	void* pHandleLenaDDI; //设备指针
	std::vector<DEVINFORMATIONEX> devInfo; //设备信息链
	int USBType; //0:USB2.0 1:USB3.0
	int fps; //图像输出的帧率
	LenaDDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];

	bool Init();
	void Release();
private:
	bool GetLenaDDIDevice();
};