#include "ColorImg.h"

void ColorImg::Play(unsigned char* buf)
{
	cv::Mat img(cv::Size(width, height), CV_8UC3, buf);

	std::unique_lock<std::shared_mutex> lockColor(colorMutex);
	flip(img, colorBuf, 0);	
} 

void ColorImg::PreProcess(cv::Mat color)
{
	int mindisparity = 0;
	int ndisparities = 64;
	int SADWindowsSize = 11;

	sgbm = cv::StereoSGBM::create(mindisparity, ndisparities, SADWindowsSize);

	//图像分割成左右两部分
	cv::Mat left = color(cv::Rect(0, 0, color.cols / 2, color.rows));

	cv::Mat right = color(cv::Rect(color.cols / 2, 0, color.cols / 2, color.rows));

	imageSize = cv::Size(left.cols, left.rows);

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

}

void ColorImg::ImageProcess(cv::Mat color)
{
	if (!preProcess)
	{
		PreProcess(color);
		preProcess = !preProcess;
		return;
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
}