#include "Hough.h"


using namespace std;


int main(int argc, char **argv){

    // read image file
    cv::Mat input_image;

    string filename = argv[1];

    input_image = cv::imread(filename, 0);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }

    cv::Mat output_image;

    cv::imshow("input", input_image);

    
    cv::waitKey(0);



    // show image


    return 0;
}