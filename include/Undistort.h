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
#include <algorithm>
#include <ceres/ceres.h>
#include <Eigen/Core>
#include <Eigen/Dense>


#define R 1
#define U 2
#define L 3
#define D 4

#define PI 3.141592

//  中心点坐标，内参数的一部分
#define CX 126.0
#define CY 118.0

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
            // // 从标定图像得到畸变参数
            // std::vector<double> GetParameters(const cv::Mat & img_input);
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

                Node(int x=0, int y=0, const cv::Point2f & pt = cv::Point2f(0,0), Node *pu = nullptr, Node *pd = nullptr, Node *pl = nullptr, Node *pr = nullptr): index_x{x}, index_y{y}, data{pt}, up{pu}, down{pd}, left{pl}, right{pr} { }

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
            Node * GetNode(int x, int y, cv::Point2f pt = cv::Point2f(0,0));
            //  由坐标获得点的坐标值
            cv::Point2f GetData(int x, int y);
            
            //  确定NodeVec中有没有
            bool NodeInVec(const Node & node);
            //  输入points向量和一个point，找最近点且形成连接
            void FormNeighborPoints(PointMap::Node & node, std::vector<cv::Point2f> & points);
    };


    //构建代价函数结构体，residual为残差。
    struct DISTORT_COST
    {
        DISTORT_COST(double ud, double vd, double ur, double vr, double r2):_ud(ud), _vd(vd), _ur(vr), _vr(vr),_r2(r2) {}
        template <typename T>
        bool operator()(const T* const K, T* residual)const
        {
            residual[0] =  (ceres::pow(((_ur-CX)*(K[0]*_r2+K[1]*_r2*_r2+K[2]*_r2*_r2*_r2+1.0) + CX - _ud),2) 
                            // * ((_ur-CX)*(K[0]*_r2+K[1]*_r2*_r2+K[2]*_r2*_r2*_r2+1.0) + CX - _ud) 
                            + ceres::pow(((_vr-CY)*(K[0]*_r2+K[1]*_r2*_r2+K[2]*_r2*_r2*_r2+1.0) + CY - _vd), 2));
            return true;
        }
        //  data: distorted (u,v)   undistorted (u,v)
        const double _ud;
        const double _vd;
        const double _ur;
        const double _vr;

        const double _r2;
    };

    //  用ceres求解优化问题
    void SolveParameters_ceres(std::vector<double> & params, PointMap & pm_real, PointMap & pm_distort);

    void SolveParameters_GN(std::vector<double> & params, PointMap & pm_real, PointMap & pm_distort);


    double PointDistance(const cv::Point2f & pt1, const cv::Point2f & pt2);

    cv::Point2f FindClosestPoint(const cv::Point2f & pt, std::vector<cv::Point2f> & points, int del=0);

    double CalculateAngle(const cv::Point2f & pt_from, const cv::Point2f & pt_to);

    //  delta值为最小角度差
    int AssertOrient(const cv::Point2f & pt_from, const cv::Point2f & pt_to, double delta=0.42);

    void GenerateRealPoints(std::vector<cv::Point2f> & real_points, cv::Mat * img, int num_x=7, int num_y=4);

    //  对单个点应用畸变
    cv::Point2f DistortPoint(const cv::Point2f & point);

    //  获得真实点畸变之后的坐标
    void DistortPoints(PointMap & pm_distort, const std::vector<double> params);

    //  计算重投影误差
    float CalculateError(PointMap & pm_real, PointMap & pm_predict);

    //  删除points vector中指定元素
    int DeletePoint(std::vector<cv::Point2f> & points, cv::Point2f pt);



}


#endif

