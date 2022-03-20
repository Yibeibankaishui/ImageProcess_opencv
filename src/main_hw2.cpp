#include "Undistort.h"


using namespace std;

int main(int argc, char **argv){

    // read an image
    cv::Mat input_image;
    // process input parameters
    string filename = argv[1];

    input_image = cv::imread(filename, cv::IMREAD_COLOR);

    if (input_image.data == nullptr){
        cerr << "image file " << filename << " NOT found !" << endl;
        return 0;
    }
    undistort::Undistort undist;

    vector<double> params;
    params.push_back(1.0);
    // pipeline
    cv::Mat output_image = undist.PreProcess(input_image);
    

    // cv::SimpleBlobDetector detector;

    // Detect blobs.
    cv::SimpleBlobDetector::Params params_b;
    // Change thresholds
    params_b.minThreshold = 0;
    params_b.maxThreshold = 215;

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
    
    std::vector<cv::KeyPoint> keypoints;
    // detector.detect( output_image, keypoints);
    detector->detect( output_image, keypoints );
    std::vector<cv::Point2f> points;
    cv::KeyPoint::convert(keypoints, points);
    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    cv::Mat im_with_keypoints = output_image.clone();
    cv::drawKeypoints( output_image, keypoints, im_with_keypoints, cv::Scalar(0,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    for (auto kp: keypoints){
        cout << kp.pt << endl;
    }
    for (auto pi: points){
        cv::circle( im_with_keypoints, pi, 1, cv::Scalar(0,0,255), -1);
    }
    // output_image = undist.GetUndistortImage(input_image, params);

    // Show blobs
    imshow("keypoints", im_with_keypoints );
    cv::imwrite("../images/keypoints.png", im_with_keypoints);
    // show images
    cv::imshow("input", input_image);
    cv::imshow("output", output_image);
    cv::imwrite("../images/output_distort.png", output_image);
    
    cv::waitKey(0);

    cv::Point2f Pt;
    cv::Point2f center{126,118};
    Pt = undistort::FindClosestPoint(center, points, 0);
    // Pt = undistort::FindClosestPoint(center,points, 1);
    // Pt = undistort::FindClosestPoint(center,points, 1);
    // Pt = undistort::FindClosestPoint(center,points, 1);
    // cout << Pt << endl;
    cv::Mat * in = &input_image;
    // 构造PointMap
    undistort::PointMap pm(points, in, 2);
    cout << "center:   " << pm.centerNode->data << endl;
    vector<cv::Point2f> new_points(points);
    pm.FormNeighborPoints(*(pm.centerNode), new_points);
    cout << points.size() << endl;
    cout << pm.NodeVec.size() << endl;
    cout << pm.centerNode->left->left->up->data << endl;

    return 0;
}



// 图像去畸变