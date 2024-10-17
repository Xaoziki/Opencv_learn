#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat image;
Mat gray, imgresize, imgblur, imgdil, imgth;
Mat cameraMatrix = (Mat_<double>(3, 3) << 3916.143931422424, 0, 1385.210988879861, 0, 3933.751441780795, 2208.169167568601, 0, 0, 1);
Mat distCoeffs = (Mat_<double>(1, 5) << 0.2994338494009186, -1.574013213125817, 0.01718489359160927, -0.01208570734985182, 4.904165993574098);

int main(int argc, char** argv) {
    /*
    if (argc != 2) {
        cout << "Usage: ./task2 test.jpg" << endl;
        return -1;
    }

    // 读取输入图片
    Mat image = imread(argv[1]);
    if (image.empty()) {
        cerr << "Error: Could not open image!" << endl;
        return -1;
    }
    */

    //预处理
    image = imread("./test4.jpg");
    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, imgblur, Size(7,7),3,0);
    threshold(imgblur, imgth, 100, 255, 1);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(imgth, imgdil, kernel);

    //查找轮廓
    vector<vector<Point>> contours;
    findContours(imgdil, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //寻找面积最大的轮廓
    double area, maxarea = 0;
    int ii;
    for (int i=0; i<contours.size();i++) {
        area = contourArea(contours[i]);
        if (area > maxarea) {
            maxarea = area;
            ii = i;
        }
    }
    //绘制轮廓
    drawContours(image, contours, ii, Scalar(0, 0, 255), 5);

    vector<Point2f> approx;
    approxPolyDP(contours[ii], approx, arcLength(contours[ii], true) * 0.02, true);
    
    //定义3D点
    vector<Point3f> objectPoints = {
        Point3f(0, 0, 0),
        Point3f(9.6, 0, 0),
        Point3f(9.6, 9.6, 0),
        Point3f(0, 9.6, 0)
    };

    //进行PNP求解
    Mat rvec, tvec;
    solvePnP(objectPoints, approx, cameraMatrix, distCoeffs, rvec, tvec);

    double distance = norm(tvec);
    cout << "距离: " << distance<< " cm" << endl;

    String text = format("Distance: %.2f cm", distance);
    putText(image, text, Point(contours[ii][0]),FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 2);
    
    resize(image,imgresize,Size(),0.3,0.3);
    imshow("result",imgresize);
    waitKey(0);

    return 0;
}
