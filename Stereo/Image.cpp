#include "Image.h"

void Image::OpenNet()
{
    nanoDet = new NanoDet("./nanodet_m.param", "./nanodet_m.bin", true);
}

std::thread Image::GetImageThread()
{
    return std::thread([=] { GetImage(); });
}

std::thread Image::ProcessThread()
{
    return std::thread([=] { ProcessImage(); });
}

std::thread Image::SerialPortThread()
{
    return std::thread([=] { SendData(); });
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
            std::unique_lock<std::shared_mutex> lock(imgMutex);
            depthImg = cv::Mat(cv::Size(width, height), CV_16UC1, DepthBuf);

            cv::Mat yuy2(height, width, CV_8UC2, returnColorBuf);
            cv::Mat rgb(height, width, CV_8UC3);
            cv::cvtColor(yuy2, rgb, cv::COLOR_YUV2RGB_YUY2);
            colorImg = rgb;
        }
    }
}

void Image::ProcessImage()
{
    cv::Mat depth, color;

    while (cv::waitKey(1) != 27)
    {
        {
            std::shared_lock<std::shared_mutex> lock(imgMutex);
            if (depthImg.empty())
            {
                continue;
            }
            depth = depthImg;
            color = colorImg;
        }

        auto boundingBox = DetectImage(color);

        auto polar = DrawBoxes(depth, color, boundingBox);

        {
            std::unique_lock<std::shared_mutex> lock(imgMutex);
            PolarBox = polar;
        }
    }
}

void Image::SendData()
{
    std::vector<PolarInfo> polar;

    int serialPort = -1;

    serialPort = open("/dev/ttyTHS2", O_RDWR | O_NONBLOCK);

    if (serialPort == -1)
    {
        printf("Error: %s\n", strerror(errno));
    }

    struct termios options;

    tcgetattr(serialPort, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(serialPort, TCIFLUSH);
    tcsetattr(serialPort, TCSANOW, &options);

    while (serialPort != -1)
    {
        {
            std::shared_lock<std::shared_mutex> lock(imgMutex);
            polar = PolarBox;
        }

        std::string data;
        int count = 0;


        for(int i = 0;i < polar.size();i++)
        {
            continue;
            //write(serialPort, polar[i], polar.size());
        }

        write(serialPort, "aaaab", 5);
    }
}

std::vector<BoxInfo> Image::DetectImage(cv::Mat color)
{
    resizeWidth = nanoDet->inputSize;

    resizeHeight = floor((height / (float)width) * resizeWidth);

    cv::Mat resizedImg = cv::Mat::zeros(resizeWidth, resizeWidth, CV_8UC3);

    cv::Mat temp;

    cv::resize(color, temp, cv::Size(resizeWidth, resizeHeight));

    int index = floor((resizeWidth - resizeHeight) / 2.0);

    memcpy(resizedImg.data + index * resizeWidth * 3, temp.data, resizeHeight * resizeWidth * 3);

    return nanoDet->Detect(resizedImg, 0.3, 0.5);
}

std::vector<PolarInfo> Image::DrawBoxes(cv::Mat depth, cv::Mat color, std::vector<BoxInfo> boundingBox)
{
    float widthRatio = (float)width / (float)resizeWidth;
    float heightRatio = (float)height / (float)resizeHeight;

    int effectX = 0, effectY = floor((resizeWidth - resizeHeight) / 2.0);

    std::vector<PolarInfo> polarBox;

    for (size_t i = 0; i < boundingBox.size(); i++)
    {
        BoxInfo &singleBox = boundingBox[i];
        PolarInfo polar;

        singleBox.x1 = (singleBox.x1 - effectX) * widthRatio <= width ? ((singleBox.x1 - effectX) * widthRatio >= 0 ? (singleBox.x1 - effectX) * widthRatio : 0) : width;
        singleBox.x2 = (singleBox.x2 - effectX) * widthRatio <= width ? ((singleBox.x2 - effectX) * widthRatio >= 0 ? (singleBox.x2 - effectX) * widthRatio : 0) : width;
        singleBox.y1 = (singleBox.y1 - effectY) * heightRatio <= height ? ((singleBox.y1 - effectY) * heightRatio >= 0 ? (singleBox.y1 - effectY) * heightRatio : 0) : height;
        singleBox.y2 = (singleBox.y2 - effectY) * heightRatio <= height ? ((singleBox.y2 - effectY) * heightRatio >= 0 ? (singleBox.y2 - effectY) * heightRatio : 0) : height;

        cv::Rect rect(cv::Point(singleBox.x1, singleBox.y1),
                      cv::Point(singleBox.x2, singleBox.y2));

        //cv::cuda::GpuMat depthGpu(depth(rect));

        //cv::cuda::fastNlMeansDenoising(depthGpu, depthGpu, 3, 7, 21);

        //cv::cuda::bilateralFilter(depthGpu,depthGpu,-1,20,50);

        //cv::Mat d(depthGpu);

        //depth(rect) = d;

        //depth(rect) = PCA(depth(rect));

        //相框中心绘制，获取图像中心点作为识别物体中点，true表示使用灰度重心法获取图像的几何中心点，false返回图像坐标中心。
        cv::Point center = GrayCenter(depth(rect), singleBox, false);

        //获取极坐标中的表示物体距离零点的距离。
        polar.distance = ItemDistance(depth(rect)) - 1800;
        //double length = depth(rect).at<ushort>(singleBox.x2/2-singleBox.x1/2,singleBox.y2/2-singleBox.y1); //返回图像正中心距离信息。

        //获取极坐标中的表示物体区域范围大小的函数。
        polar.radius = FindPolygonRadius(singleBox.x1, singleBox.x2, polar.distance);

        //获取极坐标的角度信息。
        polar.angle = AngleConvertor(center);

        polarBox.push_back(polar);

        //std::cout << "Angle:" << polar.angle << " Radius:" << polar.radius << " Distance:" << polar.distance << std::endl;
    }

    return polarBox;
}

cv::Point Image::GrayCenter(cv::Mat depth, BoxInfo singleBox, bool isGrayCenter)
{
    cv::Point center;

    if (!isGrayCenter)
    {
        center.x = cvRound(singleBox.x1 / 2 + singleBox.x2 / 2);
        center.y = cvRound(singleBox.y1 / 2 + singleBox.y2 / 2);
        return center;
    }

    cv::MatIterator_<ushort> begin, end;
    double sum = 0;
    int threshold = 5000; //threshold means gray image's minimum usable distance.
    for (begin = depth.begin<ushort>(), end = depth.end<ushort>(); begin != end; begin++)
    {
        ((*begin) > threshold) ? sum += (*begin) : 0;
    }

    double x = 0, y = 0;

    for (int i = 0; i < depth.cols; i++)
    {
        for (int j = 0; j < depth.rows; j++)
        {
            double data = ((ushort *)depth.data)[i * 1280 + j];
            if (data < threshold)
            {
                data = 0;
            }
            x += i * data / sum;
            y += j * data / sum;
        }
    }

    center.x = cvRound(x);
    center.y = cvRound(y);

    return center;
}

inline double Image::FindPolygonRadius(float x1, float x2, int length)
{
    return ((double)x2 - x1) * length * 0.394024096 / 1280; // 0.221676 / 720;
}

double Image::ItemDistance(cv::Mat depth)
{
    double sum = 0, count = 0;
    for (int i = 0; i < depth.cols; i++)
    {
        for (int j = 0; j < depth.rows; j++)
        {
            double data = ((ushort *)depth.data)[i * 1280 + j];
            if (data <= 0)
            {
                continue;
            }
            sum += data;
            count++;
        }
    }
    return sum / count;
}

inline double Image::AngleConvertor(cv::Point center)
{
    return 100.98 - (double)center.x * anglePerPixel;
}

cv::Mat Image::PCA(cv::Mat depth)
{
    int layer = 1;

    cv::PCA pca(depth, cv::Mat(), cv::PCA::Flags::USE_AVG, layer);

    depth = pca.project(depth);

    depth = pca.backProject(depth);

    return depth;
}
