#include "D435.h"
#include "../../../Smw/utility/ini_file.hpp"
#include "../../../Smw/utility/singleton.hpp"
#include <iostream>

using namespace SMW;
using namespace rs2;


bool RealSenseD435::Init(std::string &configPath)
{
    std::cout << "RealSense D435 Init" << std::endl; 
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    int idx = (*ini)["RealSenseD435"]["idx"];
    enable_depth_ = (*ini)["RealSenseD435"]["enable_depth"];
    width_ = (*ini)["RealSenseD435"]["width"];
    height_ = (*ini)["RealSenseD435"]["height"];

    std::cout << "Config: RealSenseD435 Device ID is " << idx << std::endl;
    std::cout << "Config: Whether depth information is required? " << enable_depth_ << std::endl;
    std::cout << "Config: Data Width: " << width_ << std::endl;
    std::cout << "Config: Data Height: " << height_ << std::endl;

    return true;
}

int RealSenseD435::GetFrameData(std::vector<DataBase*> &data)
{
    // 获取数据帧
    rs2::frameset frames = pipe.wait_for_frames();

    // 构建D435数据结构
    if (data.size() < 1)
        data.push_back(new D435ComposeFrame());

    D435ComposeFrame* D435 = dynamic_cast<D435ComposeFrame*>(data[0]);

    // 保存color数据
    if (D435->m_color_frame == nullptr)
        D435->m_color_frame = new ImageData();
    color_frame = frames.get_color_frame();
    D435->m_color_frame->data = (void*)color_frame.get_data();
    D435->m_color_frame->width = width_;
    D435->m_color_frame->height = height_;
    D435->m_color_frame->size = height_ * width_;

    // 保存depth数据
    if (enable_depth_)
    {   
        if (D435->m_depth_frame == nullptr){
            D435->m_depth_frame = new DepthData();
        }
       
        depth_frame = frames.get_depth_frame();

        D435->m_depth_frame->data = (void*)depth_frame.get_data();
        D435->m_depth_frame->width = width_;
        D435->m_depth_frame->height = height_;
        D435->m_depth_frame->size = height_ * width_;

    }

    // 用于释放设备资源
   ctx.query_devices();
}

int RealSenseD435::OpenDevice()
{

    std::cout << "RealSenseD435 OpenDevice" << std::endl;
    cfg.enable_stream(RS2_STREAM_COLOR, width_, height_, RS2_FORMAT_BGR8, 30);
    if (enable_depth_)
    {   
        cfg.enable_stream(RS2_STREAM_DEPTH, width_, height_, RS2_FORMAT_Z16, 30);
    }

    pipe.start(cfg);
}

int RealSenseD435::CloseDevice()
{
    std::cout << "RealSenseD435 CloseDevice" << std::endl;
    pipe.stop();
}
