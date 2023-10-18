#include "CasTouchSensor.h"
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <cstring>
#include <fcntl.h>


void CasTouchSensor::init(const char * serial)
{
    device_path_ = serial;
    serial_fd_ = -1;
}

bool CasTouchSensor::Open()
{
        while (1)
        {

            serial_fd_ = open(device_path_, O_RDWR | O_NOCTTY);
            if(serial_fd_ != -1) break;
        }
        
        // 配置串口参数
        struct termios tty;
        memset(&tty, 0, sizeof(tty));

        if (tcgetattr(serial_fd_, &tty) != 0) {
            std::cerr << "Failed to get serial port attributes." << std::endl;
            close(serial_fd_);
            return false;
        }

        // 这个波特率是固定好了吗，不需要外部传入吗？
        cfsetospeed(&tty, B57600);  // 波特率 9600
        cfsetispeed(&tty, B57600);

        tty.c_cflag &= ~PARENB;   // 无奇偶校验
        tty.c_cflag &= ~CSTOPB;   // 1位停止位
        tty.c_cflag &= ~CSIZE;    // 清除数据位设置
        tty.c_cflag |= CS8;       // 数据位设置为8位

        tty.c_cflag &= ~CRTSCTS;  // 禁用硬件流控制
        tty.c_lflag &= ~ICANON;   // 禁用规范模式
        tty.c_lflag &= ~ECHO;     // 禁用回显
        tty.c_lflag &= ~ECHOE;    // 禁用擦除字符

        if (tcsetattr(serial_fd_, TCSANOW, &tty) != 0) {
            std::cerr << "Failed to set serial port attributes." << std::endl;
            close(serial_fd_);
            return false;
        }
        return true;
}


float CasTouchSensor::ReadData()
{
    char buf[6] = {0};
    int max_size = sizeof(buf);
    int len = 0;

    memset(buf,0,max_size);
    len = read(serial_fd_,buf,max_size);
    printf("Touch value: %s\n",buf);
}

void CasTouchSensor::Close()
{
    if (serial_fd_ != -1)
    {
        std::cout<<"cas close" << std::endl;
        close(serial_fd_);
        serial_fd_ = -1;
    }   
}


