#include "Hough.h"


using namespace std;


cv::Mat Preprocess(const cv::Mat & input_image, bool ifshow){

    cv::Mat gray;
    cv::cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat blur_image = gray.clone();

    cv::Mat edge_image;
    cv::Canny(blur_image, edge_image, 160, 320);
    if (ifshow){

        cv::imshow("gray", gray);
        cv::imshow("blur", blur_image);
        cv::waitKey(0);

    }

    return edge_image;
}

int main(int argc, char **argv){

    // read image file
    cv::Mat input_image;

    string filename = argv[1];

    int R_min = 40;
    int R_max = 150;

    if (argc > 2){
        R_min = stoi(argv[2]);
        R_max = stoi(argv[3]);
    }

    int Step = 2;
    int dtheta = 1;

    if (argc > 4){
        Step = stoi(argv[4]);
    }
    if (argc > 5){
        dtheta = stoi(argv[5]);
    }

    input_image = cv::imread(filename, 1);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }

    vector<hough::Circle> circles;

    // preprocess the input image
    // 1. get the gray image
    // 2. blur the image
    // 3. detect edges using Cannt and get the binary image
    cv::Mat bin_image;
    bin_image = Preprocess(input_image, 0);

    // -----------------------HOUGH CIRCLE DETECTION-------------------------

    // cv::Mat cntMap;
    // hough::CountMap(bin_image, cntMap, R);

    // cv::Mat cntMap2 = cntMap.clone();
    // int circle_cnt = hough::NMS_CountMap(circles, cntMap2, R, 100, 0.75);

    hough::HoughCircles(bin_image, R_min, R_max, circles, Step, dtheta);

    vector<hough::Circle> circles_NMS = {};
    circles_NMS = hough::NMS_Circles(circles);

    cv::Mat show_image = input_image.clone();


    for (auto cir : circles_NMS){
        cir.show(show_image);
    }    


    cv::imshow("bin", bin_image);
    cv::imshow("circle", show_image);
    // cv::imshow("cntMap", cntMap);
    // cv::imshow("cntMap2", cntMap2);

    cv::waitKey(0);



    // show image


    return 0;
}