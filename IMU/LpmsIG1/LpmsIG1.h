#pragma once 

#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include "lpmsig1opensourcelib/LpmsIG1.h"
class LpmsIG1:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~LpmsIG1()
        {
            delete[] sensor1;
        }
    private:
        IG1I* sensor1;
        IG1ImuDataI sd;
};
CLASS_LOADER_REGISTER_CLASS(LpmsIG1,SensorDevice)


