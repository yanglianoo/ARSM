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
        char* device_path_;
        int serial_fd_;
        int  baudRate;
        
};
CLASS_LOADER_REGISTER_CLASS(Gps,SensorDevice)
