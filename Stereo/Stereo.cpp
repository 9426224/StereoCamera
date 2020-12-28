#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <thread>

#include "Device.h"
#include "DepthImg.h"
#include "ColorImg.h"

#define WINDOW_NAME "Depth"

using namespace cv;

DepthImg* depthImg = nullptr;
ColorImg* colorImg = nullptr;

#pragma region StereoCameraParameters
Mat MatrixLeft = (Mat_<double>(3, 3) << 3399.661449120224, -60.4684392882234, 1387.703847940691, 0, 3301.42342302806, 105.2354073184998, 0, 0, 1);
Mat MatrixRight = (Mat_<double>(3, 3) << 3298.197804759359, -49.339212892906765, 921.474918072008, 0, 3292.678921154359, 146.1951800035569, 0, 0, 1);
Mat DistCoeffLeft = (Mat_<double>(5, 1) << 0.342488487687882, -5.69546388395641, -0.018196044666607, 0.050214891092326, 24.037102711420996);
Mat DistCoeffRight = (Mat_<double>(5, 1) << -0.230393310199259, 3.249681340745177, -0.027724938776735, 0.02396859739654, -21.708868924258418);
Mat Transcation = (Mat_<double>(3, 1) << -394.7730882974745, 4.033315779561094, -95.74162655063917);
Mat Rotation = (Mat_<double>(3, 3) << 0.989025767924407, 0.005703614198638, 0.14763298807045, -0.003428051080464, 0.999871443268512, -0.015663505416523, -0.147703347447938, 0.014985517048698, 0.988918174285141);
Mat Rl, Rr, Pl, Pr, Q; 
Mat mapLx, mapLy, mapRx, mapRy;//映射表
Rect validROIL, validROIR; //图像裁剪之后的区域
Size imageSize; //图像尺寸
#pragma endregion

//图像回调函数
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
	}

	int mindisparity = 0;
	int ndisparities = 64;
	int SADWindowsSize = 11;
	Ptr<StereoSGBM> sgbm = StereoSGBM::create(mindisparity, ndisparities, SADWindowsSize);

	// 预处理一次图像，得出后续处理参数
	while (false)
	{
		Mat color;
		if (colorImg->colorBuf.rows != 0)
		{
			{
				std::shared_lock<std::shared_mutex> lockDepth(colorImg->colorMutex);
				color = colorImg->colorBuf;
			}
			//图像分割成左右两部分
			Mat left = color(Rect(0, 0, color.cols / 2, color.rows));

			Mat right = color(Rect(color.cols / 2, 0, color.cols / 2, color.rows));

			imageSize = Size(left.cols, left.rows);

			//畸变系数计算
			//stereoRectify(MatrixLeft, DistCoeffLeft, MatrixRight, DistCoeffRight, imageSize, Rotation, Transcation, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);

			//对极线矫正
			//initUndistortRectifyMap(MatrixLeft, DistCoeffLeft, Rl, Pl, imageSize, CV_16SC2, mapLx, mapLy);
			//initUndistortRectifyMap(MatrixRight, DistCoeffRight, Rr, Pr, imageSize, CV_16SC2, mapRx, mapRy);

			//SGBM参数设置
			//int numberOfDisparities = ((imageSize.width / 8) + 15) & -16;
			//int SADWindowSize = 9;
			//int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
			//int cn = left.channels();
			//sgbm->setPreFilterCap(15); //预处理滤波器的截断值 [1-63]
			//sgbm->setBlockSize(sgbmWinSize);
			//sgbm->setP1(8 * cn * sgbmWinSize * sgbmWinSize);// 控制视察变化平滑性的参数，P1/P2值越大，视差越平滑，P2必须大于P1
			//sgbm->setP2(32 * cn * sgbmWinSize * sgbmWinSize);//P1是相邻像素点视差增/减1的惩罚系数，P2是相邻像素点视差变化值大于1时的惩罚系数
			//sgbm->setMinDisparity(0);//最小视差值，代表了匹配搜索从哪里开始
			//sgbm->setNumDisparities(numberOfDisparities);//表示最大搜索视差数
			//sgbm->setUniquenessRatio(6);//表示匹配功能函数
			//sgbm->setSpeckleWindowSize(100);//检查视差连通域，变化度的窗口大小
			//sgbm->setSpeckleRange(2);//视差变化阈值，当窗口内视差的变化大于阈值时，该窗口内的视差清零
			//sgbm->setDisp12MaxDiff(1);//1
			////sgbm->setMode(cv::StereoSGBM::MODE_HH);
			////sgbm->setMode(cv::StereoSGBM::MODE_SGBM);
			////sgbm->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);

			break;
		}
	}

	namedWindow(WINDOW_NAME);
	cvui::init(WINDOW_NAME);

	bool UseSplitWater = false;


	while (waitKey(1) != 27)
	{
		Mat color, depth, depthSource;
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

			//边缘点集查找
			//std::vector<std::vector<Point>> contours;
			//std::vector<Vec4i> hierarchy;
			//findContours(color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			//drawContours(depth, contours, -1, (0, 0, 255), 3);

			//图像分割成左右两部分
			//Mat left = color(Rect(0, 0, color.cols / 2, color.rows));
			//Mat right = color(Rect(color.cols / 2, 0, color.cols / 2, color.rows));

			//Mat disp, leftnew, rightnew;

			//remap图像为正确对极线位置
			//remap(left, leftnew, mapLx, mapLy, INTER_LINEAR);
			//remap(right, rightnew, mapRx, mapRy, INTER_LINEAR);
			//left = leftnew;
			//right = rightnew;

			//SGBM处理图像
			//sgbm->compute(left, right, disp);
			//imshow("disp", disp);
			//disp.convertTo(disp, CV_32F, 1.0 / 16);
			//Mat disp8U = Mat(disp.rows, disp.cols, CV_8UC1);
			//normalize(disp, disp8U, 0, 255, NORM_MINMAX, CV_8UC1);
			//imshow("parallex",disp8U);


			//imshow("left", left);
			//imshow("right", right);

		}
		if (depthImg->depthBuf.rows != 0)
		{
			{
				std::shared_lock<std::shared_mutex> lockColor(depthImg->depthMutex);
				depth = depthImg->depthBuf;
				depthSource = depthImg->depthSource;
			}
			/* 深度图像处理 */
			cvtColor(depth, depth, COLOR_BGR2GRAY); //三通道灰度图转单通道灰度图,为后续计算做准备
			cvtColor(depthSource, depthSource, COLOR_BGR2GRAY);

			//GaussianBlur(depth, depth, Size(5, 5), 0, 0); //高斯滤波

			medianBlur(depth, depth, 5); //中值滤波

			//blur(depth, depth, Size(3, 3), Point(-1, -1), 4);

			//快速连通域分析
			//depthImg->QuickDomainAnalysis(depthSource);

			if(UseSplitWater)
				depthImg->SplitWater(depthSource);

			//可调节远近距离，单位mm
			//cvui::window(depthSource, 900, 300, 180, 180, "Settings");

			cvui::checkbox(depthSource, 915, 80, "Use Split Water", &UseSplitWater);
			cvui::trackbar(depthSource, 915, 120, 270, &depthImg->minDistance, 600, 10000);
			cvui::trackbar(depthSource, 915, 180, 270, &depthImg->maxDistance, 11000, 50000);
			
			cvui::trackbar(depthSource, 915, 240, 270, &depthImg->h, 300, 3000);
			cvui::trackbar(depthSource, 915, 300, 270, &depthImg->angle , 2, 88);
			cvui::update();

			imshow(WINDOW_NAME, depthSource);
		}
	}

	return 0;
}