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
        AbstractClassFactory<Base>* factory = static_cast<AbstractClassFactory<Base>*>(it->second);
        if (factory) {
            return factory->CreateObj();
        }
    }
    return nullptr;
}

class LpmsIG1 : public SensorDevice {
public:
    bool Init(std::string &configPath) override {
        std::cout<<configPath<<std::endl;
    }

    int GetFrameData(DataBase *data) override
    {
        std::cout<<"get LpmsIG1 data"<<std::endl;
    }
    int OpenDevice()  override
    {
        std::cout<<"OpenDevice:LpmsIG1"<<std::endl;
    }

    int CloseDevice() override
    {
        std::cout<<"CloseDevice:LpmsIG1"<<std::endl;
    }

};
CLASS_LOADER_REGISTER_CLASS(LpmsIG1,SensorDevice)

std::string configPath;
int SmwInit(char *pathFile)
{
    configPath = pathFile;

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

int GetFrameData(DevHandle dev, DataBase* &data)
{
    SensorDevice* devicePtr = static_cast<SensorDevice*>(dev);
    devicePtr->GetFrameData(data);
}

