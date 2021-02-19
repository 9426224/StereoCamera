#include "Image.h"

void Image::OpenNet()
{
    nanoDet = new NanoDet("./nanodet_m.param", "./nanodet_m.bin", true);
}

void Image::Process(cv::Mat depth, cv::Mat color)
{
    auto boundingBox = detectImage(color);

    float widthRatio = (float)width / (float)resizeWidth;
    float heightRatio = (float)height / (float)resizeHeight;

    int effectX = 0, effectY = floor((resizeWidth - resizeHeight) / 2.0);

    cv::Mat background(cv::Size(width, height), CV_16UC1, cv::Scalar(0));

    std::cout << boundingBox.size() << std::endl;

    for (size_t i = 0; i < boundingBox.size(); i++)
    {
        BoxInfo &singleBox = boundingBox[i];

        singleBox.x1 = (singleBox.x1 - effectX) * widthRatio;
        singleBox.x2 = (singleBox.x2 - effectX) * widthRatio;
        singleBox.y1 = (singleBox.y1 - effectY) * heightRatio;
        singleBox.y2 = (singleBox.y2 - effectY) * heightRatio;

        cv::Mat backgroundROI = background(cv::Rect(cv::Point(singleBox.x1, singleBox.y1),
                                                    cv::Point(singleBox.x2, singleBox.y2)));

        cv::Rect rect(cv::Point(singleBox.x1, singleBox.y1),
                      cv::Point(singleBox.x2, singleBox.y2));

        cv::Mat dp = depth(rect);

        depth(rect).copyTo(backgroundROI);
    }

    //cv::imshow("depth", background);

}

std::thread Image::GetImageThread()
{
    return std::thread([=] { GetImage(); });
}

std::thread Image::DisplayThread()
{
    return std::thread([=] { Display(); });
}

void Image::GetImage()
{
    while (true)
    {
        EtronDI_GetDepthImage(pHandleEtronDI, &pDevSelInfo, returnDepthBuf, &depthImageSize, &pSerialNum, 0);

        EtronDI_GetColorImage(pHandleEtronDI, &pDevSelInfo, returnColorBuf, &colorImageSize, &pSerialNum, 0);

        unsigned short
            *pReturnDepthBuf,
            *pReturnDepthBufLine,
            *pDepthBuf,
            *pDepthBufLine;

        int temp;

        pReturnDepthBuf = (unsigned short *)returnDepthBuf;
        pDepthBuf = DepthBuf;

        int minLength = fxAndBaseLine / (minDistance + 1500), maxLength = fxAndBaseLine / (maxDistance + 1500);

        for (int i = 0; i < height; i++)
        {
            pReturnDepthBufLine = pReturnDepthBuf;
            pDepthBufLine = pDepthBuf;

            for (int j = 0; j < width; j++)
            {
                if (pReturnDepthBufLine[j] < maxLength || pReturnDepthBufLine[j] > minLength)
                {
                    pDepthBufLine[0] = 0;
                }
                else
                {
                    temp = fxAndBaseLine / pReturnDepthBufLine[j];
                    pDepthBufLine[0] = (temp > 65535 ? 65535 : temp);
                }
                pDepthBufLine++;
            }

            pReturnDepthBuf += width;
            pDepthBuf += width;
        }

        {
            std::unique_lock<std::shared_mutex> lockImage(imgMutex);
            depthImg = cv::Mat(cv::Size(width, height), CV_16UC1, DepthBuf);

            cv::Mat yuy2(height, width, CV_8UC2, returnColorBuf);
            cv::Mat rgb(height, width, CV_8UC3);
            cv::cvtColor(yuy2, rgb, cv::COLOR_YUV2BGRA_YUY2);
            cv::cvtColor(rgb, colorImg, cv::COLOR_BGR2RGB);
            //cv::flip(img, colorImg ,0);
        }
    }
}

void Image::Display()
{
    cv::Mat depth, color;

    while (cv::waitKey(1) != 27)
    {
        // timespec t1, t2;
        // clock_gettime(CLOCK_MONOTONIC, &t1);

        {
            std::shared_lock<std::shared_mutex> lockImage(imgMutex);
            if (depthImg.empty())
            {
                continue;
            }
            depth = depthImg;
            color = colorImg;
        }

        Process(depth, color);

        //cv::imshow("depth", depth);
        //cv::imshow("color", color);

        // clock_gettime(CLOCK_MONOTONIC, &t2);
        // long long deltaT = (t2.tv_sec - t1.tv_sec) * 10 ^ 3 + t2.tv_nsec - t1.tv_nsec;

        // std::cout << deltaT << std::endl;
    }
}

std::vector<BoxInfo> Image::detectImage(cv::Mat color)
{
    resizeWidth = nanoDet->inputSize;

    resizeHeight = floor((height / (float)width) * resizeWidth);

    cv::Mat resizedImg = cv::Mat::zeros(resizeWidth, resizeWidth, CV_8UC3);

    cv::Mat temp;

    cv::resize(color, temp, cv::Size(resizeWidth, resizeHeight));

    int index = floor((resizeWidth - resizeHeight) / 2.0);

    memcpy(resizedImg.data + index * resizeWidth * 3, temp.data, resizeHeight * resizeWidth * 3);

    return nanoDet->Detect(resizedImg, 0.4, 0.5);
}