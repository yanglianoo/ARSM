#pragma once
#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"

class Gps:public SensorDevice
{

     public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override; 
        ~Gps()
        {
            delete [] device_path_;
        }
    private:
        const char* device_path_;
        int serial_fd_;
        
};
CLASS_LOADER_REGISTER_CLASS(Gps,SensorDevice)
