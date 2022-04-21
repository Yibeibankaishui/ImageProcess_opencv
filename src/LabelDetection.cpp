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

        // check if exist
        for (int i = 60; i < 90; i++){
            cnt += cv::sum(M_ROI.row(i))[0]/255;
            // std::cout << i << "  " << cv::sum(M_ROI.row(i))[0]/255 << std::endl;
        }
        if (cnt < 80){
            std::cout << "missing" << std::endl;
            return missing;
        }

        // check skewed or broken
        // 瓶体左边缘、右边缘
        std::vector<int> bottle_left, bottle_right;
        // 标签左边缘、右边缘
        std::vector<int> sticker_left, sticker_right;
        // 瓶体与标签边缘, 左距离、右距离
        std::vector<int> dist_left, dist_right;

        bool left_found;
        bool right_found;
        int left_j = 0;
        int right_j = cols_ROI;
        for (int i = 40; i < 90; i++) {
            left_found = false;
            right_found = false;
            
            for (int j = 0; j < MAX_DIST_L; j++) {
                if (left_found == false) {
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        left_j = j;
                        bottle_left.push_back(j);
                        left_found = true;
                        std::cout << "L  " << j << " , ";
                        continue;
                    }
                }
                else {
                    std::cout << ". ";
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        sticker_left.push_back(j);
                        std::cout << j << std::endl;
                        left_found = false;
                        break;
                    }
                    if (j == MAX_DIST_L - 1) {
                        sticker_left.push_back(left_j);
                        std::cout << left_j << std::endl;
                        left_found = false;
                        break;
                    }
                }
            }
            for (int j = cols_ROI; j > cols_ROI - MAX_DIST_R; j--) {
                if (right_found == false) {
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        right_j = j;
                        bottle_right.push_back(j);
                        right_found = true;
                        std::cout << "R  " << j << " , ";
                        continue;
                    }
                }
                else {
                    // 可能的原因, 循环从ROI边界开始,找不到
                    // 应该从瓶子边界开始
                    std::cout << j << ". ";
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        sticker_right.push_back(j);
                        right_found = false;
                        std::cout << j << std::endl;
                        break;
                    }
                    if (j == cols_ROI - MAX_DIST_R + 1){
                        sticker_right.push_back(right_j);
                        right_found = false;
                        // std::cout << right_j << std::endl;
                        break;
                    }
                }
            }
        }

        // std::cout << bottle_left.size() << std::endl;
        // std::cout << bottle_right.size() << std::endl;
        // std::cout << sticker_left.size() << std::endl;
        // std::cout << sticker_right.size() << std::endl;
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