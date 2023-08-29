#include "../../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <opencv2/opencv.hpp>



void show(std::vector<DataBase*> data)
{
    T265ComposeFrame *Data = static_cast<T265ComposeFrame *>(data[0]);

    if(Data->image_left->data != nullptr)
    {
         cv::Mat cv_image_left = cv::Mat(cv::Size(848, 800), CV_8U, Data->image_left->data, cv::Mat::AUTO_STEP);
        std::cout << "image_left Data:" << cv_image_left << std::endl;
    }

    //std::cout<<"px: "<<Data->pose_data->translation_x << std::endl;

    return;
}

int main()
{
    std::string path = "../../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("RealSenseT265");
    //OpenDevice(dev);
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
