#include "ColorReduction.h"


cv::Mat ColorReduction(const cv::Mat & img_input, int ratio){
    int width = img_input.cols;
    int height = img_input.rows;

    cv::Mat img_output = cv::Mat::zeros(height, width, CV_8UC3);

    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            for (int c = 0; c < 3; c++){
            img_output.at<cv::Vec3b>(j,i)[c] = (uchar)(floor((double)img_input.at<cv::Vec3b>(j,i)[c] / ratio) * ratio + (ratio / 2));
            }
        }
    }

    return img_output;
}