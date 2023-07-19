/**
 * @File Name: SmwApi.cpp
 * @brief  
 * @Author : Timer email:330070781@qq.com
 * @Version : 1.0
 * @Creat Date : 2023-07-15
 * 
 */
#include "SmwApi.h"
#include <unordered_map>
#include <iostream>
#include "utility/ClassFactory.hpp"
#include "utility/ini_file.hpp"
#include "utility/system.hpp"
#include "utility/singleton.hpp"
#include "utility/logger.hpp"
#include "SensorDevice.h"
#include <libudev.h>
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




struct udev_monitor * device_mon;

void Udev_Get(bool flag , std::string type)
{
    udev *udev = udev_new();
    if (!udev) {
        std::cout<<"Failed to create udev.\n"<<std::endl;
    }
    // 设备监听器
    device_mon = udev_monitor_new_from_netlink(udev, "udev");
    if (!device_mon) {
        std::cout<<"Failed to create udev monitor \n"<<std::endl;
        udev_unref(udev);
    }

    const char* device_name =  type.c_str();
    udev_monitor_filter_add_match_subsystem_devtype(device_mon, device_name, NULL);
    udev_monitor_enable_receiving(device_mon);
}

std::string configPath;
int SmwInit(std::string pathFile)
{
    configPath = pathFile;
    std::cout<<"SmwInit:" <<configPath<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    bool auto_flag = (*ini)["Feature configuration"]["auto_enble"];
    if(auto_flag == false)
    {
        return 0;
    }
    std::string device_type = (string)(*ini)["Feature configuration"]["device_type"];

    Udev_Get(auto_flag,device_type);

}

static int flag = 2;

int Auto_Monitor(DevHandle dev)
{
        // 视频设备
    if (udev_monitor_get_fd(device_mon)) 
        {
            udev_device *device = udev_monitor_receive_device(device_mon);
            if (dev) 
            {
                const char *action = udev_device_get_action(device);
                if (action) 
                {
                    string Action = udev_device_get_action(device);
                    if(Action == "add")
                    {
                        std::cout<<"Sensor insertion!"<<std::endl;
                        OpenDevice(dev);   
                        flag = 1;
                    }
                    else if(Action == "remove")
                    {
                        std::cout<<"The sensor has been unplugged!"<<std::endl;
                        CloseDevice(dev);
                        flag =  0;
                    }
                }
            udev_device_unref(device);
            }
       }

    return flag;
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
    devicePtr->OpenDevice();
}

int GetFrameData(DevHandle dev, std::vector<DataBase *> &data)
{
    SensorDevice* devicePtr = static_cast<SensorDevice*>(dev);
    devicePtr->GetFrameData(data);
}
int CloseDevice(DevHandle dev)
{

    SensorDevice* devicePtr = static_cast<SensorDevice*>(dev);
    devicePtr->CloseDevice();
}
