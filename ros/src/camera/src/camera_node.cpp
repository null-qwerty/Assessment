// ROS
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>

#include "camera/imageMsg.h"
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
// cv_bridge
#include <cv_bridge/cv_bridge.h>
// STD
#include <iostream>

int main(int argc,char* argv[],char** env)
{
    ros::init(argc,argv,"camera_node");
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_(nh_);
    // image_transport::Publisher pub = it_.advertise("/camera/image", 100);
    ros::Publisher pub = nh_.advertise<camera::imageMsg>("/camera/image", 100);
    ros::Rate loop_rate(100);

    cv::Mat frame;
    cv::VideoCapture camera(0);

    while(ros::ok)
    {
        camera >> frame;
        cv::imshow("camera",frame);
        cv::waitKey(3);
        camera::imageMsg msg;
        msg.image = *cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        msg.stamp = clock();
        pub.publish(msg);
        loop_rate.sleep();
    }

    return 0;
}