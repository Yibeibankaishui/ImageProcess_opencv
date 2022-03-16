#ifndef UNDISTORT_H
#define UNDISTORT_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

cv::Mat GetUndistortImage(const cv::Mat & img_input);

#endif

