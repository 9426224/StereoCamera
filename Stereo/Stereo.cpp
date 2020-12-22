#include <thread>

#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

using namespace cv;

DepthImg* depthImg = nullptr;
ColorImg* colorImg = nullptr;

#pragma region StereoCameraParameters
Mat MatrixLeft = (Mat_<double>(3, 3) << 3399.661449120224, -60.4684392882234, 1387.703847940691, 0, 3301.42342302806, 105.2354073184998, 0, 0, 1);
Mat MatrixRight = (Mat_<double>(3, 3) << 3298.197804759359, -49.339212892906765, 921.474918072008, 0, 3292.678921154359, 146.1951800035569, 0, 0, 1);
Mat DistCoeffLeft = (Mat_<double>(3, 3) << 0.342488487687882, -5.69546388395641, -0.018196044666607, 0.050214891092326, 24.037102711420996);
Mat DistCoeffRight = (Mat_<double>(3, 3) << -0.230393310199259, 3.249681340745177, -0.027724938776735, 0.02396859739654, -21.708868924258418);
Mat Transcation = (Mat_<double>(3, 1) << -394.7730882974745, 4.033315779561094, -95.74162655063917);
Mat Rotation = (Mat_<double>(3, 3) << 0.989025767924407, 0.005703614198638, 0.14763298807045, -0.003428051080464, 0.999871443268512, -0.015663505416523, -0.147703347447938, 0.014985517048698, 0.988918174285141);
#pragma endregion

/// <summary>
/// Image Callback Function
/// </summary>
/// <param name="imgType">Type of return images.</param>
/// <param name="imgId"></param>
/// <param name="imgBuf">Images' buffer.</param>
/// <param name="imgSize"></param>
/// <param name="width">Images' width.</param>
/// <param name="height">Images' height.</param>
/// <param name="serialNumber"></param>
/// <param name="pParam"></param>
void ImgCallback(LenaDDIImageType::Value imgType, int imgId, unsigned char* imgBuf, int imgSize, int width, int height, int serialNumber, void* pParam)
{
	if (LenaDDIImageType::IsImageColor(imgType))
	{
		colorImg->Play(imgBuf);
	}
	else if (LenaDDIImageType::IsImageDepth(imgType))
	{
		depthImg->Play(imgBuf);
	}
	else
	{
		printf_s("Unknown Image Type\n");
	}
}

int main()
{
	Device device;
	device.callbackFn = ImgCallback;
	device.Init();

	colorImg = new ColorImg(device.pStreamColorInfo[device.colorResolution].nWidth,
		device.pStreamColorInfo[device.colorResolution].nHeight);

	if (device.depthResolution != -1)
	{
		depthImg = new DepthImg(device.pStreamDepthInfo[device.depthResolution].nWidth,
			device.pStreamDepthInfo[device.depthResolution].nHeight,
			device.USBType ? device.depthOption : 8);
		depthImg->Init();
	}

	while (true)
	{
		Mat color, depth;
		if (colorImg->colorBuf.rows != 0)
		{
			{
				std::shared_lock<std::shared_mutex> lockDepth(colorImg->colorMutex);
				color = colorImg->colorBuf;
			}

			/* 彩色图像处理 */
			//cvtColor(color, color, COLOR_BGR2GRAY); //彩色图像转为灰度图像

			//GaussianBlur(color, color, Size(5, 5), 0, 0); //高斯滤波

			//threshold(color, color, 0, 255, THRESH_OTSU);

			//Canny(color, color, 50, 500, 3); //Canny算子边缘检测

			//std::vector<std::vector<Point>> contours;
			//std::vector<Vec4i> hierarchy;
			//findContours(color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			//drawContours(depth, contours, -1, (0, 0, 255), 3);
			
			//图像分割成左右两部分
			Mat left = color(Rect(0, 0, color.cols / 2, color.rows));

			Mat right = color(Rect(color.cols/2, 0, color.cols / 2, color.rows));


			Mat Rl, Rr, Pl, Pr, Q;

			Mat mapLx, mapLy, mapRx, mapRy;//映射表

			Rect validROIL, validROLR; //图像裁剪之后的区域

			Size imageSize = Size(left.cols, left.rows); //图像尺寸

			//畸变矫正
			stereoRectify(MatrixLeft, DistCoeffLeft, MatrixRight, DistCoeffRight, imageSize, Rotation, Transcation, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY, 0, imageSize, &validROIL, &validROLR);

			//对极线矫正
			initUndistortRectifyMap(MatrixLeft, DistCoeffLeft, Rl, Pl, imageSize, CV_16SC2, mapLx, mapLy);
			initUndistortRectifyMap(MatrixLeft, DistCoeffLeft, Rl, Pl, imageSize, CV_16SC2, mapLx, mapLy);

			remap(left, left, mapLx, mapLy, INTER_LINEAR); 
			remap(right, right, mapRx, mapRy, INTER_LINEAR);

			namedWindow("left", 0);
			namedWindow("right", 0);

			imshow("left", left);
			imshow("right", right);

		}
		if (device.depthResolution != -1 && depthImg->depthBuf.rows != 0) 
		{
			{
				std::shared_lock<std::shared_mutex> lockColor(depthImg->depthMutex);
				depth = depthImg->depthBuf;
			}
			/* 深度图像处理 */
			cvtColor(depth, depth, COLOR_BGR2GRAY); //三通道灰度图转单通道灰度图,为后续计算做准备

			//GaussianBlur(depth, depth, Size(5, 5), 0, 0); //高斯滤波

			medianBlur(depth, depth, 5); //中值滤波

			//threshold(depth, depth, 0, 255, THRESH_OTSU); //大津法 二值化阈值处理

			imshow("depth", depth);
		}

		waitKey(1);
	}

	return 0;
}