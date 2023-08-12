#include "../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <opencv2/opencv.hpp>



void show(DataBase *data)
{
    ImageData *Data = static_cast<ImageData *>(data);
    std::cout << Data->type << std::endl;
    std::cout << Data->ptr << std::endl;
    
    cv::Mat colorMat = cv::Mat(cv::Size(640, 480), CV_8UC3, Data->ptr, cv::Mat::AUTO_STEP);
    // // depthMat = cv::Mat(cv::Size(640, 480), CV_16UC1, data->ptr, cv::Mat::AUTO_STEP);

    cv::namedWindow("RGB", cv::WINDOW_NORMAL);
    cv::imshow("RGB", colorMat);
    cv::waitKey(0);

    return;
}
int main()
{
    std::string path = "../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("RealSenseD435i");
    OpenDevice(dev);
    std::vector<DataBase*> data;
    
    while(1)
    {
        // int ret =  Auto_Monitor(dev);
        // if(ret == 1)
        // {
        GetFrameData(dev,data);
        show(data[0]);
        show(data[1]);
        // ImageData *RgbData = static_cast<ImageData *>(data[0]);
        // ImageData *DepthData = static_cast<ImageData *>(data[1]);
        // cv::Mat colorMat = cv::Mat(cv::Size(640, 480), CV_8UC3, RgbData->ptr, cv::Mat::AUTO_STEP);
        // cv::Mat depthMat = cv::Mat(cv::Size(640, 480), CV_16UC1, DepthData->ptr, cv::Mat::AUTO_STEP);

        // cv::namedWindow("RGB", cv::WINDOW_NORMAL);
        // cv::namedWindow("Depth", cv::WINDOW_NORMAL);
        // cv::imshow("RGB", colorMat);
        // cv::imshow("Depth", depthMat);
        // cv::waitKey(0);
        // }
    }
    return 0;
}
