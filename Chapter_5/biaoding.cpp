#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 设置棋盘格的大小（内角点的行列数）
const int boardWidth = 8;  // 棋盘格的列数
const int boardHeight = 12; // 棋盘格的行数
const Size boardSize(boardWidth, boardHeight);

// 设置每个方格的物理尺寸（单位可以是毫米、厘米等）
const float squareSize = 20.0f; // 每个方格的实际大小

// 读取图像文件名
bool readImageList(const string& filename, vector<string>& imageList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            imageList.push_back(line);
        }
    }
    file.close();
    return true;
}

// 保存标定结果
void saveCalibrationResult(const string& filename, const Mat& cameraMatrix, const Mat& distCoeffs) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }
    file << "相机内参矩阵:" << endl;
    file << cameraMatrix << endl;
    file << "畸变系数:" << endl;
    file << distCoeffs << endl;
    file.close();
}

int main() {
    // 读取图像列表
    vector<string> imageList;
    if (!readImageList("calibdata.txt", imageList)) {
        return -1;
    }

    // 世界坐标系下的棋盘格角点坐标
    vector<vector<Point3f>> objectPoints;
    vector<Point3f> obj;
    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            obj.push_back(Point3f(j * squareSize, i * squareSize, 0));
        }
    }

    // 图像坐标系下的角点坐标
    vector<vector<Point2f>> imagePoints;
    Size imageSize;

    for (const auto& imageFile : imageList) {
        Mat image = imread(imageFile);
        if (image.empty()) {
            cerr << "无法读取图像: " << imageFile << endl;
            continue;
        }

        // 转换为灰度图
        Mat gray;
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // 寻找棋盘格角点
        vector<Point2f> corners;
        bool found = findChessboardCorners(gray, boardSize, corners,
                                           CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            // 亚像素精确化角点
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));

            // 保存角点
            imagePoints.push_back(corners);
            objectPoints.push_back(obj);

            // 可视化角点检测结果
            drawChessboardCorners(image, boardSize, corners, found);
            imshow("Chessboard Corners", image);
            waitKey(500); // 显示0.5秒
        } else {
            cerr << "未找到棋盘格角点: " << imageFile << endl;
        }

        // 更新图像大小
        if (imageSize == Size()) {
            imageSize = image.size();
        }
    }

    destroyAllWindows();

    if (imagePoints.size() < 5) {
        cerr << "有效图像数量不足，无法进行标定。" << endl;
        return -1;
    }

    // 相机标定
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);

    cout << "标定完成，重投影误差 = " << rms << endl;

    // 保存标定结果
    saveCalibrationResult("caliberation_result.txt", cameraMatrix, distCoeffs);

    cout << "标定结果已保存到 caliberation_result.txt" << endl;

    return 0;
}
