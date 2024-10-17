#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

int main() {

    string file;
    getline (cin,file);
    file = "./" + file;
    Mat img = imread(file);
    Mat imghsv, mask, imgresize, maskresize;
    int hmin = 10, smin = 10, vmin = 10;
    int hmax = 20, smax = 20, vmax = 20;

    namedWindow("trackbars", (640, 200));
    createTrackbar("H min", "trackbars", &hmin, 179);
    createTrackbar("H max", "trackbars", &hmax, 179);
    createTrackbar("S min", "trackbars", &smin, 255);
    createTrackbar("S max", "trackbars", &smax, 255);
    createTrackbar("V min", "trackbars", &vmin, 255);
    createTrackbar("V max", "trackbars", &vmax, 255);
    
    cvtColor(img, imghsv, COLOR_BGR2HSV);

    while (true) {

        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(imghsv, lower, upper, mask);

        resize(img, imgresize, Size(), 0.3,0.3);
        resize(mask, maskresize, Size(), 0.3,0.3);
        imshow("img", imgresize);
        imshow("mask", maskresize);
        waitKey(0);
    }
    return 0;
}