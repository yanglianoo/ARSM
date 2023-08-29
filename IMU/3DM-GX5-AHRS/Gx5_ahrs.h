#pragma once 

#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include "c++-mscl/source/mscl/mscl.h"
class GX5_AHRS:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~GX5_AHRS()
        {
            
        }
    private:
        mscl::Connection connection;
        mscl::InertialNode node;
};
CLASS_LOADER_REGISTER_CLASS(GX5_AHRS,SensorDevice)
