#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int x = 2, y = 2;
int thre=206;
cv::Mat src, gray, gaus, bin;
void threshold(int, void *);
void gaussX(int, void *)
{
    cv::GaussianBlur(gray, gaus, cv::Size(), x, y); // 高斯模糊
    cv::imshow("gauss", gaus);
    threshold(thre, 0);
}
void gaussY(int, void *)
{
    cv::GaussianBlur(gray, gaus, cv::Size(), x, y); // 高斯模糊
    cv::imshow("gauss", gaus);
    threshold(thre, 0);
}
void threshold(int, void *)
{
    cv::threshold(gaus, bin, thre, 255, cv::THRESH_BINARY); // 二值化
    cv::imshow("binary", bin);
    cv::dilate(bin, bin, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11))); // 膨胀，填充内部空洞
    cv::imshow("temp", bin);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(bin, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); // 找轮廓
    std::cout << contours.size() << std::endl;
    std::sort(contours.begin(), contours.end(), [](std::vector<cv::Point> &a, std::vector<cv::Point> &b) {
        return cv::contourArea(a) > cv::contourArea(b);
    }); // 按面积排序，最大的轮廓是口罩

    cv::Mat matClone;
    src.copyTo(matClone);
    std::vector<cv::Point2f> poly;  // 多边形拟合顶点
    cv::approxPolyDP(contours[0], poly, 10, true);  // 多边形拟合
    int i = 0;
    for (auto point : poly)
    {
        cv::circle(matClone, point, 2, cv::Scalar(255, 0, 0), 2);
        cv::putText(matClone, std::to_string(i++), point, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
    }
    cv::imshow("res", matClone);
    cv::Mat output;
    auto MM = cv::getPerspectiveTransform(poly, std::vector<cv::Point2f>{cv::Point2f(0, 0), cv::Point2f(0, 680), cv::Point2f(360, 680), cv::Point2f(360, 0)});  // 计算透视变换矩阵
    cv::warpPerspective(src, output, MM, cv::Size(360, 680));   // 透视变换
    cv::imshow("output", output);
}
int main(int argc, char *argv[], char **env)
{
    src = cv::imread("../assets/src.jpg");
    cv::resize(src, src, cv::Size(src.cols / 4, src.rows / 4));

    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("gray", gray);
    cv::namedWindow("gauss", cv::WINDOW_NORMAL);
    cv::namedWindow("binary", cv::WINDOW_NORMAL);

    cv::createTrackbar("sigmaX", "gauss", &x, 10, gaussX);
    cv::createTrackbar("sigmaY", "gauss", &y, 10, gaussY);
    cv::createTrackbar("threshold", "binary", &thre, 255, threshold);
    gaussX(x, 0);
    gaussY(y, 0);
    threshold(thre, 0);

    cv::waitKey(0);
    return 0;
}