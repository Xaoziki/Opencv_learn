#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    Mat img = imread("./Lena.png");
    Mat imgGray, imgblur, imgcanny,imgdil, imgero;

    cvtColor(img,imgGray,COLOR_BGR2GRAY);
    GaussianBlur(img,imgblur,Size(7,7),5,0);
    Canny(imgblur,imgcanny,25,75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(imgcanny,imgdil,kernel);
    erode(imgdil,imgero,kernel);

    //imshow("imgblur",imgblur);
    //imshow("imgcanny",imgcanny);
    imshow("img dilation",imgdil);
    imshow("img erode",imgero);

    waitKey(0);
    exit(0);
    return 0;
}