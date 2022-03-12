#ifndef DITHERING_H
#define DITHERING_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


// 定义Dithering接口

// 下面三个算法未实现
class Dithering{
    public:
        Dithering(){}
        ~Dithering(){}
            
        cv::Mat FloydSteinbergDithering(const cv::Mat & input_img, int bits);

        cv::Mat ColorReduction(const cv::Mat & img_input, int bits);

        cv::Vec3b FindClosestColor(const cv::Vec3b & oldpixel, int bits);

        cv::Mat MinimizedAverageError(const cv::Mat & input_img, int bits);

        cv::Mat RandomDithering(const cv::Mat & input_img, int bits);

        cv::Mat OrderedDithering(const cv::Mat & input_img, int bits);

};


#endif