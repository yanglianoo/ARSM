#pragma once 

#include "../../../Smw/SensorDevice.h"
#include "../../../Smw/utility/ClassFactory.hpp"
#include "../realsenseopensourcelib/librealsense2/rs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


class RealSenseD435:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~RealSenseD435()
        {
            // TODO
        }
    private:
        bool enable_depth_ = false;
        int width_ = 0;
        int height_ = 0;

        rs2::pipeline pipe;
        rs2::config cfg;
        rs2::pipeline_profile profile;
        rs2::context ctx;
        rs2::frame color_frame;
        rs2::frame depth_frame;
        // rs2::frameset frames;
};

CLASS_LOADER_REGISTER_CLASS(RealSenseD435, SensorDevice)


