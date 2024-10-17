#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    // 打开视频文件
    VideoCapture cap("./test.mp4");

    // 检查视频文件是否成功打开
    if (!cap.isOpened()) {
        cerr << "Error: Could not open video file." << endl;
        return -1;
    }

    Mat img;

    while (true) {
        // 读取视频帧
        if (!cap.read(img)) {
            cout << "End of video, exiting..." << endl;
            break;
        }

        // 显示视频帧
        imshow("Video", img);

        // 根据帧率调整等待时间
        if (waitKey(30) >= 0) {
            break; // 按下任意键退出
        }
    }

    // 释放资源
    cap.release();
    destroyAllWindows();

    return 0;
}
