#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main() {
    Mat matHSV(255, 255, CV_8UC3, Scalar(0, 0, 0));
    int h, s, v;
    while (true) {
        cin >> v;
        MatIterator_<Vec3b> it = matHSV.begin<Vec3b>();
        for (int h = 0; h < 255; h ++) {
            for (int s = 0; s < 255; s ++) {
                (*it)[0] = h;
                (*it)[1] = s;
                (*it)[2] = v;
                it ++;
            }
        }
        // imshow("hsv", matHSV);
        // waitKey(10);
        Mat matBGR(255, 255, CV_8UC3, Scalar(0, 0, 0));
        cvtColor(matHSV, matBGR, CV_HSV2BGR_FULL);
        imshow("BGR", matBGR);
        waitKey(0);
    }
    return 0;
}
