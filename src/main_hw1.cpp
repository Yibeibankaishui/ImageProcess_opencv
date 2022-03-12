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
    cv::Mat output_image_no_dither;
    output_image = dither.FloydSteinbergDithering(input_image, downbits);
    output_image_no_dither = dither.ColorReduction(input_image, downbits);
    // show result image
    cv::imshow("imageIN", input_image);
    cv::imshow("imageOUT", output_image);
    cv::imshow("image_no_dither", output_image_no_dither);

    // 图像写入文件
    cv::imwrite("../images/in.PNG", input_image);
    cv::imwrite("../images/out_no_dither.PNG", output_image_no_dither);
    cv::imwrite("../images/out.PNG", output_image);

    cv::waitKey(0);
    // 显示BGR各通道灰度图像
    cv::Mat BGR[3];
    cv::Mat BGRin[3];
    cv::split(output_image, BGR);
    cv::split(input_image, BGRin);
    cv::imshow("Bin", BGRin[0]);
    cv::imshow("B", BGR[0]);
    cv::imshow("G", BGR[1]);
    cv::imshow("R", BGR[2]);
    cv::imwrite("../images/inB.PNG", BGRin[0]);
    cv::imwrite("../images/B.PNG", BGR[0]);
    cv::imwrite("../images/G.PNG", BGR[1]);
    cv::imwrite("../images/R.PNG", BGR[2]);
    
    cv::waitKey(0);

    return 0;
}