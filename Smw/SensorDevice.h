/**
 * @File Name: SensorDevice.h
 * @brief  
 * @Author : Timer email:330070781@qq.com
 * @Version : 1.0
 * @Creat Date : 2023-07-15
 * 
 */

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


