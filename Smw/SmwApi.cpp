#include "SmwApi.h"
#include <unordered_map>
#include <iostream>
#include "utility/ClassFactory.hpp"
#include "utility/ini_file.hpp"
#include "utility/system.hpp"
#include "utility/singleton.hpp"
#include "utility/logger.hpp"
#include "SensorDevice.h"
using namespace SMW;
// 全局 unordered_map，用于存储类工厂对象
std::unordered_map<std::string, AbstractClassFactoryBase*> classFactories;

// 注册类工厂的函数
void RegisterClassFactory(const std::string& className, AbstractClassFactoryBase* factory) {
    classFactories[className] = factory;
}

// 创建对象的函数
template <typename Base>
Base* CreateObject(const std::string& className) {
    auto it = classFactories.find(className);
    if (it != classFactories.end()) {
        std::cout<<"CreateObj success" <<std::endl;
        AbstractClassFactory<Base>* factory = static_cast<AbstractClassFactory<Base>*>(it->second);
        if (factory) {
            return factory->CreateObj();
        }
    }
    return nullptr;
}



std::string configPath;
int SmwInit(std::string pathFile)
{
    configPath = pathFile;
    std::cout<<"SmwInit:" <<configPath<<std::endl;


}
DevHandle GetDevice(char *name)
{
    SensorDevice * newimu = CreateObject<SensorDevice>(name);
    return newimu;
}

int OpenDevice(DevHandle dev)
{
    SensorDevice* devicePtr = static_cast<SensorDevice*>(dev);
    devicePtr->Init(configPath);
}

int GetFrameData(DevHandle dev, std::vector<DataBase *> &data)
{
    SensorDevice* devicePtr = static_cast<SensorDevice*>(dev);
    devicePtr->GetFrameData(data);
}

