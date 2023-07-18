/**
 * @File Name: SmwApi.h
 * @brief  
 * @Author : Timer email:330070781@qq.com
 * @Version : 1.0
 * @Creat Date : 2023-07-15
 * 
 */
#pragma once

#include <vector>
#include "DataBase.h"

typedef void *DevHandle;
/*Smw API接口函数*/
int SmwInit(std::string pathFile);
DevHandle GetDevice(char *name);
int OpenDevice(DevHandle dev);
int GetFrameData(DevHandle dev, std::vector<DataBase*> &data); 
int CloseDevice(DevHandle dev); 
int Auto_Monitor(DevHandle dev);

