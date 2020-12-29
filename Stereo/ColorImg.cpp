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

	//ͼ��ָ������������
	cv::Mat left = color(cv::Rect(0, 0, color.cols / 2, color.rows));

	cv::Mat right = color(cv::Rect(color.cols / 2, 0, color.cols / 2, color.rows));

	imageSize = cv::Size(left.cols, left.rows);

	//����ϵ������
	//stereoRectify(MatrixLeft, DistCoeffLeft, MatrixRight, DistCoeffRight, imageSize, Rotation, Transcation, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);

	//�Լ��߽���
	//initUndistortRectifyMap(MatrixLeft, DistCoeffLeft, Rl, Pl, imageSize, CV_16SC2, mapLx, mapLy);
	//initUndistortRectifyMap(MatrixRight, DistCoeffRight, Rr, Pr, imageSize, CV_16SC2, mapRx, mapRy);

	//SGBM��������
	//int numberOfDisparities = ((imageSize.width / 8) + 15) & -16;
	//int SADWindowSize = 9;
	//int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
	//int cn = left.channels();
	//sgbm->setPreFilterCap(15); //Ԥ�����˲����Ľض�ֵ [1-63]
	//sgbm->setBlockSize(sgbmWinSize);
	//sgbm->setP1(8 * cn * sgbmWinSize * sgbmWinSize);// �����Ӳ�仯ƽ���ԵĲ�����P1/P2ֵԽ���Ӳ�Խƽ����P2�������P1
	//sgbm->setP2(32 * cn * sgbmWinSize * sgbmWinSize);//P1���������ص��Ӳ���/��1�ĳͷ�ϵ����P2���������ص��Ӳ�仯ֵ����1ʱ�ĳͷ�ϵ��
	//sgbm->setMinDisparity(0);//��С�Ӳ�ֵ��������ƥ�����������￪ʼ
	//sgbm->setNumDisparities(numberOfDisparities);//��ʾ��������Ӳ���
	//sgbm->setUniquenessRatio(6);//��ʾƥ�书�ܺ���
	//sgbm->setSpeckleWindowSize(100);//����Ӳ���ͨ�򣬱仯�ȵĴ��ڴ�С
	//sgbm->setSpeckleRange(2);//�Ӳ�仯��ֵ�����������Ӳ�ı仯������ֵʱ���ô����ڵ��Ӳ�����
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

	/* ��ɫͼ���� */
	//cvtColor(color, color, COLOR_BGR2GRAY); //��ɫͼ��תΪ�Ҷ�ͼ��

	//GaussianBlur(color, color, Size(5, 5), 0, 0); //��˹�˲�

	//threshold(color, color, 0, 255, THRESH_OTSU);

	//Canny(color, color, 50, 500, 3); //Canny���ӱ�Ե���

	//��Ե�㼯����
	//std::vector<std::vector<Point>> contours;
	//std::vector<Vec4i> hierarchy;
	//findContours(color, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//drawContours(depth, contours, -1, (0, 0, 255), 3);

	//ͼ��ָ������������
	//Mat left = color(Rect(0, 0, color.cols / 2, color.rows));
	//Mat right = color(Rect(color.cols / 2, 0, color.cols / 2, color.rows));

	//Mat disp, leftnew, rightnew;

	//remapͼ��Ϊ��ȷ�Լ���λ��
	//remap(left, leftnew, mapLx, mapLy, INTER_LINEAR);
	//remap(right, rightnew, mapRx, mapRy, INTER_LINEAR);
	//left = leftnew;
	//right = rightnew;

	//SGBM����ͼ��
	//sgbm->compute(left, right, disp);
	//imshow("disp", disp);
	//disp.convertTo(disp, CV_32F, 1.0 / 16);
	//Mat disp8U = Mat(disp.rows, disp.cols, CV_8UC1);
	//normalize(disp, disp8U, 0, 255, NORM_MINMAX, CV_8UC1);
}