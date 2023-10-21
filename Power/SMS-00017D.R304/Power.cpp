#include "Power.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
#include <iostream>

using namespace SMW;

bool Power::Init(std::string &configPath)
{
    // std::cout<<"Power Sensor"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    string serial_port = (*ini)["Power"]["serial_port"];

    power.Init(serial_port.c_str());
}

int Power::OpenDevice()
{
    power.OpenDevice();
}

int Power::GetFrameData(std::vector<DataBase *> &data)
{
    if(data.size() <= 1 )
        {
            data.push_back(new PowerData());
        }
        PowerData* power_data = dynamic_cast<PowerData*>(data[0]);
        float Powerdata[6];
        power.ReadData(Powerdata);
        power_data->frequency = 10;
        power_data->fx = Powerdata[0];
        power_data->fy = Powerdata[1];
        power_data->fz = Powerdata[2];
        power_data->Mx = Powerdata[3];
        power_data->My = Powerdata[4];
        power_data->Mz = Powerdata[5];  
        // std::cout<<"powerdata:"<<power_data->fx<<std::endl<<power_data->fy<<std::endl<<power_data->fz<<std::endl<<power_data->Mx<<std::endl;
}

int Power::CloseDevice()
{
    power.CloseDevice();
}