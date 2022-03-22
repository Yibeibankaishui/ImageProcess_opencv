#include "Undistort.h"


using namespace std;


//  检测图像中的黑色斑点（特征点）
void DetectKeyPoint(const cv::Mat & image, std::vector<cv::KeyPoint> & keypoints, std::vector<cv::Point2f> & points){
    // Detect blobs.
    cv::SimpleBlobDetector::Params params_b;
    // Change thresholds
    params_b.minThreshold = 0;
    params_b.maxThreshold = 210;

    // Filter by Area.
    params_b.filterByArea = true;
    params_b.minArea = 10;

    // Filter by Circularity
    params_b.filterByCircularity = true;
    params_b.minCircularity = 0.7;

    // Filter by Convexity
    params_b.filterByConvexity = true;
    params_b.minConvexity = 0.5;

    // Filter by Inertia
    params_b.filterByInertia = true;
    params_b.minInertiaRatio = 0.3;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params_b); 
    
    detector->detect( image, keypoints );
    cv::KeyPoint::convert(keypoints, points);
    
    //  筛选，去除异常点
    for (auto itr = points.begin(); itr != points.end(); ){
        if ((itr -> x > 228) || (itr -> x < 25)){
            itr = points.erase(itr);
        }
        else
            itr ++;
    }
}


int main(int argc, char **argv){

    // read an image
    cv::Mat input_image;
    // process input parameters
    string filename = argv[1];

    input_image = cv::imread(filename, 0);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }
    undistort::Undistort undist;
    float k1, k2, k3, cx, cy, alpha, beta;
    cx = 126;
    cy = 118;
    alpha = 1;
    beta = 1;
    //  畸变参数: k1, k2, k3, cx, cy, alpha, beta
    vector<double> params;
    params.push_back(k1);
    params.push_back(k2);
    params.push_back(k3);
    params.push_back(cx);
    params.push_back(cy);
    params.push_back(alpha);
    params.push_back(beta);

    params.push_back(1.0);
    // pipeline
    cv::Mat output_image = undist.PreProcess(input_image);

    std::vector<cv::KeyPoint> keypoints;
    std::vector<cv::Point2f> points;
    DetectKeyPoint(output_image, keypoints, points);
    
    //  Visualization of blobs
    // Draw detected blobs as red circles.
    cv::Mat im_with_keypoints = output_image.clone();
    cv::drawKeypoints( output_image, keypoints, im_with_keypoints, cv::Scalar(0,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    for (auto pi: points){
        cv::circle( im_with_keypoints, pi, 1, cv::Scalar(0,0,255), -1);
    }

    // output_image = undist.GetUndistortImage(input_image, params);



    cv::Mat * out = &output_image;
    // 构造PointMap
    undistort::PointMap pm(points, out, 2);
    cout << "center:   " << pm.centerNode->data << endl;
    // vector<cv::Point2f> new_points(points);
    // pm.FormNeighborPoints(*(pm.centerNode), new_points);
    
    //  生成真实点
    vector<cv::Point2f> real_points;
    undistort::GenerateRealPoints(real_points, out, 6, 4);
    for (auto Ppt: real_points){
        // cout << Ppt << endl;
        cv::circle( im_with_keypoints, Ppt, 1, cv::Scalar(0,255,0), -1);
    }
    undistort::PointMap pm_real(real_points, out, 2);

    // show images
    cv::imshow("input", input_image);
    cv::imshow("output", output_image);
    imshow("keypoints", im_with_keypoints );
    cv::imwrite("../images/output_distort.png", output_image);
    cv::imwrite("../images/keypoints.png", im_with_keypoints);
    
    cv::waitKey(0);

    return 0;
}



// 图像去畸变