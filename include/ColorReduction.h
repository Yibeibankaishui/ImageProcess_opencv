#ifndef COLOR_REDUCTION_H
#define COLOR_REDUCTION_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <iostream>

cv::Mat ColorReduction(const cv::Mat & img_input, int ratio=2);


#endif