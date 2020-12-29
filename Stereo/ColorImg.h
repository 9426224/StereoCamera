#include "opencv2/opencv.hpp"

#include <shared_mutex>
#include <mutex>

class ColorImg {
public:
	ColorImg(int width, int height) : width(width), height(height) {}

	void Play(unsigned char* buf);
	void ImageProcess(cv::Mat);

	bool preProcess = false; // 表示是否对图像进行预处理并得到相对应的数据
	int width, height; //彩色视频的宽度与高度
	cv::Mat colorBuf; //彩色视频图像存储数组
	mutable std::shared_mutex colorMutex; //用于彩色图像输出时进行锁定防止访问冲突

private:
	void PreProcess(cv::Mat);

	cv::Ptr<cv::StereoSGBM> sgbm;
	cv::Mat MatrixLeft = (cv::Mat_<double>(3, 3) << 3399.661449120224, -60.4684392882234, 1387.703847940691, 0, 3301.42342302806, 105.2354073184998, 0, 0, 1);
	cv::Mat MatrixRight = (cv::Mat_<double>(3, 3) << 3298.197804759359, -49.339212892906765, 921.474918072008, 0, 3292.678921154359, 146.1951800035569, 0, 0, 1);
	cv::Mat DistCoeffLeft = (cv::Mat_<double>(5, 1) << 0.342488487687882, -5.69546388395641, -0.018196044666607, 0.050214891092326, 24.037102711420996);
	cv::Mat DistCoeffRight = (cv::Mat_<double>(5, 1) << -0.230393310199259, 3.249681340745177, -0.027724938776735, 0.02396859739654, -21.708868924258418);
	cv::Mat Transcation = (cv::Mat_<double>(3, 1) << -394.7730882974745, 4.033315779561094, -95.74162655063917);
	cv::Mat Rotation = (cv::Mat_<double>(3, 3) << 0.989025767924407, 0.005703614198638, 0.14763298807045, -0.003428051080464, 0.999871443268512, -0.015663505416523, -0.147703347447938, 0.014985517048698, 0.988918174285141);
	cv::Mat Rl, Rr, Pl, Pr, Q;
	cv::Mat mapLx, mapLy, mapRx, mapRy;//映射表
	cv::Rect validROIL, validROIR; //图像裁剪之后的区域
	cv::Size imageSize; //图像尺寸
};