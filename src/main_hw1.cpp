#include "Dithering.h"
#include "ColorReduction.h"
#include <string>

using namespace std;


int main(int argc, char **argv){

    // read an image
    cv::Mat input_image;
    string filename = argv[1];
    int downbits = stoi(argv[2]);
    input_image = cv::imread(filename, cv::IMREAD_COLOR);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }

    Dithering dither;

    // pipeline processing the input image
    cv::Mat output_image;
    cv::Mat output_image_2;
    // the second parameter is the ratio of color reduction
    // int down_ratio = stoi(argv[2]);
    // output_image = dither.ColorReduction(input_image, 16);
    // cout << input_image.at<cv::Vec3b>(6001,4001) << endl;
    output_image = dither.FloydSteinbergDithering(input_image, downbits);
    output_image_2 = dither.ColorReduction(input_image, downbits);
    // show result image
    cv::namedWindow("in",CV_WINDOW_AUTOSIZE);
    cv::imshow("imageIN", input_image);
    cv::namedWindow("out",CV_WINDOW_AUTOSIZE);
    cv::imshow("imageOUT", output_image);
    cv::namedWindow("out2",CV_WINDOW_AUTOSIZE);
    cv::imshow("imageOUT2", output_image_2);
    cv::imwrite("../images/catout.JPEG", output_image);
    // cv::Mat BGR[3],BGRin[3];
    // cv::split(output_image, BGR);
    // cv::split(input_image, BGRin);
    // cv::namedWindow("Bin",CV_WINDOW_AUTOSIZE);
    // cv::imshow("Bin", BGRin[0]);
    // cv::namedWindow("B",CV_WINDOW_AUTOSIZE);
    // cv::imshow("B", BGR[0]);
    // cv::namedWindow("G",CV_WINDOW_AUTOSIZE);
    // cv::imshow("G", BGR[1]);
    // cv::namedWindow("R",CV_WINDOW_AUTOSIZE);
    // cv::imshow("R", BGR[2]);
    cv::waitKey(0);


    return 0;
}