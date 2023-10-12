#include "Speed.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cstring>
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"

using namespace SMW;

/***************************调试信息控制*********************************/
// #define __Debug_info

/**************************速度*****************************************/
#define SPEED_Buffer_Length 60
typedef struct SaveData
{
    char SPEED_Buffer[SPEED_Buffer_Length];
    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
    } raw;
    float x;
    float y;
    float z;
} _SaveData;
_SaveData Save_Data;

speed_t speed = B9600;        // 定义波特率为9600

bool Speed::Init(std::string &configPath)
{
    std::cout << "Speed Sensor" << std::endl;
    std::cout << "configPath:" <<configPath <<std::endl;
    IniFile *ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    string serial_port = (*ini)["Speed"]["serial_port"];
    baudRate = (*ini)["Speed"]["baudRate"];
    std::cout<<"serial_port:" << serial_port <<std::endl;
    device_path_ = new char[serial_port.length() + 1];
    std::strcpy(device_path_,serial_port.c_str());
    serial_fd_ = -1;
    return true;
}

int Speed::GetFrameData(std::vector<DataBase *> &data)
{
        int ret = read(serial_fd_, Save_Data.SPEED_Buffer, sizeof(Save_Data.SPEED_Buffer) - 1);
        Save_Data.SPEED_Buffer[ret] = 0;
        if (ret < 0)
        {
            printf("read failed\n");
        }
        else if (ret > 0)
        {
            // 查找关键数据位置
            int pos = 0;
            int i = 0;
            for (i = 0; i < SPEED_Buffer_Length; i++)
            {
                if (Save_Data.SPEED_Buffer[i] == 0x55 && Save_Data.SPEED_Buffer[i + 1] == 0x52)
                {
                    printf("获取到关键数据帧\r\n");
                    pos = i + 2;

                                // 计算角速度
            Save_Data.raw.x = (int16_t)(Save_Data.SPEED_Buffer[pos + 1] << 8) | Save_Data.SPEED_Buffer[pos];
            pos = pos + 2;
            Save_Data.raw.y = (int16_t)(Save_Data.SPEED_Buffer[pos + 1] << 8) | Save_Data.SPEED_Buffer[pos];
            pos = pos + 2;
            Save_Data.raw.z = (int16_t)(Save_Data.SPEED_Buffer[pos + 1] << 8) | Save_Data.SPEED_Buffer[pos];

            Save_Data.x = (float)Save_Data.raw.x / 32768 * 2000;
            Save_Data.y = (float)Save_Data.raw.y / 32768 * 2000;
            Save_Data.z = (float)Save_Data.raw.z / 32768 * 2000;

            printf("Gx: %.02f Gy: %.02f Gz: %.02f\r\n", Save_Data.x, Save_Data.y, Save_Data.z);
                    break;
                }
            }
        }
    return 0;
}

int Speed::OpenDevice()
{
    printf("device_path_ open device:%s\n",device_path_);
    /*第一步，串口初始化*/
    while (1)
    {
        //printf("device_path_:%s\n",device_path_);
        serial_fd_ = open(device_path_, O_RDWR | O_NOCTTY | O_NDELAY ); // O_NOCTTY 标志，告知系统该节点不会成为进程的控制终端
       // printf("debug!!!");
        if (serial_fd_ != -1)
            break;
    }
    struct termios new_cfg;
    memset(&new_cfg, 0, sizeof(new_cfg));
    /*第二步，配置串口参数*/
    cfmakeraw(&new_cfg);         // 设置为原始模式
    new_cfg.c_cflag |= CREAD;    // 使能接收
    cfsetspeed(&new_cfg, speed); // 将波特率设置为9600
    new_cfg.c_cflag &= ~CSIZE;   // 将数据位相关的比特位清零
    new_cfg.c_cflag |= CS8;      // 将数据位数设置为8位
    new_cfg.c_cflag &= ~PARENB;
    new_cfg.c_iflag &= ~INPCK;  // 设置为无校验模式
    new_cfg.c_cflag &= ~CSTOPB; // 将停止位设置为1位
    new_cfg.c_cc[VTIME] = 0;    // 将 MIN 和 TIME 设置为 0
    new_cfg.c_cc[VMIN] = 0;
    int ret = tcflush(serial_fd_, TCIOFLUSH); // 清空缓冲区域

    if (ret < 0)
    {
        printf("tcflush error\n");
        return -1;
    }
    ret = tcsetattr(serial_fd_, TCSANOW, &new_cfg); // 写入配置、使配置生效
    if (ret < 0)
    {
        printf("tcsetattr error\n");
        return -1;
    }

    printf("open\n");
    return 0;
}

int Speed::CloseDevice()
{  
    if (serial_fd_ != -1)
    { 
        std::cout << "speed close" << std::endl;
        close(serial_fd_);
        serial_fd_ = -1;
    }

    return 0;
}
