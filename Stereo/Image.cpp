#include "Image.h"

void Image::OpenNet()
{
    nanoDet = new NanoDet("./nanodet_m.param", "./nanodet_m.bin", true);
}

cv::Mat Image::Process(cv::Mat depth, cv::Mat color)
{
    auto boundingBox = DetectImage(color);

    depth = DrawBoxes(depth, color, boundingBox);
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
            cv::cvtColor(yuy2, rgb, cv::COLOR_YUV2RGB_YUY2);
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

    return nanoDet->Detect(resizedImg, 0.4, 0.5);
}
const int color_list[80][3] =
    {
        //{255 ,255 ,255}, //bg
        {216, 82, 24},
        {236, 176, 31},
        {125, 46, 141},
        {118, 171, 47},
        {76, 189, 237},
        {238, 19, 46},
        {76, 76, 76},
        {153, 153, 153},
        {255, 0, 0},
        {255, 127, 0},
        {190, 190, 0},
        {0, 255, 0},
        {0, 0, 255},
        {170, 0, 255},
        {84, 84, 0},
        {84, 170, 0},
        {84, 255, 0},
        {170, 84, 0},
        {170, 170, 0},
        {170, 255, 0},
        {255, 84, 0},
        {255, 170, 0},
        {255, 255, 0},
        {0, 84, 127},
        {0, 170, 127},
        {0, 255, 127},
        {84, 0, 127},
        {84, 84, 127},
        {84, 170, 127},
        {84, 255, 127},
        {170, 0, 127},
        {170, 84, 127},
        {170, 170, 127},
        {170, 255, 127},
        {255, 0, 127},
        {255, 84, 127},
        {255, 170, 127},
        {255, 255, 127},
        {0, 84, 255},
        {0, 170, 255},
        {0, 255, 255},
        {84, 0, 255},
        {84, 84, 255},
        {84, 170, 255},
        {84, 255, 255},
        {170, 0, 255},
        {170, 84, 255},
        {170, 170, 255},
        {170, 255, 255},
        {255, 0, 255},
        {255, 84, 255},
        {255, 170, 255},
        {42, 0, 0},
        {84, 0, 0},
        {127, 0, 0},
        {170, 0, 0},
        {212, 0, 0},
        {255, 0, 0},
        {0, 42, 0},
        {0, 84, 0},
        {0, 127, 0},
        {0, 170, 0},
        {0, 212, 0},
        {0, 255, 0},
        {0, 0, 42},
        {0, 0, 84},
        {0, 0, 127},
        {0, 0, 170},
        {0, 0, 212},
        {0, 0, 255},
        {0, 0, 0},
        {36, 36, 36},
        {72, 72, 72},
        {109, 109, 109},
        {145, 145, 145},
        {182, 182, 182},
        {218, 218, 218},
        {0, 113, 188},
        {80, 182, 188},
        {127, 127, 0},
};

cv::Mat Image::DrawBoxes(cv::Mat depth, cv::Mat color, std::vector<BoxInfo> boundingBox)
{
    cv::cvtColor(color, color, cv::COLOR_RGB2BGR);
    static const char *class_names[] = {"person", "bicycle", "car", "motorcycle", "airplane", "bus",
                                        "train", "truck", "boat", "traffic light", "fire hydrant",
                                        "stop sign", "parking meter", "bench", "bird", "cat", "dog",
                                        "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe",
                                        "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                                        "skis", "snowboard", "sports ball", "kite", "baseball bat",
                                        "baseball glove", "skateboard", "surfboard", "tennis racket",
                                        "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl",
                                        "banana", "apple", "sandwich", "orange", "broccoli", "carrot",
                                        "hot dog", "pizza", "donut", "cake", "chair", "couch",
                                        "potted plant", "bed", "dining table", "toilet", "tv", "laptop",
                                        "mouse", "remote", "keyboard", "cell phone", "microwave", "oven",
                                        "toaster", "sink", "refrigerator", "book", "clock", "vase",
                                        "scissors", "teddy bear", "hair drier", "toothbrush"};

    float widthRatio = (float)width / (float)resizeWidth;
    float heightRatio = (float)height / (float)resizeHeight;

    int effectX = 0, effectY = floor((resizeWidth - resizeHeight) / 2.0);

    cv::Mat background(cv::Size(width, height), CV_16UC1, cv::Scalar(0));

    //std::cout << boundingBox.size() << " Items:" << std::endl;

    for (size_t i = 0; i < boundingBox.size(); i++)
    {
        BoxInfo &singleBox = boundingBox[i];

        singleBox.x1 = (singleBox.x1 - effectX) * widthRatio <= width ? ((singleBox.x1 - effectX) * widthRatio >= 0 ? (singleBox.x1 - effectX) * widthRatio : 0) : width;
        singleBox.x2 = (singleBox.x2 - effectX) * widthRatio <= width ? ((singleBox.x2 - effectX) * widthRatio >= 0 ? (singleBox.x2 - effectX) * widthRatio : 0) : width;
        singleBox.y1 = (singleBox.y1 - effectY) * heightRatio <= height ? ((singleBox.y1 - effectY) * heightRatio >= 0 ? (singleBox.y1 - effectY) * heightRatio : 0) : height;
        singleBox.y2 = (singleBox.y2 - effectY) * heightRatio <= height ? ((singleBox.y2 - effectY) * heightRatio >= 0 ? (singleBox.y2 - effectY) * heightRatio : 0) : height;

        cv::Mat backgroundROI = background(cv::Rect(cv::Point(singleBox.x1, singleBox.y1),
                                                    cv::Point(singleBox.x2, singleBox.y2)));

        cv::Rect rect(cv::Point(singleBox.x1, singleBox.y1),
                      cv::Point(singleBox.x2, singleBox.y2));

        //cv::cuda::GpuMat depthGpu(depth(rect));

        //cv::cuda::fastNlMeansDenoising(depthGpu, depthGpu, 3, 7, 21);

        cv::Point center = GrayCenter(depth(rect));

        double length = ItemDistance(depth(rect)) - 1400;

        double radius = FindPolygonRadius(depth(rect).rows, depth(rect).cols, center, length);

        double angle = AngleConvertor(center, singleBox.x1, singleBox.x2);

        depth(rect).copyTo(backgroundROI);

        cv::Scalar clrlist = cv::Scalar(color_list[singleBox.label][0], color_list[singleBox.label][1], color_list[singleBox.label][2]);

        cv::rectangle(color, cv::Rect(cv::Point(singleBox.x1, singleBox.y1), cv::Point(singleBox.x2, singleBox.y2)), clrlist);

        char text[256];
        sprintf(text, "%s %.1f%% %.2f %.2fm %.2fmm", class_names[singleBox.label], singleBox.score * 100, angle, length / 1000, radius);
        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.6, 1, &baseLine);

        int x = singleBox.x1;
        int y = singleBox.y1 - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > color.cols)
            x = color.cols - label_size.width;

        cv::rectangle(color, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      clrlist, -1);

        cv::putText(color, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
    }
    cv::imshow("color", color);
    cv::imshow("depth", depth);
    return background;
}

cv::Point Image::GrayCenter(cv::Mat depth)
{
    cv::Point center;
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

double Image::FindPolygonRadius(int width, int height, cv::Point center, double distance)
{
    if (center.x >= width / 2)
    {
        if (center.y >= height / 2)
        {
            return (double)(sqrt(pow(center.x, 2) + pow(center.y, 2))) * distance * 0.23 / 720;
        }
        else
        {
            return (double)(sqrt(pow(center.x, 2) + pow(height - center.y, 2))) * distance * 0.23 / 720;
        }
    }
    else
    {
        if (center.y >= height / 2)
        {
            return (double)(sqrt(pow(width - center.x, 2) + pow(center.y, 2)));
        }
        else
        {
            return (double)(sqrt(pow(width - center.x, 2) + pow(height - center.y, 2)));
        }
    }
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

double Image::AngleConvertor(cv::Point center, float x, float y)
{
    return 101.96 - ((double)x + center.x) * anglePerPixel;
}
