#include <iostream>
#include "opencv2/opencv.hpp"
#include "eSPDI.h"
#include <unistd.h>

using namespace std;
using namespace cv;

int main(void)
{
	//Parameters
	void *hEtronDI = nullptr, *pHandleEtronDI = nullptr;
	void *pHwndNotice = nullptr;
	DEVSELINFO pDevSelInfo;
	vector<DEVINFORMATION> devInfo;
	ETRONDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];
	CONTROL_MODE cm = IMAGE_SN_NONSYNC;
	DEPTH_TRANSFER_CTRL dtc = DEPTH_IMG_NON_TRANSFER;
	int pFPS = 30;
	unsigned long int pDepthImageSize = sizeof(unsigned char) * 1280 * 720 *3;
	unsigned char *pDepthBuf = (unsigned char*)malloc(pDepthImageSize);

	Mat depthImg;

	//Function

	//Device PreInit
	if (EtronDI_Init(&hEtronDI, false) < 0)
	{
<<<<<<< HEAD
		EtronDI_Release(&hEtronDI);
		cout<<"Can't PreInit Device!"<<endl;
=======
		if (colorImg != nullptr)
		{
			colorImg->Play(imgBuf);
		}
>>>>>>> 490de92a9effeff34657ba282046517875ce2c34
	}

	if (hEtronDI != nullptr)
	{
<<<<<<< HEAD
		int deviceCount = EtronDI_GetDeviceNumber(hEtronDI);	
	    if(deviceCount > 0)
		{
			devInfo.clear();
			for(int i = 0;i < deviceCount; i++)
			{
				DEVSELINFO devSelInfo;
				devSelInfo.index = i;
				DEVINFORMATION devInformation;
				EtronDI_GetDeviceInfo(hEtronDI, &devSelInfo, &devInformation);
				devInfo.push_back(devInformation);
			}
			EtronDI_Release(&hEtronDI);
=======
		if (depthImg != nullptr)
		{
			depthImg->Play(imgBuf);
>>>>>>> 490de92a9effeff34657ba282046517875ce2c34
		}
	}
	
	//Device Init
	for(size_t i = 0; i < devInfo.size(); i++)
	{
		if(strcmp(devInfo[i].strDevName, "/dev/video0") == 0)
		{
			pDevSelInfo.index = i;
		}
	}
	
	EtronDI_Init(&pHandleEtronDI, false);

	EtronDI_GetDeviceResolutionList(pHandleEtronDI, &pDevSelInfo, 64, pStreamColorInfo, 64, pStreamDepthInfo);

	EtronDI_SetDepthDataType(pHandleEtronDI, &pDevSelInfo, ETronDI_DEPTH_DATA_11_BITS);
	
	if(EtronDI_OpenDevice(pHandleEtronDI, &pDevSelInfo, 1280, 720, true, 1280, 720, dtc, false, pHwndNotice, &pFPS, cm) == ETronDI_OK)
	{
		cout<<"Open Device Success!"<<endl;
	}

	while(true)
	{
		EtronDI_GetDepthImage(pHandleEtronDI, &pDevSelInfo, pDepthBuf, &pDepthImageSize, 0, 0);
		depthImg = Mat(Size(1280, 720), CV_8UC1, pDepthBuf);
		imshow("depth", depthImg);
		usleep(100000);
		
		int key = waitKey(1);

		if(key == 27)
		{
			break;
		}
	}
}
