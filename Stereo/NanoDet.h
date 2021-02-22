#include <opencv2/core/core.hpp>
#include "net.h"

typedef struct HeadInfo
{
    std::string classLayer;
    std::string disLayer;
    int stride;
} HeadInfo;

typedef struct BoxInfo
{
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;
} BoxInfo;

class NanoDet
{
public:
    NanoDet(const char *param, const char *bin, bool useGPU);
    ~NanoDet();

    std::vector<BoxInfo> Detect(cv::Mat, float, float);

    ncnn::Net *Net;

    int inputSize = 320;
    bool hasGPU;
    std::vector<HeadInfo> headsInfo{
        // cls_pred|dis_pred|stride
        {"792", "795", 8},
        {"814", "817", 16},
        {"836", "839", 32},
    };

private:
    void PreProcess(cv::Mat &, ncnn::Mat &);
    void DecodeInference(ncnn::Mat &, ncnn::Mat &, int, float, std::vector<std::vector<BoxInfo>> &);
    BoxInfo disPredictionToBoundingBox(const float *&, int, float, int, int, int);
    static void nms(std::vector<BoxInfo> &, float);

    int numClass = 80;
    int regexMax = 7;
};
