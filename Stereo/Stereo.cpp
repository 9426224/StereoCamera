#define CVUI_IMPLEMENTATION
#include <iostream>
#include "opencv2/opencv.hpp"
#include "eSPDI.h"
#include <unistd.h>
#include <sys/time.h>
#include <shared_mutex>
#include <mutex>
#include <cmath>
#include <thread>
#include "cvui.h"

using namespace std;
using namespace cv;

//Parameters
void *hEtronDI = nullptr;       //初始化设备指针
void *pHandleEtronDI = nullptr; //实际设备指针
DEVSELINFO pDevSelInfo;         //设备选择信息
vector<DEVINFORMATION> devInfo; //设备信息存储

ETRONDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64]; //深度流与彩色流
int pFPS = 30;

Mat depthImg, colorImg;

int pSerialNum = 0;
unsigned long depthImageSize = 0;
unsigned short *pDepthBuf = (unsigned short *)realloc(pDepthBuf, sizeof(unsigned short) * 1280 * 720 * 3); //深度图像存储地址

unsigned char *buf = (unsigned char *)realloc(buf, sizeof(unsigned char) * 1280 * 720 * 3); //返回数据图像存储地址

int width = 1280;
int height = 720;
int maxDistance = 16560, minDistance = 5000; //相机拍摄的最远距离与最近距离,可调节
int fxAndBaseLine = 12821030;                //焦距与基线的乘积，fx = 3203.2 = 3.75mm * 镜头实际焦距 ,BaseLine = 400.257mm
int h = 930;                                 //表示摄像头距离水面的高度,单位mm
float angle = 4.4316;                        //摄像头相对于地面拍摄到的最低点的角度信息
int USVHeightChange = 0;                     //船只高度变化时进行调整
float USVLeftRightTiltAngle = 0;             //船只左右翻转时的视场角变化
bool UseSplitWater = false;

shared_mutex depthMutex;

float AngleConverter(float angle, int type)
{
    float PI = acos(0.0) / 90.0;
    if (type == 0)
    {
        return sin((90.0 - angle) * PI) / sin(angle * PI);
    }
    if (type == 1)
    {
        return sin(angle * PI) / sin((90.0 - angle) * PI);
    }

    return 0;
}

void SplitWater(Mat depth)
{
    float nearestWater = AngleConverter(angle, 0) * h; //最低像素区域距离船只的实际距离
    //float nearestWater = 12000;
    int pixel = ((AngleConverter(angle, 1) * maxDistance - h) / (AngleConverter(angle, 1) * maxDistance)) * height / 2;
    float distancePerPixel = (maxDistance - nearestWater) / pixel; //单位像素代表的距离
    std::cout << "Pixel:" << pixel << " nearestWater:" << nearestWater << " distancePerPixel:" << distancePerPixel << std::endl;

    for (int i = 1; i <= pixel + abs(USVLeftRightTiltAngle); i++)
    {
        ushort *p = depth.ptr<ushort>(height - i);

        int begin, end, depthChangeRate;

        if (USVLeftRightTiltAngle > 0)
        {
            begin = 0;
            end = std::min((float)width, width * ((i - USVLeftRightTiltAngle - pixel) / (-2 * USVLeftRightTiltAngle)));
            //depthChangeRate = (2 * USVLeftRightTiltAngle) / (sqrt(pow(width, 2) + pow(2 * USVLeftRightTiltAngle, 2)));
        }
        else if (USVLeftRightTiltAngle < 0)
        {
            begin = std::max((float)0, width * ((i - USVLeftRightTiltAngle - pixel) / (-2 * USVLeftRightTiltAngle)));
            end = width;
            //depthChangeRate = (2 * USVLeftRightTiltAngle) / (sqrt(pow(width, 2) + pow(2 * USVLeftRightTiltAngle, 2)));
        }
        else
        {
            begin = 0;
            end = width;
            depthChangeRate = 1;
        }

        for (int j = begin; j < end; j++)
        {
            //if (std::sqrt(pow(h, 2) + pow(nearestWater + i * distancePerPixel, 2) - fixDistance) > p[j])
            if (nearestWater + i * distancePerPixel + USVHeightChange < p[j])
            {
                //std::cout << p[j] << " ";

                p[j] = 65535;
            }
        }
        //getchar();
    }
}

//Depth Img Process
void ImgDisplay()
{
    // namedWindow("depth");
    // cvui::init("depth");
    Mat depth;
    while (waitKey(1) != 27)
    {
        struct timeval tstart, tend;
        double timer;
        gettimeofday(&tstart, NULL);

        {
            shared_lock<shared_mutex> lockColor(depthMutex);
            if (depthImg.empty())
            {
                continue;
            }
            depth = depthImg;
        }

        if (UseSplitWater)
            SplitWater(depth);

        //可调节远近距离，单位mm
        // cvui::window(depth, 900, 60, 300, 400, "Settings");
        // cvui::checkbox(depth, 915, 80, "Use Split Water", &UseSplitWater);
        // cvui::trackbar(depth, 915, 120, 270, &minDistance, 600, 10000);
        // cvui::trackbar(depth, 915, 180, 270, &maxDistance, 11000, 50000);
        // cvui::trackbar(depth, 915, 240, 270, &h, 300, 3000);
        // cvui::trackbar(depth, 915, 300, 270, &angle, (float)1.50, (float)10.0);
        // cvui::trackbar(depth, 915, 360, 270, &USVHeightChange, -10000, 10000);
        // cvui::trackbar(depth, 915, 420, 270, &USVLeftRightTiltAngle, (float)-360, (float)360);
        // cvui::update();

        imshow("depth", depth);

        gettimeofday(&tend, NULL);
        timer = 1000 * (tend.tv_sec - tstart.tv_sec) + tend.tv_usec - tstart.tv_usec;

        cout << timer << endl;
    }
}

//Read Buffer and Converter to Img
void ImgCallback()
{
    while (true)
    {
        EtronDI_GetDepthImage(pHandleEtronDI, &pDevSelInfo, buf, &depthImageSize, &pSerialNum, 0);

        unsigned short *pWSL, *pWS, *pDL, *pD;

        pWSL = (unsigned short *)buf;
        pDL = pDepthBuf;

        int minLength = fxAndBaseLine / (minDistance + 1500), maxLength = fxAndBaseLine / (maxDistance + 1500); //最后加减的数字为理论测试经过实际测量之后得到的误差修正

        int temp;

        for (int i = 0; i < height; i++)
        {
            pWS = pWSL;
            pD = pDL;
            for (int j = 0; j < width; j++)
            {
                if (pWS[j] < maxLength || pWS[j] > minLength)
                {
                    pD[0] = 0;
                }
                else
                {
                    temp = fxAndBaseLine / pWS[j];
                    pD[0] = (temp > 65535 ? 65535 : temp);
                }
                pD++;
            }
            pWSL += width;
            pDL += width;
        }
        {
            unique_lock<shared_mutex> lockDepth(depthMutex);
            depthImg = Mat(Size(1280, 720), CV_16UC1, pDepthBuf);
        }
    }
}

//Device Intialize
int OnInit()
{
    //Device PreInit
    if (EtronDI_Init(&hEtronDI, false) < 0)
    {
        EtronDI_Release(&hEtronDI);
        cout << endl
             << "Can't PreInit Device!" << endl;
        return -1;
    }
    else
    {
        cout << endl
             << "PreInit Device Succeed." << endl;
    }

    if (hEtronDI != nullptr)
    {
        int deviceCount = EtronDI_GetDeviceNumber(hEtronDI);
        if (deviceCount > 0)
        {
            devInfo.clear();
            for (int i = 0; i < deviceCount; i++)
            {
                DEVSELINFO devSelInfo;
                devSelInfo.index = i;
                DEVINFORMATION devInformation;
                EtronDI_GetDeviceInfo(hEtronDI, &devSelInfo, &devInformation);
                devInfo.push_back(devInformation);
            }
            EtronDI_Release(&hEtronDI);
            cout << endl
                 << "Get Number Details Succeed." << endl;
        }
        else
        {
            cout << endl
                 << "Can't find any usable device!" << endl;
            return -1;
        }
    }

    //Device Init
    for (size_t i = 0; i < devInfo.size(); i++)
    {
        if (strcmp(devInfo[i].strDevName, "/dev/video1") == 0)
        {
            pDevSelInfo.index = i;
        }
    }

    EtronDI_Init(&pHandleEtronDI, false);

    EtronDI_GetDeviceResolutionList(pHandleEtronDI, &pDevSelInfo, 64, pStreamColorInfo, 64, pStreamDepthInfo);

    width = pStreamDepthInfo[0].nWidth;
    height = pStreamDepthInfo[0].nHeight;

    EtronDI_SetDepthDataType(pHandleEtronDI, &pDevSelInfo, ETronDI_DEPTH_DATA_11_BITS);

    if (EtronDI_OpenDevice2(pHandleEtronDI, &pDevSelInfo, pStreamColorInfo[0].nWidth, pStreamColorInfo[0].nHeight, pStreamColorInfo[0].bFormatMJPG, pStreamDepthInfo[0].nWidth, pStreamDepthInfo[0].nHeight, DEPTH_IMG_NON_TRANSFER, false, NULL, &pFPS, IMAGE_SN_NONSYNC) == ETronDI_OK)
    {
        cout << endl
             << "Open Device Succeed." << endl;
    }
    else
    {
        cout << endl
             << "Open Device Failed!" << endl;
        return -1;
    }
    return 0;
}

int main()
{
    if (OnInit() != 0)
    {
        cout << endl
             << "Init Failed... Program Will Close Immediately." << endl;
        return -1;
    }
    else
    {
        cout << endl
             << "Init Succeed. Image Will Show Soon." << endl;
    }

    thread t1(ImgCallback);
    thread t2(ImgDisplay);
    t1.join();
    t2.join();
}
