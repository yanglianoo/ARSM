#include "cas.h"
#include <iostream>

#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
 
using namespace SMW;

bool Cas::Init(std::string &configPath)
{
    std::cout<<"Cas Touch Sensor"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    string serial_port = (*ini)["Cas"]["serial_port"];

    cas.init(serial_port.c_str());

}

int Cas::GetFrameData(std::vector<DataBase *> &data)
{
    //暂时先不定义力矩传感器的数据结构，用imu的代替
    if(data.size() <= 1 )
    {
        data.push_back(new ImuData());
    }
    ImuData* cas_data = dynamic_cast<ImuData*>(data[0]);
    cas_data->accelerometer_x = cas.ReadData();
}


int Cas::OpenDevice()
{
    cas.Open();
}

int Cas::CloseDevice()
{
    cas.Close();
}

