// ROS
#include <ros/ros.h>
#include <std_msgs/String.h>
// STD
#include <iostream>

void countCB(const std_msgs::StringConstPtr &msg)
{
    std::cout << msg->data << std::endl;
}
int main(int argc,char* argv[],char** env)
{
    ros::init(argc, argv, "serial_node");
    ros::NodeHandle nh_;
    ros::Subscriber sub;
    sub = nh_.subscribe<std_msgs::String>("/find_red/count", 100,countCB);

    ros::spin();
    return 0;
}