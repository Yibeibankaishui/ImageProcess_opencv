#ifndef LABEL_DETECTION_H
#define LABEL_DETECTION_H


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>


// 分别是：缺失，破损，歪斜，完好
enum label {missing, broken, skewed, intact};

namespace LabelDetector{
    label label_detect(const cv::Mat & input_image);

    void drawROIs(const cv::Mat & srcImg, cv::Mat & dstImg, std::vector<cv::Rect> ROIs);

    void drawROIs_text(const cv::Mat & srcImg, cv::Mat & dstImg, std::multimap<cv::Rect, label> ROIs_label);
}



#endif

