#include "../Smw/SmwApi.h"
#include <iostream>
#include <string>
int main()
{
    std::string path = "../sensor_config.ini";
    SmwInit(const_cast<char *>(path.c_str()));
    auto dev = GetDevice("LpmsIG1");
    OpenDevice(dev);
    DataBase* data_ptr;
    DataBase* &ptr = data_ptr;
    GetFrameData(dev,ptr);
    return 0;
}
