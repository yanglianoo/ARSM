#pragma once

#include <string>
#include "DataBase.h"
#include <typeinfo>
#include <vector>
class SensorDevice
{
public:
    virtual ~SensorDevice() {}
    virtual bool Init(std::string &configPath) = 0;
    virtual int GetFrameData(std::vector<DataBase *> &data)=0;
    virtual int OpenDevice() = 0;
    virtual int CloseDevice() = 0;

};


