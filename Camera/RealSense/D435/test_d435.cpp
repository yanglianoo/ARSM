#include "../../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <opencv2/opencv.hpp>


void show(std::vector<DataBase*> data)
{
    D435ComposeFrame *Data = static_cast<D435ComposeFrame *>(data[0]);
    int height = Data->m_depth_frame->height;
    int width = Data->m_depth_frame->width;
    int size = Data->m_depth_frame->size;
    
    // Color Data
    cv::Mat colorMat = cv::Mat(cv::Size(width, height), CV_8UC3, Data->m_color_frame->data, cv::Mat::AUTO_STEP);
    std::cout << "Color Data:" << colorMat << std::endl;

    // if enable depth
    if (Data->m_depth_frame != nullptr){
        cv::Mat depthMat = cv::Mat(cv::Size(width, height), CV_16UC1, Data->m_depth_frame->data, cv::Mat::AUTO_STEP);
        // std::cout << "Depth Data Height:" << height << std::endl;
        // std::cout << "Depth Data Width:" << width << std::endl;
        // std::cout << "Depth Data Size:" << size << std::endl;
        std::cout << "Depth Data:" << depthMat << std::endl;
    }
    
    // TODO cv window
    // cv::namedWindow("RGB", cv::WINDOW_NORMAL);
    // cv::imshow("RGB", colorMat);
    // cv::waitKey(0);
    return;
}

int main()
{
    std::string path = "../../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("RealSenseD435");
    std::vector<DataBase*> data;
    while(1)
    {
        SensorState ret =  Auto_Monitor(dev);
        if(ret == RUNNING)
        {
            GetFrameData(dev,data);
            show(data);
        }
    }
    return 0;
}
