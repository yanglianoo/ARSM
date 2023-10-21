#include "../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>

void PrintComposeframe(DataBase *data)
{
    SpeedData *frameData = static_cast<SpeedData *>(data);
    std::cout << "frequency: " << frameData->frequency << " ";
    std::cout << "Speed sensor value: " <<frameData->n << " ";
    std::cout << std::endl;
    return;
}
int main()
{
    std::string path = "../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("OID");
    std::vector<DataBase*> data;
    while(1)
    {
        SensorState ret =  Auto_Monitor(dev);

        if(ret == RUNNING)
       {
            GetFrameData(dev,data);
           PrintComposeframe(data[0]);
        }
    }
    return 0;
}