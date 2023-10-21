#include "Nbit.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
#include <iostream>

using namespace SMW;

bool Nbit::Init(std::string &configPath)
{
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    string ip = (string)(*ini)["Nbit"]["ip"];
    int port = (*ini)["Nbit"]["port"];
    nbit.init(ip.c_str(),port);
}

int Nbit::OpenDevice()
{
    nbit.UDPDirectInit(0x01);
}

int Nbit::GetFrameData(std::vector<DataBase *> &data)
{
    if(data.size() <= 1 )
    {
        data.push_back(new PowerData());
    }
    PowerData* power_data = dynamic_cast<PowerData*>(data[0]);
    nbit.UDP_GetData();
    nbit.calc_resault();
    power_data->frequency = 10;
    power_data->fx = nbit.Sum[0];
    power_data->fy = nbit.Sum[1];
    power_data->fz = nbit.Sum[2];
    power_data->Mx = nbit.Sum[3];
    power_data->My = nbit.Sum[4];
    power_data->Mz = nbit.Sum[5]; 
}

int Nbit::CloseDevice()
{
    nbit.sensor_close();
}