#include "NanoDet.h"

inline float fastExp(float x)
{
    union
    {
        uint32_t i;
        float f;
    } v{};
    v.i = (1 << 23) * (1.4426950409 * x + 126.93490512f);
    return v.f;
}

inline float sigmoid(float x)
{
    return 1.0f / (1.0f + fastExp(-x));
}

template <typename _Tp>
int activationFunctionSoftmax(const _Tp *src, _Tp *dst, int length)
{
    const _Tp alpha = *std::max_element(src, src + length);
    _Tp denominator{0};

    for (int i = 0; i < length; ++i)
    {
        dst[i] = fastExp(src[i] - alpha);
        denominator += dst[i];
    }

    for (int i = 0; i < length; ++i)
    {
        dst[i] /= denominator;
    }

    return 0;
}

NanoDet::NanoDet(const char *param, const char *bin, bool useGPU)
{
    this->Net = new ncnn::Net();
    this->hasGPU = ncnn::get_gpu_count() > 0;
    this->Net->opt.use_vulkan_compute = true;
    this->Net->set_vulkan_device(0);
    this->Net->opt.use_fp16_arithmetic = true;
    this->Net->load_param(param);
    this->Net->load_model(bin);
}

NanoDet::~NanoDet()
{
    delete this->Net;
}

void NanoDet::PreProcess(cv::Mat &image, ncnn::Mat &in)
{
    in = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR, image.cols, image.rows);

    const float meanValues[3] = {103.53f, 116.28f, 123.675f};
    const float normValues[3] = {0.017429f, 0.017507f, 0.017125f};
    in.substract_mean_normalize(meanValues, normValues);
}

std::vector<BoxInfo> NanoDet::Detect(cv::Mat image, float scoreThreshold, float nmsThreshold)
{
    ncnn::Mat input;
    PreProcess(image, input);

    auto ex = this->Net->create_extractor();
    ex.set_light_mode(false);
    ex.set_num_threads(4);
    ex.set_vulkan_compute(true);
    ex.input("input.1", input);

    std::vector<std::vector<BoxInfo>> results;
    results.resize(this->numClass);

    for (const auto &headInfo : this->headsInfo)
    {
        ncnn::Mat disPrediction;
        ncnn::Mat classPrediction;
        ex.extract(headInfo.disLayer.c_str(), disPrediction);
        ex.extract(headInfo.classLayer.c_str(), classPrediction);

        this->DecodeInference(classPrediction, disPrediction, headInfo.stride, scoreThreshold, results);
    }

    std::vector<BoxInfo> dets;
    for (int i = 0; i < (int)results.size(); i++)
    {
        this->nms(results[i], nmsThreshold);

        for (auto box : results[i])
        {
            dets.push_back(box);
        }
    }

    return dets;
}

void NanoDet::DecodeInference(ncnn::Mat &classPrediction, ncnn::Mat &disPrediction, int stride, float threshold, std::vector<std::vector<BoxInfo>> &results)
{
    int feature_h = this->inputSize / stride;
    int feature_w = this->inputSize / stride;

    for (int index = 0; index < feature_h * feature_w; index++)
    {
        const float *scores = classPrediction.row(index);
        int row = index / feature_w;
        int col = index % feature_w;
        float score = 0;
        int currentLabel = 0;
        for (int label = 0; label < this->numClass; label++)
        {
            if (scores[label] > score)
            {
                score = scores[label];
                currentLabel = label;
            }
        }
        if (score > threshold)
        {
            const float *boundingBoxPrediction = disPrediction.row(index);
            results[currentLabel].push_back(this->disPredictionToBoundingBox(boundingBoxPrediction, currentLabel, score, col, row, stride));
        }
    }
}

BoxInfo NanoDet::disPredictionToBoundingBox(const float *&dfl_det, int label, float score, int x, int y, int stride)
{
    float ct_x = (x + 0.5) * stride;
    float ct_y = (y + 0.5) * stride;
    std::vector<float> disPrediction;
    disPrediction.resize(4);
    for (int i = 0; i < 4; i++)
    {
        float dis = 0;
        float *dis_after_sm = new float[this->regexMax + 1];
        activationFunctionSoftmax(dfl_det + i * (this->regexMax + 1), dis_after_sm, this->regexMax + 1);
        for (int j = 0; j < this->regexMax + 1; j++)
        {
            dis += j * dis_after_sm[j];
        }
        dis *= stride;
        //std::cout << "dis:" << dis << std::endl;
        disPrediction[i] = dis;
        delete[] dis_after_sm;
    }
    float xmin = (std::max)(ct_x - disPrediction[0], .0f);
    float ymin = (std::max)(ct_y - disPrediction[1], .0f);
    float xmax = (std::min)(ct_x + disPrediction[2], (float)this->inputSize);
    float ymax = (std::min)(ct_y + disPrediction[3], (float)this->inputSize);

    return BoxInfo{xmin, ymin, xmax, ymax, score, label};
}

void NanoDet::nms(std::vector<BoxInfo> &inputBoxes, float NMS_THRESH)
{
    std::sort(inputBoxes.begin(), inputBoxes.end(), [](BoxInfo a, BoxInfo b) { return a.score > b.score; });
    std::vector<float> vArea(inputBoxes.size());
    for (int i = 0; i < int(inputBoxes.size()); ++i)
    {
        vArea[i] = (inputBoxes.at(i).x2 - inputBoxes.at(i).x1 + 1) * (inputBoxes.at(i).y2 - inputBoxes.at(i).y1 + 1);
    }
    for (int i = 0; i < int(inputBoxes.size()); ++i)
    {
        for (int j = i + 1; j < int(inputBoxes.size());)
        {
            float xx1 = (std::max)(inputBoxes[i].x1, inputBoxes[j].x1);
            float yy1 = (std::max)(inputBoxes[i].y1, inputBoxes[j].y1);
            float xx2 = (std::min)(inputBoxes[i].x2, inputBoxes[j].x2);
            float yy2 = (std::min)(inputBoxes[i].y2, inputBoxes[j].y2);
            float w = (std::max)(float(0), xx2 - xx1 + 1);
            float h = (std::max)(float(0), yy2 - yy1 + 1);
            float inter = w * h;
            float ovr = inter / (vArea[i] + vArea[j] - inter);
            if (ovr >= NMS_THRESH)
            {
                inputBoxes.erase(inputBoxes.begin() + j);
                vArea.erase(vArea.begin() + j);
            }
            else
            {
                j++;
            }
        }
    }
}
