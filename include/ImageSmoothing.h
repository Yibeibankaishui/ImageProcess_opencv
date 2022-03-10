#ifndef DITHERING_H
#define DITHERING_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

// 定义ImageSmoothing接口
namespace ImageSmoothing{

    // 一般方法的均值滤波器
    cv::Mat AverageFilter(const cv::Mat & input_img, int kernel_w = 3, int kernel_h = 3);
    // 基于列和的均值滤波器
    cv::Mat AverageFilter_SumCol(const cv::Mat & input_img, int kernel_w = 3, int kernel_h = 3);
    // 基于积分图的均值滤波器
    cv::Mat AverageFilter_SumImg(const cv::Mat & input_img, int kernel_w = 3, int kernel_h = 3);
}

#endif