#pragma once
#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include <iostream>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

class OID:public SensorDevice
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
CLASS_LOADER_REGISTER_CLASS(OID,SensorDevice)


