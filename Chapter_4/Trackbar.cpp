#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int beta, alpha;
double alpha_1;
Mat image,new_image;

void on_track(int , void *)
{
    alpha_1 = (double)alpha / 100;
    image.convertTo(new_image, -1, alpha_1, beta);
    imshow("Trackbar",new_image);
}

int main()
{
    image=imread("./Lena.png");
    if (image.empty())
    {
        cout << "not load the image" << endl;
        return -1;
    }
    new_image = Mat::zeros(image.size(), image.type());
    
    namedWindow("Trackbar",1);
    createTrackbar("beta","Trackbar",&beta,100,on_track);
    createTrackbar("alpha","Trackbar",&alpha,300,on_track);
    
    on_track(0,0);

    waitKey(0);
    return 0;
}