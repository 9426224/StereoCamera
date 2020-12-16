#include "PDI_DM.H"
#include "opencv2/opencv.hpp"

class Device {
public:
	Device() : hLenaDDI(nullptr) {}
	~Device();
	
	void* hLenaDDI; //�豸�豸��ʼ��ʹ�õ�ָ��
	DEVSELINFO pDevSelInfo; //�豸ѡ����Ϣ
	void* pHandleLenaDDI; //�豸ָ��
	std::vector<DEVINFORMATIONEX> devInfo; //�豸��Ϣ��
	int USBType; //0:USB2.0 1:USB3.0
	int fps; //ͼ�������֡��
	LenaDDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];

	bool Init();
	void Release();
private:
	bool GetLenaDDIDevice();
};