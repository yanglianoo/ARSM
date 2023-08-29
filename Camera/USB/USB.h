#pragma once 

#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/video.hpp>


class USB:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~USB()
        {
            // TODO
        }
    private:
        std::string device;
        cv::VideoCapture cap;
        cv::Mat frame;
};
CLASS_LOADER_REGISTER_CLASS(USB,SensorDevice)


