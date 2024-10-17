#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    Mat img = imread("./Lena.png");
    Mat imgresize;
    //cout << img.size() <<endl;
    resize (img ,imgresize,Size(),0.5,0.7);
    cout << imgresize.size() <<endl;

    imshow("img",img);
    imshow("imgresize",imgresize);
    waitKey(0);
    exit(0);
    return 0;
}