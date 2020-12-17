#include "stdafx.h"
#include "PDI_DM.H"
#include "opencv2/opencv.hpp"

class Device {
public:
	Device() : hLenaDDI(nullptr) {}
	~Device();

	//���޸ĵĲ���
	int depthResolution = 0; // ��ȷֱ��� 0-> 1280*720  1-> 640*480 -1-> No
	int colorResolution = 0; //YUV: 0��1280 * 720, 1:2560 * 720, 2 : 2560 * 960, 3 : 1280 * 480, 4 : 640 * 480, 5 : 1280 * 720MJPG, 6 : 2560 * 720MJPG, 7 : 2560 * 960MJPG
	int depthOption = 11; // 11-> 11bit�Ӳ�ͼ  14-> 14bit���ͼ /*8-> 8bit�Ӳ�ͼ*/  USB2.0�£�8bit�Ӳ�ͼ
	
	
	int USBType; //0:USB2.0 1:USB3.0
	LenaDDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];
	LenaDDI_ImgCallbackFn callbackFn;

	bool Init();
	
private:
	std::vector<DEVINFORMATIONEX> devInfo; //�豸��Ϣ��
	int fps; //ͼ�������֡��
	BOOL isImgRGB = true;
	void* hLenaDDI = NULL; //�豸�豸��ʼ��ʹ�õ�ָ��
	DEVSELINFO pDevSelInfo; //�豸ѡ����Ϣ
	void* pHandleLenaDDI; //�豸ָ��

	void Release(void *dev);
	bool GetLenaDDIDevice();
};