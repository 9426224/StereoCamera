#include "DepthImg.h"

DepthImg::~DepthImg()
{

}

/// <summary>
/// Create depth temporary buffer.
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
	}
}

/// <summary>
/// Read parollax or depth images.
/// </summary>
/// <param name="buf">Return buffer from callback function.</param>
void DepthImg::Play(unsigned char* buf)
{
	if (type == 8)
	{
		cv::Mat img(cv::Size(width * 2, height), CV_8UC1, buf);
		std::unique_lock<std::shared_mutex> lockDepth(depthMutex);
		depthBuf = img;
	}
	else
	{
		switch (type) 
		{
		case 11:
			BufferD11ConvertToGray(buf);
			break;
		case 14:
			BufferZ14ConvertToGray(buf);
			break;
		default:
			break;
		}
		cv::Mat img(cv::Size(width, height), CV_8UC3, pDepthBuf);
		std::unique_lock<std::shared_mutex> lockDepth(depthMutex);
		depthBuf = img;
	}
}

void DepthImg::BufferD11ConvertToGray(unsigned char* buf)
{
	int nBPS;
	unsigned short* pWSL, * pWS;
	unsigned char* pDL, * pD;

	nBPS = ((width * 3 + 3) / 4) * 4;
	pWSL = (unsigned short*)buf;
	pDL = pDepthBuf;

	int minLength = fxAndBaseLine / minDistance, maxLength = fxAndBaseLine / maxDistance;

	for (int i = 0; i < height; i++)
	{
		pWS = pWSL;
		pD = pDL;
		for (int j = 0; j < width; j++)
		{
			if (pWS[j] < maxLength || pWS[j]> minLength)
			{
				pD[0] = 0;//B
				pD[1] = 0;//G
				pD[2] = 0;//R
			}
			else
			{
				pD[0] =255- (pWS[j] - maxLength) * 255 / (minLength- maxLength);//B
				pD[1] =255- (pWS[j] - maxLength) * 255 / (minLength - maxLength);//G
				pD[2] =255- (pWS[j] - maxLength) * 255 / (minLength - maxLength);//R
			}
			pD += 3;
		}
		pWSL += width;
		pDL += nBPS;
	}

	int k = 1;
	return;
}

void DepthImg::BufferZ14ConvertToGray(unsigned char* buf)
{
	int nBPS;
	unsigned short* pWSL, * pWS;
	unsigned char* pDL, * pD;

	nBPS = ((width * 3 + 3) / 4) * 4;
	pWSL = (unsigned short*)buf;
	pDL = pDepthBuf;


	for (int y = 0; y < height; y++)
	{
		pD = pDL;
		for (int x = 0; x < width; x++)
		{
			pD[0] = pWSL[x] / 64; //B
			pD[1] = pWSL[x] / 64; //G
			pD[2] = pWSL[x] / 64; //R
			pD += 3;
		}
		pWSL += width;
		pDL += nBPS;
	}
}

cv::Mat DepthImg::QuickDomainAnalysis(cv::Mat depth)
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

	return depth;
}