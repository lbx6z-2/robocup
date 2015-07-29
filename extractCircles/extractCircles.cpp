#include <cstdlib>
#include <cv.h>
#include <highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

// h: 117.5 ~ 165.5
// s: 45.9 ~ 126.225
// v: 58.905 ~ 147.9

bool isBallHSV(float h, float s, float v){
    int h1 = 161, h2 = 204;
    int s1 = 58, s2 = 118;
    int v1 = 71, v2 = 104;    
    /*if( (h>235.0/2 && h<331.0/2) &&
        (s>0.18*255 && s<0.495*255) &&
        (v>0.231*255 && v<0.58*255))*/
    if (h >= h1 && h <= h2 && s >= s1 && s <= s2 && v >= v1 && v <= v2)
        return true;
    else
        return false;
}

/*
// 颜色降维
void colorReduce(const Mat& image,Mat& outImage,int div)
{
    int nr=image.rows;
    int nc=image.cols;
    outImage.create(image.size(),image.type());
    if(image.isContinuous()&&outImage.isContinuous())
    {
        nr=1;
        nc=nc*image.rows*image.channels();
    }
    for(int i=0;i<nr;i++)
    {
        const uchar* inData=image.ptr<uchar>(i);
        uchar* outData=outImage.ptr<uchar>(i);
        for(int j=0;j<nc;j++)
        {
            *outData++ = *inData++ / div * div + div/2;
        }
    }
}
*/

vector<Point3f> extractCircles(Mat& img) {
    Mat gray;
    // colorReduce(_img, img, 32);
    imshow("img", img);
    cvtColor(img, gray, CV_BGR2GRAY);
    GaussianBlur(gray, gray, Size(9, 9), 2, 2);
    imshow("gray", gray);
    waitKey(30);
    vector<Vec3f> circles;
    // HoughCircles的结果
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, 10, 100, 50, 5, 150);     // 话说这里为什么参数4是2
    for (vector<Vec3f>::iterator it = circles.begin(); it != circles.end(); it ++) {
        Vec3f cur = *it;
        Point center(cvRound(cur[0]), cvRound(cur[1]));
        int radius = cvRound(cur[2]);
        circle(img, center, radius, Scalar(0, 0, 255));
    }
    cout << circles.size() << endl;

    vector<Vec3f> balls;

    // HSV必须满足一定条件
    Mat img_hsv(img.rows, img.cols, CV_32F, 3);
    cvtColor(img, img_hsv, CV_BGR2HSV_FULL);
    
    for (vector<Vec3f>::iterator it = circles.begin(); it != circles.end(); it ++) {
        Vec3f cur = *it;
        int x = cvRound(cur[0]), y = cvRound(cur[1]), r = cvRound(cur[2]);
        int ballPixNum = 0, totalPixNum = 0;    // 总的像素
        for (int i = y-r; i <= y+r; i ++) {
            for (int j = x-r; j <= x+r; j ++) {
                if ( i >= 0 && i < img.rows && j >= 0 && j < img.cols && ((i-y)*(i-y)+(j-x)*(j-x) <= r*r) ) {
                    totalPixNum ++;
                    float h = (float)(img_hsv.at<Vec3b>(i, j)[0]);
                    float s = (float)(img_hsv.at<Vec3b>(i, j)[1]);
                    float v = (float)(img_hsv.at<Vec3b>(i, j)[2]);
                    if (isBallHSV(h, s, v))
                        ballPixNum ++;
                }
            }
        }
        float ratio = (float)ballPixNum/totalPixNum;
        cout << ratio << endl;
        if (ratio > 0.4)
            balls.push_back(cur);
    }

    // 选择半径最大的圆
    int res_r = 0, res_x = 0, res_y = 0;
    bool foundBall = false;
    if (!balls.empty()) {
        foundBall = true;
        for (vector<Vec3f>::iterator it = balls.begin(); it != balls.end(); it ++) {
            Vec3f cur = *it;
            int x = cvRound(cur[0]), y = cvRound(cur[1]), r = cvRound(cur[2]);
            circle(img, Point(x, y), r, Scalar(255, 0, 255), 2);
            if(r > res_r){
                res_r = r;
                res_x = x;
                res_y = y;
            }
        }
    }
    if (foundBall)
        circle(img, Point(res_x, res_y), res_r, Scalar(255, 255, 0), 2);

    imshow("circles", img);
    waitKey(0);
    vector<Point3f> res;
    return res;
}

int main(int argc, char** argv) {
    Mat img = imread(argv[1]);
    vector<Point3f> circles = extractCircles(img);
    return 0;
}


