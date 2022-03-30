#ifndef HOUGH_H
#define HOUGH_H


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>


// enum HoughMethod{

// }

namespace hough{

    void HoughCircles(const cv::Mat & bin_img, std::vector<cv::Vec3f> & circles);

}


namespace RMWhough{
    // void RmwDrawCircle(BYTE *pGryImg, int width, int height, int x0, int y0, int r, int color);
    // 滤波
    // 边缘检测
    // 二值化
    class Circle{
        public:
            int xCenter;
            int yCenter;
            int radius;
            int count;

        public:
            Circle() {}
            Circle(int xc, int yc, int r, int cnt) : xCenter(xc), yCenter(yc), radius(r), count(cnt) { }
            ~Circle() {}
            void show(cv::Mat & image);
    };

    void HoughCircles(const cv::Mat & bin_img, int radius_l, int radius_u, std::vector<Circle> circles, int dtheta = 1);
    // 输入二值边缘图像
    int RmwHoughCircle(const cv::Mat & bin_img, int radius, cv::Point3i & circle, int dtheta = 1);
    // count图像
    Circle CountMap(const cv::Mat & bin_img, cv::Mat & cntMap, int radius, int dtheta = 1);
}

namespace hough_RANSAC{

    struct Circle{
        double radius;
        int xCenter;
        int yCenter;
        int maxVote;
    };

    void computeHoughVote(cv::Mat &Vote, cv::Mat img, double radius, std::map<int,std::pair<double,double>> thetaMap, int &maxVote);

    void findHoughPeak(cv::Mat &Vote, int maxVote, int numberPeaks, std::vector<Circle> &peakCenters, double radius);

    bool checkCirclePresent(std::vector<Circle> &bestCircles, Circle newCircle, int pixelInterval);

    void HoughTransform(cv::Mat imgEdges, std::vector<Circle> &bestCircles, int radiusStart, int radiusEnd);

    void circleRANSAC(std::vector<cv::Point> edgePoints, std::vector<Circle> &bestCircles, int iterations);

    cv::Mat drawCircles(cv::Mat img, std::vector<Circle> bestCircles, const int limit);

}



#endif