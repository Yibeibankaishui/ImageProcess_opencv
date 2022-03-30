#include "Hough.h"

using namespace std;    
using namespace cv;

const double PI = 3.1415926;

namespace hough{
    
    void HoughCircles(const cv::Mat & bin_img, std::vector<cv::Vec3f> & circles){}

}

namespace RMWhough{

    void Circle::show(cv::Mat & image){}

    Circle CountMap(const cv::Mat & bin_img, cv::Mat & cntMap, int radius, int dtheta, bool zoom){

        const int width = bin_img.cols;
        const int height = bin_img.rows;

        cntMap = cv::Mat::zeros(height, width, CV_8UC1);

        int X0_pre, Y0_pre;
        int cost, sint;
        double maxValue = 0;
        cv::Point2i maxPt;

        for (int theta = 0; theta < 360; theta += dtheta){

            // 用int进行计算
            cost = (int)(cos(theta * PI / 180) * 2048);
            sint = (int)(sin(theta * PI / 180) * 2048);

            for (int i = 0; i < height; i++){
                for (int j = 0; j < width; j++){

                    if (bin_img.at<uchar>(i, j) != 0){
                        // 移位代替除法
                        X0_pre = j - ((radius * cost) >> 11);
                        // 去除异常点
                        if ((X0_pre < 0) || (X0_pre > width - 1)) continue;
                        Y0_pre = i - ((radius * sint) >> 11);
                        // 去除异常点
                        if ((Y0_pre < 0) || (Y0_pre > height - 1)) continue;
                        // 计数器递增
                        cntMap.at<uchar>(Y0_pre, X0_pre) += 1;
                        
                    }
                }
            }
        }

        cv::minMaxLoc(cntMap, NULL, &maxValue, NULL, &maxPt);
        if (zoom) {cntMap = cntMap*255/maxValue;}

        return Circle(maxPt.x, maxPt.y, radius, maxValue);

    }        



    // 用非极大值抑制去除相距太近的圆心
    // threshold : 最小count值的阈值，范围为0-1
    // 邻域大小
    // 假设同一半径的圆不重叠
    std::vector<Circle> NMS_CountMap(cv::Mat & cntMap, int radius, double threshold, int range){
        std::vector<Circle> circles;
        double maxValue;
        cv::Point2i maxPt;

        cv::minMaxLoc(cntMap, NULL, &maxValue, NULL, &maxPt);
        // circles.push_back(Circle(maxPt.x, maxPt.y, radius, maxValue));
        double thresValue = threshold * maxValue;

        // 先去除count值小的部分
        for (int i = 0; i < cntMap.rows; i++){
            for (int j = 0; j < cntMap.cols; j++){
                cntMap.at<uchar>(i, j) = cntMap.at<uchar>(i, j) > (thresValue) ? cntMap.at<uchar>(i, j) : 0;
            }
        }

        double curMax = maxValue;
        cv::Point2i curMaxPt;
        while(curMax > thresValue){
            circles.push_back(Circle(curMaxPt.x, curMaxPt.y, radius, curMax));
            // 最大值点邻域清空
            for (int i = curMaxPt.y - range; i < curMaxPt.y + range + 1; i++){
                for (int j = curMaxPt.x - range; j < curMaxPt.x + range + 1; j++){
                    cntMap.at<uchar>(i, j) = 0;
                }
            }
            cv::minMaxLoc(cntMap, NULL, &curMax, NULL, &curMaxPt);    
        }

        return circles;

    }



    int RmwHoughCircle(const cv::Mat & bin_img, int radius, cv::Point3i & circle, int dtheta){
        const int width = bin_img.cols;
        const int height = bin_img.rows;

        int *pCount = new int[width * height];
        int *x0 = new int;
        int *y0 = new int;
        int *pCur;
        int X0_pre, Y0_pre;
        int cost, sint;
        int maxCount;
        memset(pCount, 0, width * height * sizeof(int));

        for (int theta = 0; theta < 360; theta += dtheta){

            // 用int进行计算
            cost = (int)(cos(theta * PI / 180) * 2048);
            sint = (int)(sin(theta * PI / 180) * 2048);

            for (int i = 0; i < height; i++){
                for (int j = 0; j < width; j++){

                    if (bin_img.at<uchar>(i, j) != 0){
                        // 移位代替除法
                        X0_pre = j - ((radius * cost) >> 11);
                        // 去除异常点
                        if ((X0_pre < 0) || (X0_pre > width - 1)) continue;
                        Y0_pre = i - ((radius * sint) >> 11);
                        // 去除异常点
                        if ((Y0_pre < 0) || (Y0_pre > height - 1)) continue;
                        // 计数器递增
                        pCount[X0_pre * width + Y0_pre] += 1;

                    }
                }
            }
        }


        *x0 = *y0 = maxCount = 0;

        for (Y0_pre = 0, pCur = pCount; Y0_pre < height; Y0_pre++)
        {
            for (X0_pre = 0; X0_pre < width; X0_pre++, pCur++)
            {
                if (*pCur > maxCount)
                {
                    maxCount = *pCur;
                    *x0 = X0_pre;
                    *y0 = Y0_pre;
                }
            }
        }
 
        circle = cv::Point3i(*x0, *y0, radius);

        delete [] pCount;
        delete x0;
        delete y0;

        return maxCount; 

    }
}

namespace hough_RANSAC{


    void computeHoughVote(Mat &Vote, Mat img, double radius, map<int,pair<double,double>> thetaMap, int &maxVote){
        
        int rows = img.rows;
        int cols = img.cols;
        Scalar pix;
        int a, b, theta, i, j;
        
        //loop through each pixel of image
        for(i=0; i<rows; i++){
            for(j=0; j<cols; j++){
                
                //only compute Hough transform on edge pixels
                pix = img.at<uchar>(i,j);
                if(pix.val[0] != 0){
                    for (theta=0; theta < 360; theta++) {
                        a = (int)(i - radius*thetaMap[theta].first);
                        b = (int)(j + radius*thetaMap[theta].second);
                        
                        //only increase vote if value are inbounds
                        if(a >=0 && a < rows && b >= 0 && b < cols){
                            Vote.at<short>(a,b)++ ;
                            
                            if(Vote.at<short>(a,b) > maxVote){
                                maxVote = Vote.at<short>(a,b);
                            }
                        }
                    }
                }
            }
        }
    }

    void findHoughPeak(Mat &Vote, int maxVote, int numberPeaks, vector<Circle> &peakCenters, double radius){
        
        int threshold = 0.8 * maxVote;
        
        //If threshold under 100, it's probably not a circle
        if(threshold < 100) threshold = 100;
        
        Point maxPt;
        double maxValue;
        Circle newCircle;
        
        int numP = 0;
        int clearzone = 4;
        
        //loop until desired nnumber of peaks are reach
        while(numP < numberPeaks){
            
            //find max value of HoughVote and location a/b
            minMaxLoc(Vote, NULL, &maxValue, NULL, &maxPt);
            
            //if maxValue over threshold
            if(maxValue > threshold){
                numP++;
                
                //create new Circle
                newCircle.maxVote = maxValue;
                newCircle.xCenter = maxPt.x;
                newCircle.yCenter = maxPt.y;
                newCircle.radius = radius;
                
                //store newCircle
                peakCenters.push_back(newCircle);
                
                //set neighborhood zone to zero to avoid circle in same region
                for(int i=maxPt.x-clearzone; i<=maxPt.x+clearzone; i++){
                    for(int j=maxPt.y-clearzone; j<maxPt.y+clearzone; j++){
                        Vote.at<short>(j,i)=0;
                    }
                }
            }
            else{
                break;
            }
        }
    }

    //Check if circle already present
    bool checkCirclePresent( vector<Circle> &bestCircles, Circle newCircle, int pixelInterval){
        
        bool found = false;
        
        //Loop through BestCircle vector
        for(vector<Circle>::iterator it = bestCircles.begin(); it != bestCircles.end(); /*nothing*/)
        {
            //Check if circle with same center already present
            if((newCircle.xCenter <= it->xCenter+pixelInterval && newCircle.xCenter >= it->xCenter-pixelInterval) && (newCircle.yCenter <= it->yCenter+pixelInterval && newCircle.yCenter >= it->yCenter-pixelInterval))
            {
                //If already present, check if new circle has more vote, if yes, keep remove old circle, if no discard newcircle
                if(it->maxVote < newCircle.maxVote)
                {
                    it = bestCircles.erase(it);
                    found = false;
                    break;
                }
                else{
                    //check if it's a circle within a circle using a ratio of twice the smaller radius
                    if(it->radius*2 < newCircle.radius){
                        found = false;
                        ++it;
                    }
                    else{
                        found = true;
                        ++it;
                    }
                }
            }
            else{
                it++;
            }
        }
        
        //Only circle returned false will be added to the BestCircle vector
        return found;
    }

    void HoughTransform(Mat imgEdges, vector<Circle> &bestCircles, int radiusStart, int radiusEnd){
        
        int rows = imgEdges.rows;
        int cols = imgEdges.cols;
        int maxVote = 0;
        int numberPeaks = 10;
        int pixelInterval = 15;
        int size[2] = {rows, cols};
        Mat HoughVote;
        vector<Circle> peakCenters;
        
        //Compute all possible theta from degree to radian and store them into a map to avoid overcomputation
        map<int, pair<double, double>> thetaMap;
        for (int thetaD=0; thetaD <360; thetaD++) {
            double thetaR = static_cast<double>(thetaD) * (CV_PI / 180);
            thetaMap[thetaD].first = cos(thetaR);
            thetaMap[thetaD].second = sin(thetaR);
        }
        
        //Loop for each possible radius - radius range may need to be changed following the image (size of circle)
        for (double r = radiusStart; r <radiusEnd; r+=1.0){
            
            //Initialize maxVote, accumulator, peakCenters to zeros
            maxVote= 0;
            HoughVote = Mat::zeros(2,size, CV_16U);
            peakCenters.clear();
            
            //Compute Vote for each edge pixel
            computeHoughVote(HoughVote, imgEdges, r, thetaMap, maxVote);
            
            //Find Circles with maximum votes
            findHoughPeak(HoughVote, maxVote, numberPeaks, peakCenters, r);
            
            //For each Circle find, only keep the best ones (max votes) and remove duplicates
            for(int i=0; i<peakCenters.size(); i++){
                bool found = checkCirclePresent(bestCircles, peakCenters[i], pixelInterval);
                if(!found){
                    bestCircles.push_back(peakCenters[i]);
                }
            }
        }
        
    }

    void circleRANSAC(vector<Point> edgePoints, vector<Circle> &bestCircles, int iterations){
        
        //Define three points to define a circle
        Point A, B, C, D;
        
        //Define slopes, intercepts between points
        double slope_AB, slope_BC, intercept_AB, intercept_BC;
        
        //Define midpoints
        Point midpt_AB, midpt_BC;
        
        //Define slopes, intercepts midpoints perpendicular
        double slope_midptAB, slope_midptBC, intercept_midptAB, intercept_midptBC;
        
        RNG random;
        Point center;
        double radius;
        double circumference;
        Circle circleFound;
        
        for(int i=0; i<iterations;i++){
            
            //Select three points at random among edgePoints vector
            A = edgePoints[random.uniform((int)0, (int)edgePoints.size())];
            B = edgePoints[random.uniform((int)0, (int)edgePoints.size())];
            C = edgePoints[random.uniform((int)0, (int)edgePoints.size())];
            
            //Calculate midpoints
            midpt_AB = (A+B)*0.5;
            midpt_BC = (B+C)*0.5;
            
            //Calculate slopes and intercepts
            slope_AB = (B.y - A.y) / (B.x - A.x + 0.000000001);
            intercept_AB = A.y - slope_AB * A.x;
            slope_BC = (C.y - B.y) / (C.x - B.x + 0.000000001);
            intercept_BC = C.y - slope_BC * C.x;
            
            //Calculate perpendicular slopes and intercepts
            slope_midptAB = -1.0 / slope_AB;
            slope_midptBC = -1.0 / slope_BC;
            intercept_midptAB = midpt_AB.y - slope_midptAB * midpt_AB.x;
            intercept_midptBC = midpt_BC.y - slope_midptBC * midpt_BC.x;
            
            //Calculate intersection of perpendiculars to find center of circle and radius
            double centerX = (intercept_midptBC - intercept_midptAB) / (slope_midptAB - slope_midptBC);
            double centerY =  slope_midptAB * centerX + intercept_midptAB;
            center = Point(centerX, centerY);
            Point diffradius = center - A;
            radius = sqrt(diffradius.x*diffradius.x + diffradius.y*diffradius.y);
            circumference = 2.0 * CV_PI * radius;
            
            vector<int> onCircle;
            vector<int> notOnCircle;
            int radiusThreshold = 3;
            
            //Find edgePoints that fit on circle radius
            for(int i =0; i< edgePoints.size(); i++){
                Point diffCenter = edgePoints[i] - center;
                double distanceToCenter = sqrt(diffCenter.x*diffCenter.x + diffCenter.y*diffCenter.y);
                if(abs(distanceToCenter - radius) < radiusThreshold){
                    onCircle.push_back(i);
                }
                else{
                    notOnCircle.push_back(i);
                }
            }
            
            //If number of edgePoints more than circumference, we found a correct circle
            if( (double)onCircle.size() >= circumference )
            {
                circleFound.xCenter = center.x;
                circleFound.yCenter = center.y;
                circleFound.radius = radius;
                
                bestCircles.push_back(circleFound);
                
                //remove edgePoints if circle found (only keep non-voting edgePoints)
                vector<Point> toKeep;
                for(int i = 0; i < (int)notOnCircle.size(); i++)
                {
                    toKeep.push_back(edgePoints[notOnCircle[i]]);
                }
                edgePoints.clear();
                edgePoints = toKeep;
            }
            
            //stop iterations when there is not enough edgePoints
            if(edgePoints.size() < 100){
                break;
            }
        }
    }

    //Draw best circles on image
    Mat drawCircles(Mat img, vector<Circle> bestCircles, const int limit)
    {
        Mat result;
        
        //Transform image to RGB to draw circles in color
        cvtColor(img, result, CV_GRAY2BGR);
        
        for (int i=0; i < limit; ++i) {
            circle(result, Point(bestCircles[i].xCenter, bestCircles[i].yCenter), bestCircles[i].radius, Scalar(255,0,0),4);
        }
        return result;
    }
}