#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int alpha, beta;
Mat image;
Mat gray, imgresize, imgblur, imgdil, imgth;

/*
void on_track(int, void*){
    threshold(imgblur, image, alpha, 255, 0);
    resize(image,imgresize,Size(),0.3,0.3);
    imshow("result",imgresize);
}
*/

Mat cameraMatrix = (Mat_<double>(3, 3) << 3945.45992802447, 0, 1378.521371614085, 0, 3962.445556513878, 2200.710056708469, 0, 0, 1);
Mat distCoeffs = (Mat_<double>(1, 5) << 0.3139766312433063, -1.735717776661014, 0.01567993142102008, -0.01343764071682717, 5.621684162487409);


void calculateDistance(const vector<Point2f>& imagePoints) {
    // 定义色块的真实三维坐标 (单位: cm)
    vector<Point3f> objectPoints;
    objectPoints.push_back(Point3f(0, 0, 0));
    objectPoints.push_back(Point3f(9.6, 0, 0));
    objectPoints.push_back(Point3f(9.6, 9.6, 0));
    objectPoints.push_back(Point3f(0, 9.6, 0));

    // solvePnP 计算旋转矩阵和平移向量
    Mat rvec, tvec;
    solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

    // 计算距离 (即平移向量 tvec 的模长)
    double distance = norm(tvec);

    // 打印距离
    cout << "距离: " << distance<< " cm" << endl;

    // 在图像上标注距离
    String text = format("Distance: %.2f cm", distance);
    putText(image, text, Point(imagePoints[0].x, imagePoints[0].y - 10),
            FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
}

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
    image = imread("./test2.jpg");
    imgblur = Mat::zeros(image.size(),image.type());
    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, imgblur, Size(7,7),3,0);
    threshold(imgblur, imgth, 62, 255, 1);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(imgth, imgdil, kernel);

    //查找轮廓
    vector<vector<Point>> contours;
    vector<Point> largestContour;
    findContours(imgdil, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //寻找面积最大的轮廓
    double area, maxarea = 0;
    int ii;
    for (int i=0; i<contours.size();i++) {
        area = contourArea(contours[i]);
        if (area > maxarea) {
            maxarea = area;
            ii = i;
            largestContour = contours[ii];
        }
    }
    drawContours(image, contours, ii, Scalar(0, 0, 255), 5);

    vector<Point2f> approx;
    approxPolyDP(largestContour, approx, arcLength(largestContour, true) * 0.02, true);
    
    /*
    namedWindow("Trackbar",1);
    createTrackbar("阈值","Trackbar",&alpha,255,on_track);
    */

    calculateDistance(approx);

    resize(image,imgresize,Size(),0.3,0.3);
    imshow("result",imgresize);
    waitKey(0);

    return 0;
}
