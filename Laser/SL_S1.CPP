#include "SL_S1.h"
#include "../Smw/utility/ini_file.hpp"
#include "../Smw/utility/singleton.hpp"
#include <iostream>
#include <thread>

using namespace sl;
using namespace SMW;
using namespace std;

bool SL_lidar_S1::Init(std::string &configPath) 
{
    std::cout<<"SL_lidar_S1"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    int baudRate = (*ini)["SL_lidar_S1"]["baudRate"];
    string serial_port = (*ini)["SL_lidar_S1"]["serial_port"];
    std::cout<<"serial_port:"<<serial_port<< "  baudRate: " << baudRate << std::endl;
    
    drv = *createLidarDriver();
    if(!drv)
    {
        std::cout << "drv Create fail"<<std::endl;
        exit(-2);
    }
    _channel = *createSerialPortChannel(serial_port,baudRate);
    drv->connect(_channel);
    std::cout << "drv connected"<<std::endl;
}

int SL_lidar_S1::OpenDevice()
{
    std::cout<<"SL_Lidar OpenDevice"<<std::endl;
    if(opt_channel_type = sl::CHANNEL_TYPE_SERIALPORT)
        drv->setMotorSpeed();
        drv->startScan(0,1);
}
int SL_lidar_S1::GetFrameData(std::vector<DataBase*> &data)
{
        op_result = drv->grabScanDataHq(nodes,count);
        drv->ascendScanData(nodes,count);
         for(int i = 0 ; i<(int)count ; ++i)
        {
           printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
                    (nodes[i].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
                    (nodes[i].angle_z_q14 * 90.f) / 16384.f,
                    nodes[i].dist_mm_q2/4.0f,
                    nodes[i].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT); /* code */
        }
        
    
}



int SL_lidar_S1::CloseDevice()
{
    drv->stop();
    if(opt_channel_type = sl::CHANNEL_TYPE_SERIALPORT)
    drv->setMotorSpeed(0);
    if(drv)
    {
        delete drv;
        drv = NULL;
    }
}