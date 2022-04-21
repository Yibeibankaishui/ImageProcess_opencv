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
        // 标签左右间距
        std::vector<int> sticker_width;

        bool left_found;
        bool right_found;
        int left_j = 0;
        int right_j = cols_ROI;
        int l_begin = 0;
        int r_begin = cols_ROI;
        for (int i = 32; i < 98; i++) {
            left_found = false;
            right_found = false;
            // l_begin = std::max(0, left_j - 5);
            // r_begin = std::min(cols_ROI, right_j + 5);
            for (int j = l_begin; j < l_begin + MAX_DIST_L; j++) {
                if (left_found == false) {
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        left_j = j;
                        l_begin = std::max(0, left_j - 5);
                        bottle_left.push_back(j);
                        left_found = true;
                        // std::cout << "L  " << j << " , ";
                        continue;
                    }
                }
                else {
                    // std::cout << ". ";
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        sticker_left.push_back(j);
                        // std::cout << j << "::" << std::endl;
                        left_found = false;
                        break;
                    }
                    if (j == l_begin + MAX_DIST_L - 1) {
                        sticker_left.push_back(left_j);
                        // std::cout << left_j << "::" << std::endl;
                        left_found = false;
                        break;
                    }
                }
            }
            for (int j = r_begin; j > r_begin - MAX_DIST_R; j--) {
                if (right_found == false) {
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        right_j = j;
                        r_begin = std::min(cols_ROI, right_j + 5);
                        bottle_right.push_back(j);
                        right_found = true;
                        // std::cout << "R  " << j << " , ";
                        continue;
                    }
                }
                else {
                    // 可能的原因, 循环从ROI边界开始,找不到
                    // 应该从瓶子边界开始
                    // std::cout << ". ";
                    if (M_ROI.at<uchar>(i, j) > 0) {
                        sticker_right.push_back(j);
                        right_found = false;
                        // std::cout << j << "::" << std::endl;
                        break;
                    }
                    if (j == r_begin - MAX_DIST_R + 1){
                        sticker_right.push_back(right_j);
                        right_found = false;
                        // std::cout << right_j << "::" << std::endl;
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < bottle_left.size(); i++) {
            dist_left.push_back(sticker_left[i] - bottle_left[i]);
            dist_right.push_back(bottle_right[i] - sticker_right[i]);
            sticker_width.push_back(sticker_right[i] - sticker_left[i]);
        }

        // for (auto itr = dist_left.begin(); itr != dist_left.end(); itr++){
        //     std::cout << *itr << ", ";
        // }
        // std::cout << std::endl << std::endl;
        // for (auto itr = dist_right.begin(); itr != dist_right.end(); itr++){
        //     std::cout << *itr << ", ";
        // }  
        // std::cout << std::endl << std::endl;
        // for (auto itr = sticker_width.begin(); itr != sticker_width.end(); itr++){
        //     std::cout << *itr << ", ";
        // }  
        // std::cout << std::endl << std::endl;      
        if (isConsistent(sticker_width, 1.0) == false) {
            std::cout << "broken" << std::endl;
            return broken;
        }
        if ( (isConsistent(dist_left, 0.7) == false) || (isConsistent(dist_right, 0.7) == false) ) {
            std::cout << "skewed" << std::endl;
            return skewed;
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


    bool isConsistent(const std::vector<int> & vec, double stdev_threshold) {
        double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
        double mean = sum / vec.size();
        double accum = 0;
        std::for_each(vec.begin(), vec.end(), [&] (const double d) {accum += (d - mean) * (d - mean);});
        double stdev = sqrt(accum / (vec.size() - 1));
        std::cout << "mean = " << mean << " stdev = " << stdev << std::endl;

        if (stdev < stdev_threshold){
            return true;
        }
        return false;
    }

}