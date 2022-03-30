#include "Hough.h"


using namespace std;


cv::Mat Preprocess(const cv::Mat & input_image, bool ifshow){

    cv::Mat gray;
    cv::cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat blur_image = gray.clone();
    // cv::GaussianBlur(gray, blur_image, cv::Size(3, 3), 2, 2 );

    cv::Mat edge_image;
    cv::Canny(blur_image, edge_image, 160, 320);
    if (ifshow){
        // cv::imshow("in", input_image);
        cv::imshow("gray", gray);
        cv::imshow("blur", blur_image);

    }


    return edge_image;


}

int main(int argc, char **argv){

    // read image file
    cv::Mat input_image;

    string filename = argv[1];

    input_image = cv::imread(filename, 1);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }

    // preprocess the input image
    // 1. get the gray image
    // 2. blur the image
    // 3. detect edges using Cannt and get the binary image
    cv::Mat bin_image;
    bin_image = Preprocess(input_image, 1);

    cv::imshow("bin", bin_image);



    // // pipleline of Hough Circle using OpenCV
    // cv::Mat gray;
    // cv::cvtColor(input_image, gray, cv::COLOR_BGR2GRAY);

    // // cv::Mat output_image = gray.clone();
    // cv::Mat show_image = input_image.clone();

    // // smooth it, otherwise a lot of false circles may be detected
    // cv::Mat blur_image;
    // cv::GaussianBlur(gray, blur_image, cv::Size(9, 9), 2, 2 );
    // vector<cv::Vec3f> circles;
    // cv::HoughCircles(blur_image, circles, cv::HOUGH_GRADIENT, 2, blur_image.rows/4, 200, 100 );

    // // draw circles detected
    // for( size_t i = 0; i < circles.size(); i++ )
    // {
    //      cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    //      int radius = cvRound(circles[i][2]);
    //      // draw the circle center
    //      circle( show_image, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
    //      // draw the circle outline
    //      circle( show_image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    // }

    // // show images
    // cv::namedWindow( "circles", 1 );
    // cv::imshow( "circles", show_image );
    // // write images to files

    cv::waitKey(0);



    // show image


    return 0;
}