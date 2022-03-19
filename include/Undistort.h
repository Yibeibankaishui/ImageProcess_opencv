#ifndef UNDISTORT_H
#define UNDISTORT_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>


#define R 1
#define U 2
#define L 3
#define D 4

#define PI 3.141592

namespace undistort{

    class Undistort{
        public:
            Undistort() {}
            Undistort(std::vector<double> parameters): _parameters(parameters) {}
            ~Undistort() {}
            std::vector<double> _parameters;
        public:
            // 图像预处理
            cv::Mat PreProcess(const cv::Mat & img_input);
            // 从标定图像得到畸变参数
            std::vector<double> GetParameters(const cv::Mat & img_input);
            // 对输入图像做去畸变处理，输入图像和参数
            cv::Mat GetUndistortImage(const cv::Mat & img_input, const std::vector<double> & parameters);

    };


    class PointMap{
        public:
            int size;
            cv::Mat *image;
            
            struct Node
            {
                cv::Point2f data;
                //  二维索引（可以不需要？）
                int index_x;
                int index_y;

                Node *up;
                Node *down;
                Node *left;
                Node *right;

                Node(int x, int y, const cv::Point2f & pt = cv::Point2f(0,0), Node *pu = nullptr, Node *pd = nullptr, Node *pl = nullptr, Node *pr = nullptr): index_x{x}, index_y{y}, data{pt}, up{pu}, down{pd}, left{pl}, right{pr} { }

                bool empty() {return (data == cv::Point2f(0,0));}
                bool complete() {return ((up!=nullptr) && (down!=nullptr) && (left!=nullptr) && (right!=nullptr));}
            };
            //  指向中心点的指针
            Node * centerNode;
            //  用vector来存储
            std::vector<Node> NodeVec;
        public:
            PointMap();
            ~PointMap();
            PointMap(const std::vector<cv::Point2f> & points, cv::Mat *img, int s);

            //  由坐标获得Node
            Node * GetNode(int x, int y);
            //  由坐标获得点的坐标值
            const cv::Point2f & GetData(int x, int y);
    };

    double PointDistance(const cv::Point2f & pt1, const cv::Point2f & pt2);

    cv::Point2f FindClosestPoint(const cv::Point2f & pt, std::vector<cv::Point2f> & points, int del=0);

    double CalculateAngle(const cv::Point2f & pt_from, const cv::Point2f & pt_to);

    int AssertOrient(const cv::Point2f & pt_from, const cv::Point2f & pt_to, double delta=0.1745);

    //  输入points向量和一个point，找最近点且形成连接
    void FormNeighborPoints(PointMap::Node & node, std::vector<cv::Point2f> & points);

    //  删除points vector中指定元素

    void ConstructPointMap();
}


#endif

