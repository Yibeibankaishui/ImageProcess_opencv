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
        cv::Mat img_light;
        cv::Mat img_corp;
        cv::Rect rect(4,4,252,236);
        img_corp = img_input(rect);
        int k = 81;
        cv::GaussianBlur( img_corp, img_light, cv::Size( k, k ), 0, 0 );
        cv::imshow( "blur", img_light);
        img_output = 255-(img_light - img_corp);
        // cv::imshow("out", img_output);
        // cv::waitKey(0);
        return img_output;
    }

    PointMap::PointMap(){}

    PointMap::~PointMap(){}

    //  由点来构造
    PointMap::PointMap(const std::vector<cv::Point2f> & points, cv::Mat *img, int s){
        size = s;
        image = img;
        std::vector<PointMap::Node> NodeSearch;
        //  深拷贝
        std::vector<cv::Point2f> points_copy(points);
        //  先确定中心点
        cv::Point2f center_img{(static_cast<float>(img->cols))/2, (static_cast<float>(img->rows))/2};
        //  会逐渐删除拷贝points中的元素
        cv::Point2f centerPoint = FindClosestPoint(center_img, points_copy, 1);
        centerNode = new Node(0, 0, centerPoint);
        //  建立NodeVec
        NodeVec.push_back(*centerNode);

        std::vector<cv::Point2f> new_points(points);
        PointMap::FormNeighborPoints(*(this->centerNode), new_points);

    }

    //  由索引坐标获得node, 没有就创建
    PointMap::Node * PointMap::GetNode(int x, int y, cv::Point2f pt){
        for (auto itr = NodeVec.begin(); itr != NodeVec.end(); itr++){
            if ((itr->index_x == x) && (itr->index_y == y)){
                return &(*itr);
            }
        }
        PointMap::Node * node_new = new PointMap::Node(x, y, pt);
        NodeVec.push_back(*node_new);
        return node_new;
    }

    //  由索引坐标获得node的图像坐标值
    cv::Point2f PointMap::GetData(int x, int y){
        PointMap::Node * n = PointMap::GetNode(x, y);
        return n->data;
    }

    double PointDistance(const cv::Point2f & pt1, const cv::Point2f & pt2){
        double distance = pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2);
        return distance;
    }

    // find the closest point
    cv::Point2f FindClosestPoint(const cv::Point2f & pt, std::vector<cv::Point2f> & points, int del){
        
        cv::Point2f res;
        double dis;
        double minDis = 1700;
        
        auto itr_del = points.begin();
        for (auto itr = points.begin(); itr != points.end(); itr++){
            dis = PointDistance(pt, *itr);
            if (dis < minDis) {
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
        //  如果未找到，会返回[0, 0]
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

        if ((angle >= -delta) && (angle <= delta)) {return R;}
        if ((angle >= PI/2-delta) && (angle <= PI/2+delta)) {return D;}
        if (((angle >= -PI) && (angle <= -PI+delta)) || ((angle >= PI-delta) && (angle <= PI))) {return L;}
        if ((angle >= -PI/2-delta) && (angle <= -PI/2+delta)) {return U;}

        return 0;
    }

    //  输入points向量和一个point，找最近点且形成连接
    void PointMap::FormNeighborPoints(PointMap::Node & node, std::vector<cv::Point2f> & points){
        //  先删除当前点
        DeletePoint(points, node.data);
        //  拷贝points作为搜索vector
        std::vector<cv::Point2f> points_search(points);

        //  记录下当前点的索引坐标（不是在图像中的坐标）
        int id_x = node.index_x;
        int id_y = node.index_y;
        
        for (int cnt = 0; cnt < 4; cnt++) {

            //  找四个最邻点
            if (points_search.empty()) {break;}
            if (node.complete()) {break;}

            cv::Point2f p_neighbor = FindClosestPoint(node.data, points_search);
            if (p_neighbor == cv::Point2f(0,0)) {break;}
            int ori = AssertOrient(node.data, p_neighbor);
            if ((ori == R) && (node.right == nullptr)) {
                node.right = PointMap::GetNode(id_x+1,id_y,p_neighbor);
                node.right->left = &node;
                FormNeighborPoints(*(node.right), points);
                }
            else if((ori == U) && (node.up == nullptr)) {
                node.up = PointMap::GetNode(id_x,id_y+1,p_neighbor);
                node.up->down = &node;
                FormNeighborPoints(*(node.up), points);
                }
            else if((ori == L) && (node.left == nullptr)) {
                node.left = PointMap::GetNode(id_x-1,id_y,p_neighbor);
                node.left->right = &node;
                FormNeighborPoints(*(node.left), points);
                }
            else if((ori == D) && (node.down == nullptr)) {
                node.down = PointMap::GetNode(id_x,id_y-1,p_neighbor);
                node.down->up = &node;
                FormNeighborPoints(*(node.down), points);
                }

            //  最近点不在四个方位上，直接去除
            DeletePoint(points_search, p_neighbor);
            
        }
        
        return;
        
    }

    bool PointMap::NodeInVec(const PointMap::Node & node){
        for (auto itr = NodeVec.begin(); itr != NodeVec.end(); itr++){
            if (itr->data == node.data) {
            // if ((itr->data == node.data) || ((itr->index_x == node.index_x) && (itr->index_y == node.index_y))){
                //  NodeVec中已经包含这个node
                return true;
            }
        }
        return false;
    }

    //  删除points vector中指定元素
    int DeletePoint(std::vector<cv::Point2f> & points, cv::Point2f pt){
        for (auto itr = points.begin(); itr != points.end(); itr++){
            if(*itr == pt){
                points.erase(itr);
                return 1;
            }
        }
        return 0;
    }

}
