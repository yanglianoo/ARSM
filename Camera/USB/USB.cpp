#include "USB.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
#include <iostream>

using namespace SMW;
using namespace cv;

bool USB::Init(std::string &configPath)
{
    std::cout << "HiKVision Init" << std::endl; 
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    device  = (string)(*ini)["USB"]["video"];
    std::cout << "Config: USBCamera Device ID is " << device << std::endl;

    return true;
}

int USB::GetFrameData(std::vector<DataBase*> &data)
{
    if (data.size() < 1)
        data.push_back(new ImageData());
    ImageData* Data = static_cast<ImageData*>(data[0]);
    
    
    if (cap.isOpened())
    {
        cap.read(frame);
        Data->data = frame.data;
        Data->width = frame.cols;
        Data->height = frame.rows;
        Data->channelNum = frame.channels();
    }
}

int USB::OpenDevice()
{
    std::cout << "USBCamera OpenDevice" << std::endl;
    cap.open(device);
}

int USB::CloseDevice()
{
    std::cout << "USBCamera CloseDevice" << std::endl;
    cap.release();
}
