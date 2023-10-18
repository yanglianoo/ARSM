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
#include <unistd.h>
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
struct udev *udev;
struct udev_enumerate *enumerate;
void Udev_Get(bool flag , std::string type)
{
    udev = udev_new();
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


    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, device_name);
}

std::string configPath;
SensorState ST;

int SmwInit(std::string pathFile)
{
    configPath = pathFile;
    std::cout<<"SmwInit: " <<configPath<<std::endl;
    return 0;
}

static string last_Action = "nothing";
const char * ID_Model ;
static int id_flag = 1;

SensorState Udev_Monitor(DevHandle dev)
{
    if (udev_monitor_get_fd(device_mon))
        {
            udev_device *device = udev_monitor_receive_device(device_mon);

            if (dev) 
            {
                const char *action = udev_device_get_action(device);
                if (action) 
                {
                    string Action = action;
                    if(Action == "add")
                    {
                        if(last_Action != "added") //防止多次重复open
                        {
                            //第一次插入时获取设备ID,作为唯一标识符
                            if(id_flag == 1)
                            {
                                ID_Model  = udev_device_get_property_value(device, "ID_MODEL");
                                id_flag ++;
                            }
                            std::cout<<"Sensor insertion!"<<std::endl;
                            OpenDevice(dev);  
                            last_Action = "added";
                            ST = RUNNING;
                            return ST; 
                        } 
                    }
                    else if(Action == "remove")
                    {
                        if(ST == ADD)  //防止抖动
                        {
                            return ST;
                        }
                        else
                        {
                            // 拔出时需要遍历usb设备确保拔出的是正确的传感器
                            struct udev_list_entry *devices, *entry;
                            udev_enumerate_scan_devices(enumerate);
                            devices = udev_enumerate_get_list_entry(enumerate);
                            udev_list_entry_foreach(entry, devices)
                            {
                                const char *path = udev_list_entry_get_name(entry);
                                struct udev_device *dev = udev_device_new_from_syspath(udev, path);
                                // 获取ID_MODEL的值
                                const char *model = udev_device_get_property_value(dev, "ID_MODEL");
                                if (model && strcmp(model, ID_Model) == 0)
                                {
                                    udev_device_unref(dev);
                                    ST = RUNNING;
                                    return ST;
                                }
                            }
                            //如果确认拔出是此传感器，则closedevice
                            if(last_Action != "removed") //防止多次重复close
                            {
                                    std::cout<<"The sensor has been unplugged!"<<std::endl;
                                    CloseDevice(dev);
                                    last_Action = "removed";
                                    ST = REMOVE;
                                    return ST;
                            }
                        }
                    }
                }
                else
                {
                        if(ST == ADD) 
                        {
                            OpenDevice(dev); 
                            ST = RUNNING;
                            return ST;
                        }
                        if(ST == RUNNING) return ST;
                        if(ST == REMOVE)  return ST;
                }
            udev_device_unref(device);
            }
       }
       else{
        std::cout << "debug udev" <<std::endl;
       }
}

SensorState Net_Monitor(DevHandle dev,const char* device)
{

    char path[128];
    snprintf(path, sizeof(path), "/sys/class/net/%s/carrier", device);

    FILE *file = fopen(path, "r");
    if (file) {
        char line[256];
        if (fgets(line, sizeof(line), file)) {
            int carrier = atoi(line);
            fclose(file);
            if(carrier == 1)
            {
                if(last_Action != "added") //防止多次重复open
                {
                    
                    std::cout<<"Sensor insertion!"<<std::endl;
                    OpenDevice(dev);
                    last_Action = "added";
                    ST = RUNNING;
                    return ST; 
                }
                ST = RUNNING;
                return ST; 
            }
            if(carrier == 0)
            {
                //如果确认拔出是此传感器，则closedevice
                if(last_Action != "removed") //防止多次重复close
                {
                    std::cout<<"The sensor has been unplugged!"<<std::endl;
                    CloseDevice(dev);
                    last_Action = "removed";
                    ST = REMOVE;
                    return ST;
                }
                ST = REMOVE;
                return ST;
            }
        }
    }
    fclose(file);
}

static int bit=0;
std::string device_type;
std::string net_device;
SensorState Auto_Monitor(DevHandle dev)
{
    if(bit==0)
    {
        bit++;
        IniFile * ini = Singleton<IniFile>::instance();
        ini->load(configPath);
        bool auto_flag = (*ini)["Feature configuration"]["auto_enble"];
        if(auto_flag == false)
        {
            std::cout<<"The Plug and Play feature is not enabled!"<<std::endl;
            return REMOVE;
        }
        device_type = (string)(*ini)["Feature configuration"]["device_type"];
        std::string SensorStaus = (string)(*ini)["Feature configuration"]["sensor_state"];
        if(SensorStaus == "Already_inserted") ST = ADD;
        if(SensorStaus == "Not_inserted") ST = REMOVE;

        if(device_type == string("usb"))
        {
            //初始化tty设备的即插即用，使用libudev
            std::cout<<"device: usb"<<std::endl;
            Udev_Get(auto_flag,device_type);
        }
        if(device_type == string("net"))
        {
            net_device = (string)(*ini)["Feature configuration"]["net_device"];
            std::cout<<"net_device:" << net_device << std::endl;
        }
    }
    else
    {

        if(device_type == string("usb"))
        {
            return Udev_Monitor(dev);
        }
        if(device_type == string("net"))
        {
            const char * device = net_device.c_str();
            return Net_Monitor(dev, device);
        }

    }
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
