#include "Image.h"

void Image::Process(cv::Mat depth)
{
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

            colorImg = rgb;
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

        Process(depth);

        cv::imshow("depth", depth);
        cv::imshow("color", color);

        // clock_gettime(CLOCK_MONOTONIC, &t2);
        // long long deltaT = (t2.tv_sec - t1.tv_sec) * 10 ^ 3 + t2.tv_nsec - t1.tv_nsec;

        // std::cout << deltaT << std::endl;
    }
}

std::vector<BoxInfo> Image::detectImage(cv::Mat color)
{
    object_rect effect_roi;

    int resize_w = 320;

    int resize_h = floor((height / width) * resize_w);

    cv::Mat resized_img(cv::Size(resize_w, resize_w), CV_8UC3, cv::Scalar(0));

    cv::Mat temp;

    cv::resize(color, temp, cv::Size(resize_w, resize_h));

    memcpy(resized_img.data + floor((resize_w - resize_h) / 2) * resize_w * 3, temp.data, resize_w * resize_h * 3);

    effect_roi.x = 0;
    effect_roi.y = floor((resize_w - resize_h) / 2);
    effect_roi.width = resize_w;
    effect_roi.height = resize_h;

    return detector.detect(resized_img, 0.4, 0.5);
}