#pragma once
#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"


class BRT38:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override; 
    private:
        char* device_path_;
        int serial_fd_;
        int  baudRate;
};
CLASS_LOADER_REGISTER_CLASS(BRT38,SensorDevice)


