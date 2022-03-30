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


namespace hough{

    // 定义结构体Circle 存储中心点坐标、半径、count值
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

            // 计算两个circle圆心间距离
            int dist(const Circle & circle);
            void show(cv::Mat & image);
    };

    // 输入二值边缘图像，对应一个半径，输出一个圆
    int RmwHoughCircle(const cv::Mat & bin_img, int radius, cv::Point3i & circle, int dtheta = 1);
    
    // 主要算法：用霍夫变换检测多个圆
    // @param   bin_img: 输入图像（二值边缘图）
    // @param   radius_l: 半径的下界
    // @param   radius_u: 半径的上界
    // @param   circles: circles向量（输出参数）
    // @param   Step: 半径迭代步长
    // @param   dtheta: 角度迭代步长
    // @param   minCount: 判定为圆的最小计数值
    // @param   threshold: 对于特定半径，可能存在数个符合条件的圆，此值与最大计数的乘积为判定圆是否符合条件的阈值 
    void HoughCircles(const cv::Mat & bin_img, int radius_l, int radius_u, std::vector<Circle> & circles,
                 int Step = 2, int dtheta = 1, int minCount = 120, double threshold=0.8);

    // CountMap: 与原图像尺寸一致，灰度值表示计数值的Mat，越亮的点越可能是圆心
    // @param   bin_img: 输入图像（二值边缘图）
    // @param   cntMap: 输出的CountMap
    // @param   radius: 半径
    // @param   dtheta: 角度迭代步长
    // @param   zoom: 是否放缩CountMap的灰度值（标准化到0-255）
    Circle CountMap(const cv::Mat & bin_img, cv::Mat & cntMap, int radius, int dtheta = 1, bool zoom = false);
    
    // 用非极大值抑制去除相距太近的圆心
    // @param   range: 邻域大小
    int NMS_CountMap(std::vector<Circle> & circles, cv::Mat & cntMap, int radius, int minCount = 120, double threshold=0.8, int range=15);
    
    //  得到包含用数个半径得到的圆组成的Circles向量之后，再用极大值抑制，去除重合圆
    // @param   min_dist: 最短圆心距离
    std::vector<Circle> NMS_Circles(std::vector<Circle> & circles, int min_dist = 10);
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