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

	int min = 99999, max = 0;

	for (int i = 0; i < height; i++)
	{
		pWS = pWSL;
		pD = pDL;
		for (int j = 0; j < width; j++)
		{
			if(false)
			//if (pWS[j] < 100 || pWS[j]> 1800)
			{
				pD[0] = 0;//B
				pD[1] = 0;//G
				pD[2] = 0;//R
			}
			else
			//{
			//	pD[0] = pWS[j] / 8;//B
			//	pD[1] = pWS[j] / 8;//G
			//	pD[2] = pWS[j] / 8;//R
			//}
			{
				//pD[0] = 255 - pWS[j] / 8;//B
				//pD[1] = 255 - pWS[j] / 8;//G
				//pD[2] = 255- -pWS[j] / 8;//R
				pD[0] = 1282103.2224 / pWS[j];
				pD[1] = 1282103.2224 / pWS[j];
				pD[2] = 1282103.2224 / pWS[j];

				if (pD[0] < min)
				{
					min = pD[0];
				}
				if (pD[0] > max)
				{
					max = pD[0];
				}
			}
			pD += 3;
		}
		pWSL += width;
		pDL += nBPS;
	}
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