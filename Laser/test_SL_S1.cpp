#include "../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
void PrintComposeframe(DataBase *data)
{
    LidarScan *frameData = static_cast<LidarScan *>(data);
    std::cout << "frame_id: " << frameData->frame_id << " ";
    std::cout << "angle_min: " << frameData->angle_min <<" ";
    std::cout << "angle_max: " << frameData->angle_max <<" ";
    std::cout << "range_min: " << frameData->range_min <<" ";
    std::cout << "range_max: " <<frameData->range_max << " ";
    for(int i = 1;i<frameData->ranges.size();i++){
        std::cout << "ranges: " <<frameData->ranges[i] << " ";
    }
    for(int j = 1;j<frameData->intensities.size();j++){
    std::cout << "intensities: " <<frameData->intensities[j] << " ";
    }
    std::cout << std::endl;
    return;
    return;
}
int main()
{
    std::string path = "../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("SL_lidar_S1");
    std::vector<DataBase*> data;   
    while (1)
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
