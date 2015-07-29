/*
+ 识别照片中所有的圆
*/
#include <cstdlib>
#include <cv.h>
#include <highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

vector<Point3f> extractCircles(Mat& img) {
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    GaussianBlur(gray, gray, Size(9, 9), 2, 2);     // 可调参数，不过这个的结果比较好
    vector<Vec3f> circles;
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, 10, 100, 30, 5, 150);     // 第四个参数表示分辨率相关的问题，并没有弄懂
    for (vector<Vec3f>::iterator it = circles.begin(); it != circles.end(); it ++) {
        Vec3f cur = *it;
        Point center(cvRound(cur[0]), cvRound(cur[1]));
        int radius = cvRound(cur[2]);
        circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    }
    cout << circles.size() << endl;
    namedWindow("circles");
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
