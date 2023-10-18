#pragma once
#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include "LIJU.h"
#include <iostream>
#include <stdio.h>


class Nbit : public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
    private:
        LIJU nbit;

};
CLASS_LOADER_REGISTER_CLASS(Nbit,SensorDevice)

