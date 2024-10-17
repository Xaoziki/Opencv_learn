#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// 已知的色块的实际尺寸 (cm)
const float blockWidth = 10.0;
const float blockHeight = 10.0;

// 相机内参 (假设已知或通过标定获得)
Mat cameraMatrix = (Mat_<double>(3, 3) << 3085.924956718158, 0, 1626.255824526778, 0, 3101.282175832519, 2136.129568885164, 0, 0, 1);
Mat distCoeffs = Mat::zeros(4, 1, CV_64F);  // 假设没有畸变
Mat image;

// 计算目标与相机的距离
void calculateDistance(const vector<Point2f>& imagePoints) {
    // 定义色块的真实三维坐标 (单位: cm)
    vector<Point3f> objectPoints;
    objectPoints.push_back(Point3f(0, 0, 0));
    objectPoints.push_back(Point3f(blockWidth, 0, 0));
    objectPoints.push_back(Point3f(blockWidth, blockHeight, 0));
    objectPoints.push_back(Point3f(0, blockHeight, 0));

    // solvePnP 计算旋转矩阵和平移向量
    Mat rvec, tvec;
    solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

    // 计算距离 (即平移向量 tvec 的模长)
    double distance = norm(tvec);

    // 打印距离
    cout << "距离: " << distance * 100 << " cm" << endl;

    // 在图像上标注距离
    String text = format("Distance: %.2f cm", distance * 100);
    putText(image, text, Point(imagePoints[0].x, imagePoints[0].y - 10),
            FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: ./task2 block.jpg" << endl;
        return -1;
    }

    // 读取输入图片
    Mat image = imread(argv[1]);
    if (image.empty()) {
        cerr << "Error: Could not open image!" << endl;
        return -1;
    }

    // 转换为灰度图像
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // 二值化
    Mat thresh;
    threshold(gray, thresh, 50, 255, THRESH_BINARY_INV);

    // 寻找轮廓
    vector<vector<Point>> contours;
    findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 找到面积最大的轮廓
    double maxArea = 0;
    vector<Point> largestContour;
    for (const auto& contour : contours) {
        double area = contourArea(contour);
        if (area > maxArea) {
            maxArea = area;
            largestContour = contour;
        }
    }

    // 如果没有找到轮廓
    if (largestContour.empty()) {
        cerr << "Error: No contour found!" << endl;
        return -1;
    }

    // 逼近多边形并获取顶点
    vector<Point2f> approx;
    approxPolyDP(largestContour, approx, arcLength(largestContour, true) * 0.02, true);

    // 需要找到四边形
    if (approx.size() != 4) {
        cerr << "Error: Detected contour is not a quadrilateral!" << endl;
        return -1;
    }

    // 绘制轮廓
    drawContours(image, vector<vector<Point>>{approx}, -1, Scalar(0, 0, 255), 5);

    // 计算距离
    calculateDistance(approx);

    // 显示结果图像
    imshow("Result", image);
    waitKey(0);

    return 0;
}
