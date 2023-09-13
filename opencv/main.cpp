#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc,char* argv[],char** env)
{
    cv::Mat src=cv::imread("../assets/src.jpg");

    cv::Mat bin;
    cv::threshold(src, bin, 0, 255, cv::THRESH_BINARY);
    cv::imshow("src", src);
    cv::imshow("bin", bin);
    
    cv::waitKey(0);
    return 0;
}