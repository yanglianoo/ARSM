#include "D435i.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
#include <iostream>

using namespace std;
using namespace SMW;
using namespace rs2;

bool RealSenseD435i::Init(std::string &configPath)
{
    cout << "RealSense D534i Init" << endl; 
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    int idx = (*ini)["RealSenseD435i"]["idx"];
    bool enable_depth = (*ini)["RealSenseD435i"]["enable_depth"];
    enable_depth_ = enable_depth;
    // bool is_show = (*ini)["RealSenseD435i"]["is_show"];
    // is_show_ = is_show;

    cout << "Config: RealSenseD435i Device ID is " << idx << endl;
    cout << "Config: Whether depth information is required? " << enable_depth << endl;
    // cout << "Config: Whether you need to visualize data? " << is_show << endl;

    auto list = ctx.query_devices();
    if (list.size() == 0)
    {
        throw std::runtime_error("No device detected. Is it plugged in?");
        return false;
    }
    rs2::device dev = list[idx];

    cout << "Using device " + to_string(idx) + ": " <<  dev.get_info(RS2_CAMERA_INFO_NAME) << endl;
    cout << "Serial number: " <<  dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) << endl;
    cout << "Firmware version: " <<  dev.get_info(RS2_CAMERA_INFO_FIRMWARE_VERSION) << endl;
    cout << "Physical port: " <<  dev.get_info(RS2_CAMERA_INFO_PHYSICAL_PORT) << endl;

    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    if (enable_depth)
    {
        cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    }

    return true;
}

int RealSenseD435i::GetFrameData(std::vector<DataBase*> &data)
{
    frames = pipe.wait_for_frames();

    // 保存RGB数据
    if (data.size() < 1)
        data.push_back(new ImageData());
    ImageData* RgbData = dynamic_cast<ImageData*>(data[0]);
    rs2::frame rgb_frame = frames.get_color_frame();
    RgbData->type = rgb;
    RgbData->ptr = (void*)rgb_frame.get_data();


    // 保存深度数据
    if (enable_depth_)
    {   
        if (data.size() < 2) 
            data.push_back(new ImageData());
        ImageData* DepthData = dynamic_cast<ImageData*>(data[1]);
        rs2::frame depth_frame = frames.get_depth_frame();
        DepthData->type = depth;
        DepthData->ptr = (void*)depth_frame.get_data();
    }

    
    // frames = pipe.wait_for_frames();
    // rs2::frame color_frame = frames.get_color_frame();
    // colorMat = cv::Mat(cv::Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);

    // if (enable_depth_){
    //     rs2::frame depth_frame = frames.get_depth_frame();
    //     depthMat = cv::Mat(cv::Size(640, 480), CV_16UC1, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);

    // }
    return 1;
}

int RealSenseD435i::OpenDevice()
{
    cout << "RealSenseD435 OpenDevice" << endl;
    pipe.start(cfg);
    return 1;

}

int RealSenseD435i::CloseDevice()
{
    cout << "RealSenseD435 CloseDevice" << std::endl;
    return 1;
}
