#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include "PowerDevice.h"
#include <iostream>
#include <stdio.h>



class Power : public SensorDevice
{
    public:
     bool Init(std::string &configPath)  override;
     int GetFrameData(std::vector<DataBase *> &data) override ;
     int OpenDevice() override;
     int CloseDevice() override;
     PowerSensor& getPowerSensor() {
            return power;
        }
    private:
    PowerSensor power;
    
};
CLASS_LOADER_REGISTER_CLASS(Power,SensorDevice)