#include "Undistort.h" 


namespace undistort{

    cv::Mat Undistort::GetUndistortImage(const cv::Mat & img_input, const std::vector<double> & parameters){
        
        // 畸变参数
        double k1 = 0.028340811, k2 = -0.0007395907, p1 = 0, p2 = 0;
        // double k1, k2, p1, p2 = 0.0;
        // k1 = 0.00002;
        // k2 = 0.0002;
        // 内参
        double fx = 100, fy = 100, cx = 120, cy = 128;

        int rows = img_input.rows;
        int cols = img_input.cols;
        std::cout << rows << "    " << cols << std::endl;
        cv::Mat image_undistort = img_input.clone();
        //  = cv::Mat(rows, cols, CV_8UC1);   // 去畸变以后的图

        // 计算去畸变后图像的内容
        for (int v = 0; v < rows; v++) {
            for (int u = 0; u < cols; u++) {

                double u_distorted = 0, v_distorted = 0;

                // 按照公式，计算点(u,v)对应到畸变图像中的坐标(u_distorted, v_distorted) (~6 lines)
                double x = (u - cx) / fx;
                double y = (v - cy) / fy;
                double r = sqrt(x * x + y * y);
                double x_distorted = x * (1 + k1 * r * r + k2 * r * r * r * r) + 2 * p1 * x * y + p2 * (r * r + 2 * x * x);
                double y_distorted = y * (1 + k1 * r * r + k2 * r * r * r * r) + p1 * (r * r + 2 * y * y) + 2 * p2 * x * y;
                u_distorted = fx * x_distorted + cx;
                v_distorted = fy * y_distorted + cy;     
                // std::cout << v << "    " <<  u << " --- "<< v_distorted << "    " << u_distorted << std::endl;
                // 赋值 (最近邻插值)
                if (u_distorted >= 0 && v_distorted >= 0 && u_distorted < cols && v_distorted < rows) {
                    image_undistort.at<uchar>(v, u) = img_input.at<uchar>((int) v_distorted, (int) u_distorted);
                } 
                else {
                    image_undistort.at<uchar>(v, u) = 0;
                }
            }
        }
        std::cout << rows << "    " << cols << std::endl;

        return image_undistort;
    }

    std::vector<double> Undistort::GetParameters(const cv::Mat & img_input){

    }

    cv::Mat Undistort::PreProcess(const cv::Mat & img_input){
        // 锐化/模糊化
        cv::Mat img_output;
        // int k = 3;
        // cv::GaussianBlur( img_input, img_output, cv::Size( k, k ), 0, 0 );

        // img_output = img_input.clone();
        // 图像裁剪
        // cv::Point2f pt();
        cv::Rect rect(4,4,252,236);
        img_output = img_input(rect);
        // 二值化
        return img_output;
        // 腐蚀操作
    }

    PointMap::PointMap(){}

    PointMap::~PointMap(){}

    //  由点来构造
    PointMap::PointMap(const std::vector<cv::Point2f> & points, cv::Mat *img, int s){
        size = s;
        image = img;
        //  深拷贝
        std::vector<cv::Point2f> points_copy(points);
        //  先确定中心点
        cv::Point2f center_img{(static_cast<float>(img->cols))/2, (static_cast<float>(img->rows))/2};
        //  会逐渐删除拷贝points中的元素
        cv::Point2f centerPoint = FindClosestPoint(center_img, points_copy, 1);
        centerNode = new Node(0, 0, centerPoint);
        //  建立NodeVec
        NodeVec.push_back(centerNode);
        for (int x = -s; x < s; x++){
            for (int y = -s; y < s; y++){
                //  建立带索引的Node
                curNode = new Node(x, y);
                //  设置相邻Node
            }
        }

        //  对points中元素迭代
        for (auto itr = points.begin(); itr != points.end(); itr++){
            //  怎么迭代？怎么构造MAP？
            //
            //
            //  找最近点
            //  计算角度，判断是否为相邻点
        }
    }

    PointMap::Node * PointMap::GetNode(int x, int y){

    }

    const cv::Point2f & PointMap::GetData(int x, int y){

    }

    double PointDistance(const cv::Point2f & pt1, const cv::Point2f & pt2){
        double distance = pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2);
        return distance;
    }

    cv::Point2f FindClosestPoint(const cv::Point2f & pt, std::vector<cv::Point2f> & points, int del){
        cv::Point2f res;
        double dis;
        double minDis = 999999;
        auto itr_del = points.begin();
        for (auto itr = points.begin(); itr != points.end(); itr++){
            dis = PointDistance(pt, *itr);
            if (dis < minDis){
                res = *itr;
                minDis = dis;
                if (del == 1){
                    itr_del = itr;
                }
            }
        }
        //  删除points中最近的点
        if (del == 1){
            points.erase(itr_del);
        }
        std::cout << res << std::endl;
        return res;
    }

    double CalculateAngle(const cv::Point2f & pt_from, const cv::Point2f & pt_to){
        double res;
        res = atan2((pt_to.y - pt_from.y), (pt_to.x - pt_from.x));
        return res;
    }

    //  判断点之间的方位
    int AssertOrient(const cv::Point2f & pt_from, const cv::Point2f & pt_to, double delta){
        double angle = CalculateAngle(pt_from, pt_to);

        if ((angle >= -delta) && (angle <= delta)) {return 1;}
        if ((angle >= PI/2-delta) && (angle >= PI/2+delta)) {return 2;}
        if (((angle >= -PI) && (angle <= -PI+delta)) || ((angle >= PI-delta) && (angle <= PI))) {return 3;}
        if ((angle >= -PI/2-delta) && (angle <= -PI/2+delta)) {return 4;}

        return 0;
    }


}
