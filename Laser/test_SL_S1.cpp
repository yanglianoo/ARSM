#include "../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
void PrintComposeframe(DataBase *data)
{
    ImuData *frameData = static_cast<ImuData *>(data);
    std::cout << "frequency: " << frameData->frequency << " ";
    std::cout << "gyroscope_x: " << frameData->gyroscope_x <<" ";
    std::cout << "gyroscope_y: " << frameData->gyroscope_y <<" ";
    std::cout << "gyroscope_z: " << frameData->gyroscope_z <<" ";
    std::cout << "accelerometer_x: " <<frameData->accelerometer_x << " ";
    std::cout << "accelerometer_y: " <<frameData->accelerometer_y << " ";
    std::cout << "accelerometer_z: " <<frameData->accelerometer_z << " ";
    std::cout << std::endl;
    return;
}
int main()
{
    std::string path = "../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("SL_lidar_S1");
    OpenDevice(dev);
    std::vector<DataBase*> data;   
    while (1)
    {
        GetFrameData(dev,data);     
    }
    
    
    return 0;
}