#pragma once
#define _GNU_SOURCE     //在源文件开头定义_GNU_SOURCE宏
#include <iostream>
#include <functional>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include "../../Smw/DataBase.h"

#define SERIAL_BUFFER_LEN_1         29

typedef struct SaveData
{
    unsigned int fx;//x轴力
    unsigned int fy;//y轴力
    unsigned int fz;//z轴力
    unsigned int Mx;//x轴力矩
    unsigned int My;//y轴力矩
    unsigned int Mz;//z轴力矩

} _SaveData;


typedef struct uart_hardware_cfg {
    unsigned int baudrate;      /* 波特率 */
    unsigned char dbit;         /* 数据位 */
    char parity;                /* 奇偶校验 */
    unsigned char sbit;         /* 停止位 */
} uart_cfg_t;
class PowerSensor
{
    friend  void io_handler(int sig, siginfo_t *info, void *context);
    public:
        void Init(const char * serial);
        bool OpenDevice();
        int ReadData(float data[6]);
        void CloseDevice();
        void LIJU_data_printf(unsigned char *buffer, int len);  
        int uart_cfg(const uart_cfg_t *cfg);
        ~PowerSensor()
        {
            delete [] device_path_;
        }
    private:
        _SaveData Save_Data;
        char device_path_[50];
        int serial_fd_;      
        struct termios old_cfg;  //用于保存终端的配置参数
        uart_cfg_t cfg = {0};
        void Compon_data(unsigned int* desc,unsigned char* buffer,int i);
        float hex_flo(unsigned int hvalue)
        {
            unsigned int temp = hvalue;
            float fvalue;
            fvalue = *(float*)&temp;
            return fvalue;
        }
        
};
