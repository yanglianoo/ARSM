#pragma once

#include <vector>
#include "DataBase.h"

typedef void *DevHandle;

    /*API接口函数*/
    int SmwInit(std::string pathFile);
    DevHandle GetDevice(char *name);
    int OpenDevice(DevHandle dev);
    int GetFrameData(DevHandle dev, std::vector<DataBase*> &data); 
    int CloseDevice(DevHandle dev);  

