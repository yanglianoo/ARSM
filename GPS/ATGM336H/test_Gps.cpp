#include "../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>

void PrintComposeframe(DataBase *data)
{
    ImuData *frameData = static_cast<ImuData *>(data);
    std::cout << "Gps sensor value: " <<frameData->accelerometer_x << " ";
    std::cout << std::endl;
    return;
}
int main()
{
    std::string path = "../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("Gps");
    std::vector<DataBase*> data;
    while(1)
    {
        SensorState ret =  Auto_Monitor(dev);

        if(ret == RUNNING)
        {
            GetFrameData(dev,data);
           // PrintComposeframe(data[0]);
        }
    }
    return 0;
}
