#include "ImageSmoothing.h"


using namespace std;

namespace ImageSmoothing{

        // 一般方法的均值滤波器
    cv::Mat AverageFilter(const cv::Mat & input_img, int kernel_w, int kernel_h){

    }


    // 基于列和的均值滤波器
    cv::Mat AverageFilter_SumCol(const cv::Mat & input_img, int kernel_w, int kernel_h){

    }


    // 基于积分图的均值滤波器
    cv::Mat AverageFilter_SumImg(const cv::Mat & input_img, int kernel_w, int kernel_h){

    }


    // 计算积分图
    cv::Mat CalculateSumImg(const cv::Mat & input_img){

    }
}