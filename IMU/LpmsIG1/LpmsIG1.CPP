#include "LpmsIG1.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
#include <iostream>
#include <thread>
using namespace SMW;
using namespace std;

static bool printThreadIsRunning=false;
bool LpmsIG1::Init(std::string &configPath) 
{
    std::cout<<"LpmsIG1"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    int baudRate = (*ini)["LpmsIG1"]["baudRate"];
    string serial_port = (*ini)["LpmsIG1"]["serial_port"];

    std::cout<<"serial_port:"<<serial_port<< "  baudRate: " << baudRate << std::endl;
    sensor1 = IG1Factory();
    sensor1->setVerbose(VERBOSE_INFO);
    sensor1->setAutoReconnectStatus(false);

    // 连接最大次数 20 次
    for(size_t i=0; i<=20; i++)
    {
        if (!sensor1->connect(serial_port, baudRate))
        {
            sensor1->release();
            std::cout<< "Error connecting to sensor"<<std::endl;
        }
        else
        {
            std::cout<<"Sensor connected"<<std::endl;
            break;
        }
    }
    return true;
}

int LpmsIG1::GetFrameData(std::vector<DataBase*> &data)
{
    data.push_back(new ImuData());
    ImuData* LpmsIG1_data = dynamic_cast<ImuData*>(data[0]);
    if (sensor1->hasImuData())
    {
        sensor1->getImuData(sd);
        LpmsIG1_data->frequency  = sensor1->getDataFrequency();
        LpmsIG1_data->accelerometer_x =  sd.accCalibrated.data[0];
        LpmsIG1_data->accelerometer_y =  sd.accCalibrated.data[1];
        LpmsIG1_data->accelerometer_z =  sd.accCalibrated.data[2];

        LpmsIG1_data->gyroscope_x = sd.gyroIAlignmentCalibrated.data[0];
        LpmsIG1_data->gyroscope_y = sd.gyroIAlignmentCalibrated.data[1];
        LpmsIG1_data->gyroscope_z = sd.gyroIAlignmentCalibrated.data[2];
    }
    
}

int LpmsIG1::OpenDevice()
{
    std::cout<<"LpmsIG1 OpenDevice"<<std::endl;
    //设置为数据流模式
    sensor1->commandGotoStreamingMode();

}

int LpmsIG1::CloseDevice()
{
    std::cout<<"LpmsIG1 CloseDevice"<<std::endl;
    sensor1->release();
}