#include "LpmsIG1.h"
#include "../Smw/utility/ini_file.hpp"
#include "../Smw/utility/singleton.hpp"
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
    if (!sensor1->connect(serial_port, baudRate))
    {
        sensor1->release();
        std::cout<< "Error connecting to sensor"<<std::endl;
    }
    // do
    // {
    //     std::cout<<"Waiting for sensor to connect"<<std::endl;
    //     this_thread::sleep_for(chrono::milliseconds(1000));
    // } while (
    //     !(sensor1->getStatus() == STATUS_CONNECTED) && 
    //     !(sensor1->getStatus() == STATUS_CONNECTION_ERROR)
    // );
    // if (sensor1->getStatus() != STATUS_CONNECTED)
    // {
    //     std::cout<<"Sensor connection error status: "<< sensor1->getStatus()<<std::endl;
    //     sensor1->release();
    //     std::cout<<"bye"<<std::endl;
    // }
    // std::cout<<"Sensor connected"<<std::endl;
    // if (sensor1->getStatus() != STATUS_CONNECTED)
    // {
    //     printThreadIsRunning = false;
    //     std::cout<<"Sensor is not connected. Sensor Status: "<<sensor1->getStatus()<<std::endl;
    //     std::cout<<"printTask terminated"<<std::endl;
    //     return false;
    // }
    // printThreadIsRunning = false;
    // std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    // return true;
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
        std::cout<<"frequency:" <<sensor1->getDataFrequency() <<std::endl;
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