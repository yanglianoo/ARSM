#pragma once 
#include <chrono>
#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"
#include "rplidar_sdk-master/sl_lidar.h"
#include "rplidar_sdk-master/sl_lidar_driver.h"
#include <iostream>
using namespace sl;

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

class SL_lidar_S1:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
        ~SL_lidar_S1()
        {
            delete[] drv;
        }
        
    private:
        bool inverted = false;
        double angle_compensate_multiple = 1.0;
        sl_result op_result;
        ILidarDriver * drv ;
        IChannel* _channel;
        int opt_channel_type = sl::CHANNEL_TYPE_SERIALPORT ;

        static float getAngle(const sl_lidar_response_measurement_node_hq_t& node)
        {
             return node.angle_z_q14 * 90.f / 16384.f;
        }
};
CLASS_LOADER_REGISTER_CLASS(SL_lidar_S1,SensorDevice)