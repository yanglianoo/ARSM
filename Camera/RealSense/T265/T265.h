#pragma once 

#include "../../../Smw/SensorDevice.h"
#include "../../../Smw/utility/ClassFactory.hpp"
#include "../realsenseopensourcelib/librealsense2/rs.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>


class RealSenseT265:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~RealSenseT265()
        {
            // TODO
        }
    private:

        rs2::pipeline pipe;
        rs2::config cfg;
        rs2::context ctx;
        rs2::frameset dataframe;
        rs2::frame image_left;
        rs2::frame image_right ;

};

CLASS_LOADER_REGISTER_CLASS(RealSenseT265, SensorDevice)


