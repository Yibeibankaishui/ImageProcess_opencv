#include "Dithering.h"
#include "ColorReduction.h"
#include <string>

using namespace std;


int main(int argc, char **argv){

    // read an image
    cv::Mat input_image;
    input_image = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (input_image.data == nullptr){
        cerr << "image file " << argv[1] << " NOT found !" << endl;
        return 0;
    }

    // pipeline processing the input image
    cv::Mat output_image;
    // the second parameter is the ratio of color reduction
    int down_ratio = stoi(argv[2]);
    output_image = ColorReduction(input_image, down_ratio);
    // output_image = input_image.clone();

    // show result image
    cv::imshow("image", output_image);
    cv::waitKey(0);


    return 0;
}