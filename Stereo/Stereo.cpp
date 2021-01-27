#include <iostream>
#include "opencv2/opencv.hpp"
#include "eSPDI.h"
#include <unistd.h>

using namespace std;
using namespace cv;

int main()
{
	//Parameters
	void *hEtronDI = nullptr; //初始化设备指针
	void *pHandleEtronDI = nullptr; //实际设备指针
	void *pHwndNotice = nullptr; //
	DEVSELINFO pDevSelInfo; //设备选择信息
	vector<DEVINFORMATION> devInfo; //设备信息存储
	ETRONDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];//深度流与彩色流
	int pFPS = 30;
	unsigned long int pDepthImageSize = sizeof(unsigned char) * 1280 * 720 *3;
	unsigned char *pDepthBuf = (unsigned char*)malloc(pDepthImageSize); //深度图像存储地址

	unsigned long int pColorImageSize = sizeof(unsigned char) * 1280 * 720 *3;
	
	unsigned char *pColorBuf = (unsigned char*)malloc(pColorImageSize); //深度图像存储地址

	if(pDepthBuf == NULL)
	{
		cout<<"Create Swap Failed!"<<endl;
		return -1;
	}

	Mat depthImg, colorImg;

	//Function

	//Device PreInit
	if (EtronDI_Init(&hEtronDI, false) < 0)
	{
		EtronDI_Release(&hEtronDI);
		cout<<"Can't PreInit Device!"<<endl;
		return -1;
	}

	if (hEtronDI != nullptr)
	{
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
		}
	}
	
	//Device Init
	for(size_t i = 0; i < devInfo.size(); i++)
	{
		if(strcmp(devInfo[i].strDevName, "/dev/video1") == 0)
		{
			pDevSelInfo.index = i;
		}
	}
	
	EtronDI_Init(&pHandleEtronDI, false);

	EtronDI_GetDeviceResolutionList(pHandleEtronDI, &pDevSelInfo, 64, pStreamColorInfo, 64, pStreamDepthInfo);

	EtronDI_SetDepthDataType(pHandleEtronDI, &pDevSelInfo, ETronDI_DEPTH_DATA_11_BITS);
	
	if(EtronDI_OpenDevice2(
			pHandleEtronDI, 
			&pDevSelInfo, 
			1280, 
			720, 
			true, 
			1280, 
			720, 
			DEPTH_IMG_NON_TRANSFER, 
			false, 
			NULL, 
			&pFPS ) == ETronDI_OK)
	{
		cout<<"Open Device Success!"<<endl;
	}
	else
	{
		cout<<"Open Device Failed!"<<endl;
		return -1;
	}

	while(true)
	{
		EtronDI_GetDepthImage(pHandleEtronDI, &pDevSelInfo, pDepthBuf, &pDepthImageSize, 0, 0);
		depthImg = Mat(Size(1280, 720), CV_8UC1, pDepthBuf);
		//imshow("depth", depthImg);

		EtronDI_GetColorImage(pHandleEtronDI, &pDevSelInfo, pColorBuf, &pColorImageSize, 0,0);

		colorImg = Mat(Size(1280,720), CV_8UC3, pColorBuf);

		//imshow("color", colorImg);

		usleep(100000);
		
		int key = waitKey(1);

		if(key == 27)
		{
			break;
		}
	}
}
