#include "DepthImg.h"

/// <summary>
/// Read parollax or depth images.
/// </summary>
/// <param name="buf">Return buffer from callback function.</param>
void DepthImg::Play(unsigned char* buf)
{
	BufferReader(buf);
	cv::Mat img(cv::Size(width, height), CV_8UC3, pDepthBuf);
	cv::Mat img2(cv::Size(width, height), CV_16UC3, pDepthSource);
	std::unique_lock<std::shared_mutex> lockDepth(depthMutex);
	depthBuf = img;
	depthSource = img2;
}

void DepthImg::BufferReader(unsigned char* buf)
{
	int nBPS;
	unsigned short* pWSL, * pWS ,* pDS,* pDSL;
	unsigned char* pDL,* pD;

	nBPS = ((width * 3 + 3) / 4) * 4;
	pWSL = (unsigned short*)buf;
	pDL = pDepthBuf;
	pDSL = pDepthSource;

	int minLength = fxAndBaseLine / (minDistance + 1500), maxLength = fxAndBaseLine / (maxDistance + 1500);

	for (int i = 0; i < height; i++)
	{
		pWS = pWSL;
		pD = pDL;
		pDS = pDSL;
		for (int j = 0; j < width; j++)
		{
			if (pWS[j] < maxLength || pWS[j]> minLength)
			{
				pD[0] = 0;
				pD[1] = 0;
				pD[2] = 0;
				pDS[0] = 0;
				pDS[1] = 0;
				pDS[2] = 0;
			}
			else
			{
				int temp = fxAndBaseLine / pWS[j];
				pD[0] = 255 - (pWS[j] - maxLength) * 255 / (minLength - maxLength);
				pD[1] = 255 - (pWS[j] - maxLength) * 255 / (minLength - maxLength);
				pD[2] = 255 - (pWS[j] - maxLength) * 255 / (minLength - maxLength);
				pDS[0] = (temp > 65535 ? 65535 : temp);
				pDS[1] = (temp > 65535 ? 65535 : temp);
				pDS[2] = (temp > 65535 ? 65535 : temp);
			}
			pD += 3;
			pDS += 3;
		}
		pWSL += width;
		pDL += nBPS;
		pDSL += nBPS;
	}
}


void DepthImg::QuickDomainAnalysis(cv::Mat depth)
{
	//快速连通域分析
	cv::Mat depth_thresh, depth_label, stats, centroids;

	cv::threshold(depth, depth_thresh, 0, 255, cv::THRESH_BINARY); //二值化阈值处理 大于0则置为255

	int nccomps = connectedComponentsWithStats(depth_thresh, depth_label, stats, centroids);

	std::vector<uchar> colors(nccomps);

	for (int i = 1; i < nccomps; i++)
	{
		colors[i] = stats.at<int>(i, cv::CC_STAT_AREA) < 300 ? 0 : 1;
	}

	for (int y = 0; y < depth_thresh.rows; y++)
	{
		for (int x = 0; x < depth_thresh.cols; x++)
		{
			int label = depth_label.at<int>(y, x);
			CV_Assert(0 <= label && label <= nccomps);
			depth_thresh.at<uchar>(y, x) = colors[label];
		}
	}

	depth = depth.mul(depth_thresh);
}

void DepthImg::SplitWater(cv::Mat depth)
{
	
	float distancePerPixel = (farthestWater - nearestWater) / pixel; //单位像素代表的距离

	for (int i = height - 1 ; i > height - pixel; i--)
	{
		for (int j = 0; j < width; j++)
		{
			if (distancePerPixel * i + nearestWater > depth.at<ushort>(i, j))
			{
				depth.at<ushort>(i, j) = 0;
			}
		}
	}
}