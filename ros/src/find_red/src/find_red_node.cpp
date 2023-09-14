// ROS
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>

#include "find_red/imageMsg.h"
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
// cv_bridge
#include <cv_bridge/cv_bridge.h>
// STD
#include <iostream>
ros::Publisher pub;
ros::Subscriber sub;
int last_stamp, stamp;

cv::Mat image;
void imageCB(const find_red::imageMsgConstPtr &msg)
{
    image = cv_bridge::toCvCopy(msg->image, sensor_msgs::image_encodings::BGR8)->image;
    stamp = msg->stamp;
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::Mat red;
    cv::inRange(hsv, cv::Scalar(156, 43, 46), cv::Scalar(180, 255, 255), red);
    cv::erode(red,red,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7))); // 腐蚀
    cv::dilate(red, red, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11))); // 膨胀,开运算
    cv::medianBlur(red, red, 7);    // 中值滤波
    cv::imshow("red", red);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE); // 找轮廓
    for (int i = 0; i < contours.size(); i++)
    {
        cv::Rect rect = cv::boundingRect(contours[i]);  // 外接矩形
        cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 2);
    }
    cv::putText(image,std::string("fps: "+std::to_string(1.0*CLOCKS_PER_SEC/(stamp-last_stamp))+" stamp: "+std::to_string(stamp)),cv::Point(0,50),cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(0,0,255),2);
    last_stamp = stamp;
    cv::imshow("image", image);

    cv::waitKey(3);

    std_msgs::String sendmsg;
    sendmsg.data = "red count number: " + std::to_string(contours.size());
    pub.publish(sendmsg);
}
int main(int argc,char* argv[],char** env)
{
    ros::init(argc, argv, "find_red_node");
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_(nh_);
    pub = nh_.advertise<std_msgs::String>("/find_red/count", 100);
    sub = nh_.subscribe("/camera/image", 100, &imageCB);

    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::namedWindow("red", cv::WINDOW_NORMAL);

    while(ros::ok)
    {
        ros::spinOnce();
    }
    return 0;
}