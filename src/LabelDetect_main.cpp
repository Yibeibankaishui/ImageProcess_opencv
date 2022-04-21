#include "LabelDetection.h"
#include <chrono>
#include <string>


using namespace std;    


cv::Mat Preprocess(const cv::Mat & input_image, bool ifshow){

    // 直方图均衡化
    // cv::Mat enhanced_image;

	// cv::Mat imageRGB[3];
	// cv::split(input_image, imageRGB);
	// for (int i = 0; i < 3; i++)
	// {
	// 	cv::equalizeHist(imageRGB[i], imageRGB[i]);
	// }
	// cv::merge(imageRGB, 3, enhanced_image);

    // gaussian
    cv::Mat blur_image;
    cv::GaussianBlur(input_image, blur_image, cv::Size(7,7), 0, 0);

    // cv::imshow("blur_image", blur_image);

    // binary
    // cv::Mat bin_image;
    // cv::threshold(input_image, bin_image, 45, 255, cv::THRESH_TOZERO);
    // cv::imshow("bin_image", bin_image);


    cv::Mat hsv_image;
    cv::cvtColor(blur_image, hsv_image, cv::COLOR_RGB2HSV);

    std::vector<cv::Mat> HSV;
    cv::split(hsv_image, HSV);
    // cv::imshow("V", HSV[0]);
    // cv::imshow("S", HSV[1]);
    // cv::imshow("H", HSV[2]);
    // cout << (HSV[2]) << endl;

    // binary in HSV
    cv::Mat HSV_bin_image;
    cv::Mat hsv_bin[3], hsv_binL[3], hsv_binU[3];    
    // cv::threshold(HSV[0], hsv_binL[0], 5, 255, cv::THRESH_BINARY);
    // cv::threshold(HSV[0], hsv_binU[0], 74, 255, cv::THRESH_BINARY_INV);
    // cv::bitwise_and(hsv_binL[0], hsv_binU[0], hsv_bin[0]);
    // cv::imshow("V", hsv_bin[0]);
    cv::threshold(HSV[1], hsv_bin[1], 159, 255, cv::THRESH_BINARY_INV);
    // cv::imshow("S", hsv_bin[1]);
    // 注意H分量 0-180
    // cv::threshold(HSV[2], hsv_bin[2], 250, 255, cv::THRESH_BINARY_INV);
    // cv::imshow("H", hsv_bin[2]);
    // cv::merge(hsv_bin, 3, HSV_bin_image);
    // cv::imshow("HSV_bin", HSV_bin_image);

    cv::Mat gray_image;
    cv::cvtColor(input_image, gray_image, cv::COLOR_BGR2GRAY);

    // 改进边缘检测:
    // 尝试在多通道上做canny
    cv::Mat edge_image;
    // S通道
    cv::Mat S_edge_image;
    cv::Mat gray_edge_image;
    cv::Canny(hsv_bin[1], S_edge_image, 250, 360);
    cv::Canny(gray_image, gray_edge_image, 140, 230); 
    // 最后用或运算
    cv::bitwise_or(S_edge_image, gray_edge_image, edge_image);

    if (ifshow){
        // cv::imshow("enhanced_image", enhanced_image); 
        // cv::imshow("gray_image", gray_image);
        // cv::imwrite("../images/gray.jpg", gray);
        // cv::imshow("blur", blur_image);
        cv::imshow("S_edge_image", S_edge_image);
        cv::imshow("gray_edge_image", gray_edge_image);
        cv::imshow("edge_image", edge_image);
        // cv::imwrite(filename_write, edge_image);
        // cv::waitKey(0);
    }

    return edge_image;
}


int main(int argc, char **argv){

    // read input file
    cv::Mat input_image;

    string dir = "../images/";
    string postfix = ".jpg";
    string filename = dir + argv[1] + postfix;

    // 读入图像格式？
    input_image = cv::imread(filename, 1);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }

    cv::Mat edge_image;
    edge_image = Preprocess(input_image, 0);

    int height = edge_image.rows;
    int width = edge_image.cols;

    cout << "[ " << height << " , " << width << " ]" << endl;

    // time counter
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    // show the rois (total 5)
    vector<cv::Rect> ROIs;
    int dx{3};
    int dy{3};
    for (int i = 0; i < 5; i++){
        ROIs.push_back(cv::Rect(width/5 * (i) + dx, height/2 + dy, width/5 - dx,height/2 - dy*3));
    }

    LabelDetector::label_detect(edge_image, ROIs);

    cv::Mat output_image;
    LabelDetector::drawROIs(input_image, output_image, ROIs);

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "time used:   " << time_used.count() << "  seconds. " << endl;

    // hough line BAD
    // vector<cv::Vec4i> lines;
    // cv::HoughLinesP(edge_image, lines, 1, CV_PI/45, 50, 50, 10);
    // for ( int i = 0; i < lines.size(); i++ ){
    //     cv::Vec4i l = lines[i];
    //     cv::line(output_image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
    // }

    // SHOW IMAGES
    cv::imshow("input", input_image);
    cv::imshow("output", output_image);
    cv::imshow("edge", edge_image);
    cv::imwrite(dir + argv[1] + "_edge" + postfix, edge_image);

    // cout << edge_image << endl;

    cv::waitKey(0);

    // for (auto itr = ROIs.begin(); itr != ROIs.end();itr++){
    //     cv::imshow("roi", edge_image(*itr));
    //     cv::waitKey(0);
    // }


    return EXIT_SUCCESS;
}