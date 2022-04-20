#include "LabelDetection.h"



namespace LabelDetector{

    label label_detect(const cv::Mat & input_image){

        return intact;
    }


    void drawROIs(const cv::Mat & srcImg, cv::Mat & dstImg, std::vector<cv::Rect> ROIs){
        dstImg = srcImg.clone();
        for (auto itr = ROIs.begin(); itr != ROIs.end();){
            cv::rectangle(dstImg, *itr, cv::Scalar(0, 0, 255), 1);
            itr++;
        }
    }


    void drawROIs_text(const cv::Mat & srcImg, cv::Mat & dstImg, std::multimap<cv::Rect, label> ROIs_label){

    }

}