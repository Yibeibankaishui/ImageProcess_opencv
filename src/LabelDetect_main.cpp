#include "LabelDetection.h"
#include <chrono>


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

    cv::imshow("blur_image", blur_image);

    // binary
    cv::Mat bin_image;
    cv::threshold(input_image, bin_image, 45, 255, cv::THRESH_TOZERO);
    cv::imshow("bin_image", bin_image);

    cv::Mat hsv_image;
    cv::cvtColor(bin_image, hsv_image, cv::COLOR_RGB2HSV);

    std::vector<cv::Mat> HSV;
    cv::split(hsv_image, HSV);
    // cv::imshow("V", HSV[0]);
    cv::imshow("S", HSV[1]);
    // cv::imshow("H", HSV[2]);

    cv::waitKey(0);

    cv::Mat gray_image;
    cv::cvtColor(input_image, gray_image, cv::COLOR_BGR2GRAY);

    // 改进边缘检测:
    // 尝试在多通道上做canny
    cv::Mat edge_image;
    // S通道
    cv::Mat S_edge_image;
    cv::Mat gray_edge_image;
    cv::Canny(HSV[1], S_edge_image, 250, 360);
    cv::Canny(gray_image, gray_edge_image, 100, 230); 
    // 最后用或运算
    cv::bitwise_or(S_edge_image, gray_edge_image, edge_image);


    // cv::Mat grad_x, grad_y;
    // cv::Mat abs_grad_x, abs_grad_y;
    // cv::Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    // cv::Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    // // converting back to CV_8U
    // cv::convertScaleAbs(grad_x, abs_grad_x);
    // cv::convertScaleAbs(grad_y, abs_grad_y);
    // cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    // cv::imshow(window_name, grad);


    if (ifshow){
        // cv::imshow("enhanced_image", enhanced_image); 
        cv::imshow("gray_image", gray_image);
        // cv::imwrite("../images/gray.jpg", gray);
        // cv::imshow("blur", blur_image);
        cv::imshow("S_edge_image", S_edge_image);
        cv::imshow("gray_edge_image", gray_edge_image);
        cv::imshow("edge_image", edge_image);
        // cv::imwrite("../images/edge_image.jpg", edge_image);
        cv::waitKey(0);
    }

    return edge_image;
}


int main(int argc, char **argv){

    // read input file
    cv::Mat input_image;

    string filename = argv[1];

    // 读入图像格式？
    input_image = cv::imread(filename, 1);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }

    cv::Mat edge_image;
    edge_image = Preprocess(input_image, 1);

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
        ROIs.push_back(cv::Rect(width/5 * (i) + dx, height/2 + dy, width/5 - dx,height/2 - dy*2));
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

    // cout << edge_image << endl;

    cv::waitKey(0);

    // for (auto itr = ROIs.begin(); itr != ROIs.end();itr++){
    //     cv::imshow("roi", edge_image(*itr));
    //     cv::waitKey(0);
    // }


    return EXIT_SUCCESS;
}