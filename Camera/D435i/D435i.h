#pragma once 

#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include "realsenseopensourcelib/librealsense2/rs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/imgcodecs.hpp>


class RealSenseD435i:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~RealSenseD435i()
        {
            // TODO
            // delete[] sensor1;
        }
    private:
        bool enable_depth_ = false;
        bool is_show_ = false;
        rs2::pipeline pipe;
        rs2::frameset frames;
        rs2::context ctx;
        rs2::config cfg;
        // cv::Mat colorMat,depthMat;
};
CLASS_LOADER_REGISTER_CLASS(RealSenseD435i,SensorDevice)


