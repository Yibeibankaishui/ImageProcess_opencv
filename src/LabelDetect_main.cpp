#include "LabelDetection.h"


using namespace std;    


cv::Mat Preprocess(const cv::Mat & input_image, bool ifshow){

    cv::Mat gray;
    cv::cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat blur_image = gray.clone();

    cv::Mat edge_image;
    cv::Canny(blur_image, edge_image, 100, 230); 


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

        cv::imshow("gray_image", gray);
        cv::imwrite("../images/gray.jpg", gray);
        // cv::imshow("blur", blur_image);
        cv::imshow("edge_image", edge_image);
        cv::imwrite("../images/edge_image.jpg", edge_image);
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




    // SHOW IMAGES
    cv::imshow("input", input_image);

    cv::waitKey(0);

    return EXIT_SUCCESS;
}