#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "eSPDI.h"

#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include "time.h"

#include "NanoDet.h"

class Image
{
public:
    Image(void *pHandleEtronDI, DEVSELINFO pDevSelInfo)
    {
        this->pHandleEtronDI = pHandleEtronDI;
        this->pDevSelInfo = pDevSelInfo;

        DepthBuf = (unsigned short *)realloc(DepthBuf, sizeof(unsigned short) * 1280 * 720 * 3);
        ColorBuf = (unsigned char *)realloc(ColorBuf, sizeof(unsigned char) * 1280 * 720 * 3);

        returnDepthBuf = (unsigned char *)realloc(returnDepthBuf, sizeof(unsigned char) * 1280 * 720 * 3);
        returnColorBuf = (unsigned char *)realloc(returnColorBuf, sizeof(unsigned char) * 1280 * 720 * 3);
    }

    std::thread GetImageThread();
    std::thread DisplayThread();
    void OpenNet();

private:
    void Process(cv::Mat,cv::Mat);
    void GetImage();
    void Display();
    std::vector<BoxInfo> detectImage(cv::Mat);

    NanoDet *nanoDet;
    cv::Mat depthImg, colorImg;
    std::shared_mutex imgMutex;
    void *pHandleEtronDI;
    DEVSELINFO pDevSelInfo;

    unsigned char *returnDepthBuf;
    unsigned char *returnColorBuf;

    unsigned short *DepthBuf;
    unsigned char *ColorBuf;

    int pSerialNum = 0;
    int width = 1280, height = 720 , resizeWidth, resizeHeight;
    unsigned long depthImageSize = 0, colorImageSize = 0;
    int maxDistance = 16560, minDistance = 5000;
    int fxAndBaseLine = 12821030;
};