#pragma once

#include <vector>
#include "DataBase.h"

typedef void *DevHandle;
/*API接口函数*/
int SmwInit(char *pathFile);
DevHandle GetDevice(char *name);
int OpenDevice(DevHandle dev);
int GetFrameData(DevHandle dev, DataBase* &data); 
int CloseDevice(DevHandle dev);  