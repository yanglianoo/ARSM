#include "../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

void show(std::vector<DataBase*> data)
{
    ImageData *Data = static_cast<ImageData *>(data[0]);
    cv::Mat colorMat = cv::Mat(cv::Size(Data->width, Data->height), CV_8UC3, Data->data, cv::Mat::AUTO_STEP);
    std::cout << colorMat << std::endl;
    return;
}
int main()
{
    std::string path = "../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("USB");
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
