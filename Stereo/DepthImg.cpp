#include "DepthImg.h"

using namespace cv;

/// <summary>
/// 析构函数
/// </summary>
DepthImg::~DepthImg()
{

}

/// <summary>
/// 深度图像暂存变量创建
/// </summary>
void DepthImg::Init()
{
	if (type == 8)
	{
		pDepthBuf = (unsigned char*)realloc(pDepthBuf, width * height * 2 * sizeof(unsigned char));
	}
	else
	{
		pDepthBuf = (unsigned char*)realloc(pDepthBuf, width * height * 3 * sizeof(unsigned char));
		switch (type)
		{
		case 11: 
			DmColorMode11(ColorPaletteD11, 0);
			break;
		case 14:
			DmColorMode14(ColorPaletteZ14, 0);
			break;
		default:
			printf("Wrong Depth Type");
			break;
		}
	}
}

/// <summary>
/// 读取视差图或深度距离图
/// </summary>
/// <param name="buf">深度图像流</param>
void DepthImg::Play(unsigned char* buf)
{
	if (type == 8)
	{
		Mat img(Size(width * 2, height), CV_8UC1, buf);
		depthBuf = img;
	}
	else
	{
		switch (type) 
		{
		case 11:
			UpdateD11DisplayImage_DIB24(ColorPaletteD11, buf, pDepthBuf, width, height);
			break;
		case 14:
			UpdateZ14DisplayImage_DIB24(ColorPaletteZ14, buf, pDepthBuf, width, height);
			break;
		default:
			break;
		}
		Mat img(Size(width, height), CV_8UC3, pDepthBuf);
		depthBuf = img;
	}
	if (zoom != 1.0)
	{
		resize(depthBuf, depthBuf, Size(depthBuf.cols * zoom, depthBuf.rows * zoom), 0, 0, INTER_LINEAR);
	}
	//GaussianBlur(depthBuf, depthBuf, Size(5, 5), 0, 0); //高斯滤波

	//imshow("Depth Image", depthBuf);
	DepthImageProcess(depthBuf);

	key = waitKey(1);

	depthMutex.lock();
	if (key == 'S')
	{
		zoom = (zoom == 1.0) ? 0.5 : 1.0;
	}
	depthMutex.unlock();
}

void DepthImg::DepthImageProcess(Mat img)
{
    img = insertDepth32f(img);

	imshow("Depth",img);
}

Mat insertDepth32f(cv::Mat& depth)
{
    const int width = depth.cols;
    const int height = depth.rows;
    float* data = (float*)depth.data;
    cv::Mat integralMap = cv::Mat::zeros(height, width, CV_64F);
    cv::Mat ptsMap = cv::Mat::zeros(height, width, CV_32S);
    double* integral = (double*)integralMap.data;
    int* ptsIntegral = (int*)ptsMap.data;
    memset(integral, 0, sizeof(double) * width * height);
    memset(ptsIntegral, 0, sizeof(int) * width * height);
    for (int i = 0; i < height; ++i)
    {
        int id1 = i * width;
        for (int j = 0; j < width; ++j)
        {
            int id2 = id1 + j;
            if (data[id2] > 1e-3)
            {
                integral[id2] = data[id2];
                ptsIntegral[id2] = 1;
            }
        }
    }
    // 积分区间
    for (int i = 0; i < height; ++i)
    {
        int id1 = i * width;
        for (int j = 1; j < width; ++j)
        {
            int id2 = id1 + j;
            integral[id2] += integral[id2 - 1];
            ptsIntegral[id2] += ptsIntegral[id2 - 1];
        }
    }
    for (int i = 1; i < height; ++i)
    {
        int id1 = i * width;
        for (int j = 0; j < width; ++j)
        {
            int id2 = id1 + j;
            integral[id2] += integral[id2 - width];
            ptsIntegral[id2] += ptsIntegral[id2 - width];
        }
    }
    int wnd;
    double dWnd = 2;
    while (dWnd > 1)
    {
        wnd = int(dWnd);
        dWnd /= 2;
        for (int i = 0; i < height; ++i)
        {
            int id1 = i * width;
            for (int j = 0; j < width; ++j)
            {
                int id2 = id1 + j;
                int left = j - wnd - 1;
                int right = j + wnd;
                int top = i - wnd - 1;
                int bot = i + wnd;
                left = max(0, left);
                right = min(right, width - 1);
                top = max(0, top);
                bot = min(bot, height - 1);
                int dx = right - left;
                int dy = (bot - top) * width;
                int idLeftTop = top * width + left;
                int idRightTop = idLeftTop + dx;
                int idLeftBot = idLeftTop + dy;
                int idRightBot = idLeftBot + dx;
                int ptsCnt = ptsIntegral[idRightBot] + ptsIntegral[idLeftTop] - (ptsIntegral[idLeftBot] + ptsIntegral[idRightTop]);
                double sumGray = integral[idRightBot] + integral[idLeftTop] - (integral[idLeftBot] + integral[idRightTop]);
                if (ptsCnt <= 0)
                {
                    continue;
                }
                data[id2] = float(sumGray / ptsCnt);
            }
        }
        int s = wnd / 2 * 2 + 1;
        if (s > 201)
        {
            s = 201;
        }
        cv::GaussianBlur(depth, depth, cv::Size(s, s), s, s);
    }
    return depth;
}