#include "LabelDetection.h"



namespace LabelDetector{

    std::vector<label> label_detect(const cv::Mat & input_image, const std::vector<cv::Rect> & ROIs){

        std::vector<label> lb_vec;
        for (auto itr = ROIs.begin(); itr != ROIs.end();){
            label lb = label_detect(input_image, *itr);
            lb_vec.push_back(lb);
            itr++;
        }

        return lb_vec;
    }


    label label_detect(const cv::Mat & input_image, const cv::Rect & ROI){

        cv::Mat M_ROI = input_image(ROI);
        int rows_ROI = M_ROI.rows;
        int cols_ROI = M_ROI.cols;

        int cnt = 0;

        for (int i = 60; i < 90; i++){
            cnt += cv::sum(M_ROI.row(i))[0]/255;
            // std::cout << i << "  " << cv::sum(M_ROI.row(i))[0]/255 << std::endl;
        }
        if (cnt < 80){
            std::cout << "missing" << std::endl;
            return missing;
        }
        std::cout << "intact" << std::endl;
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