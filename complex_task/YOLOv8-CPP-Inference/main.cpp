#include <iostream>
#include <vector>
#include <getopt.h>

#include <opencv2/opencv.hpp>

#include "inference.h"

using namespace std;
using namespace cv;

std::vector<cv::Point> Trace;
std::vector<cv::Point> Pred_trace;
cv::Point Pred_point;
bool first = true;
int Diff[10][2];
int Last_diff[10][2];

int main(int argc, char **argv)
{
    std::string projectBasePath = "../"; // Set your ultralytics base path

    bool runOnGPU = true;

    //
    // Pass in either:
    //
    // "yolov8s.onnx" or "yolov5s.onnx"
    //
    // To run Inference with yolov8/yolov5 (ONNX)
    //

    // Note that in this example the classes are hard-coded and 'classes.txt' is a place holder.
    Inference inf(projectBasePath + "../model/best.onnx", cv::Size(640, 480), projectBasePath+"classes.txt", runOnGPU);

    // std::vector<std::string> imageNames;
    // imageNames.push_back(projectBasePath + "/ultralytics/assets/bus.jpg");
    // imageNames.push_back(projectBasePath + "/ultralytics/assets/zidane.jpg");

    cv::VideoCapture cam;
    cv::Mat frame;
    cam.open(projectBasePath + "12mm_blue_dark.MP4");
    while (cam.isOpened())
    {
        // cv::Mat frame = cv::imread(imageNames[i]);
        cam >> frame;

        // Inference starts here...
        std::vector<Detection> output = inf.runInference(frame);

        int detections = output.size();
        std::cout << "Number of detections:" << detections << std::endl;

        for (int i = 0; i < detections; ++i)
        {
            Detection detection = output[i];
            if(detection.className!="unactivated_blue") continue;

            memcpy(Last_diff, Diff, sizeof(Diff));

            cv::Rect box = detection.box;
            cv::Scalar color = detection.color;

            cv::Point centerP= cv::Point(box.x+box.width/2, box.y+box.height/2);
            if(first)
            {
                Pred_point=centerP;
                first = false;
            }
            if((Pred_point.x-centerP.x)*(Pred_point.x-centerP.x)+(Pred_point.y-centerP.y)*(Pred_point.y-centerP.y)>=400)
            {
                Trace.clear();
                Pred_trace.clear();
                memset(Diff, 0, sizeof(Diff));
                memset(Last_diff, 0, sizeof(Last_diff));
                first = true;
                continue;
            }

            cv::circle(frame, centerP, 5, cv::Scalar(0, 0, 255), -1);
            Trace.push_back(centerP);
            Diff[0][0]=Trace.size()==1?0:Trace[Trace.size()-1].x-Trace[Trace.size()-2].x;
            Diff[0][1]=Trace.size()==1?0:Trace[Trace.size()-1].y-Trace[Trace.size()-2].y;
            for(int i=1;i<10;i++)
            {
                Diff[i][0]=Diff[i-1][0]-Last_diff[i-1][0];
                Diff[i][1]=Diff[i-1][1]-Last_diff[i-1][1];
            }
            int Pred_x=centerP.x, Pred_y=centerP.y;
            int factor=1;
            for(int i=0;i<10;i++)
            {
                Pred_x+=1.0*Diff[i][0]/factor*cv::pow(0.5, i);
                Pred_y+=1.0*Diff[i][1]/factor*cv::pow(0.5, i);
            }
            cv::Point Pred_point(Pred_x, Pred_y);
            cv::circle(frame, Pred_point, 5, cv::Scalar(0, 255, 0), -1);
            Pred_trace.push_back(Pred_point);

            if(Trace.size()>10) Trace.erase(Trace.begin());
            for(int i=0;i<Trace.size()-1;i++)
            {
                cv::line(frame, Trace[i], Trace[i+1], cv::Scalar(0, 0, 255), 2);
            }
            if(Pred_trace.size()>10) Pred_trace.erase(Pred_trace.begin());
            for(int i=0;i<Pred_trace.size()-1;i++)
            {
                cv::line(frame, Pred_trace[i], Pred_trace[i+1], cv::Scalar(0, 255, 0), 2);
            }

            // Detection box
            cv::rectangle(frame, box, color, 2);

            // Detection box text
            std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
            cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
            cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

            cv::rectangle(frame, textBox, color, cv::FILLED);
            cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
        }
        // Inference ends here...

        // This is only for preview purposes
        float scale = 0.8;
        cv::resize(frame, frame, cv::Size(frame.cols*scale, frame.rows*scale));
        cv::imshow("Inference", frame);

        cv::waitKey(1);
    }
}
