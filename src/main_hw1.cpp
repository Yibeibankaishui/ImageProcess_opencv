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
    // the second parameter is the ratio of color reduction
    // int down_ratio = stoi(argv[2]);
    // output_image = dither.ColorReduction(input_image, 16);
    // cout << input_image.at<cv::Vec3b>(6001,4001) << endl;
    output_image = dither.FloydSteinbergDithering(input_image, 4);
    // show result image
    cout << output_image.rows << endl;
    cv::imshow("image", output_image);
    cv::waitKey(0);


    return 0;
}