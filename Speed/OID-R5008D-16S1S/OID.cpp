#include <iostream>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "OID.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
using namespace SMW;


/**************************速度*****************************************/
#define SUDU_Buffer_Length 60
typedef struct SaveData
{
        char SUDU_Buffer[SUDU_Buffer_Length];
        char   order[8]; 
        char buf[5] = { 0 };
        int Data;
        int  pos = 0;
        float n = 0;  //转速
} _SaveData;
_SaveData Save_Data;



/* 返回ch字符在sign数组中的序号 */
int getIndexOfSigns(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    return -1;

}
/* 十六进制数转换为十进制数 */
long hexToDec(char* source)
{
    long sum = 0;
    long t = 1;
    int i, len;

    len = (int)strlen(source);
    for (i = len - 1; i >= 0; i--)
    {
        sum += t * getIndexOfSigns(*(source + i));
        t *= 16;
    }

    return sum;
}

void idle_init()
{
    // Save_Data.order[0] = 0x01;
    // Save_Data.order[1] = 0x03;
    // Save_Data.order[2] = 0x00;
    // Save_Data.order[3] = 0x20;
    // Save_Data.order[4] = 0x00;
    // Save_Data.order[5] = 0x02;
    // Save_Data.order[6] = 0xC5;
    // Save_Data.order[7] = 0xC1;

        Save_Data.order[0] = 0x01;
        Save_Data.order[1] = 0x03;
        Save_Data.order[2] = 0x00;
        Save_Data.order[3] = 0x03;
        Save_Data.order[4] = 0x00;
        Save_Data.order[5] = 0x01;
        Save_Data.order[6] = 0x74;
        Save_Data.order[7] = 0x0A;
}

bool OID::Init(std::string &configPath)
{
    std::cout<<"OID Speed Sensor"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    string serial_port = (*ini)["OID"]["serial_port"];
    baudRate = (*ini)["OID"]["baudRate"];
    std::cout<<"serial_port:" << serial_port <<std::endl;
    device_path_ = new char[serial_port.length() + 1];
    std::strcpy(device_path_,serial_port.c_str());

    serial_fd_ = -1;
    idle_init();
    return true;
}


int OID::GetFrameData(std::vector<DataBase *> &data)
{
    if(data.size() <= 1 )
    {
        data.push_back(new SpeedData());
    }
    SpeedData* speed_data = dynamic_cast<SpeedData*>(data[0]);
    int ret = write(serial_fd_,Save_Data.order,8);
    if(ret < 0)
    {
        printf("write fail\r\n");
    }
    bzero(Save_Data.SUDU_Buffer,sizeof(Save_Data.SUDU_Buffer));
    ret = read(serial_fd_,Save_Data.SUDU_Buffer,10);
    if(ret < 0)
    {
        printf("read fail\r\n");
    }
    for(int i = 0;i < ret;i++)
    {
        if(Save_Data.SUDU_Buffer[i] == 0x03 && Save_Data.SUDU_Buffer[i + 1] == 0x02)
        {
            Save_Data.pos = i + 1;
            break;
        }
    }
    //sprintf(Save_Data.buf, "%x%x%x%x",Save_Data.SUDU_Buffer[ Save_Data.pos + 1],Save_Data.SUDU_Buffer[ Save_Data.pos + 2],Save_Data.SUDU_Buffer[ Save_Data.pos +3],Save_Data.SUDU_Buffer[ Save_Data.pos + 4]);
     sprintf(Save_Data.buf, "%x%x",Save_Data.SUDU_Buffer[ Save_Data.pos + 1],Save_Data.SUDU_Buffer[ Save_Data.pos + 2]);

    
    Save_Data.Data =  hexToDec(Save_Data.buf);

    speed_data->frequency = 10;
    speed_data->n = (float)Save_Data.Data*0.00915;
    usleep(100000);
}


int OID::OpenDevice()
{

   /*第一步，串口初始化*/
    while (1)
        {
            serial_fd_ = open(device_path_,O_RDWR | O_NOCTTY | O_NDELAY);
            if(serial_fd_ != -1) 
            break;
        }
    
    struct termios new_cfg={0}; //用于保存终端新配置的参数
    speed_t speed;		//定义波特率
    switch (baudRate) {
            case 1200: speed = B1200;
                break;
            case 1800: speed = B1800;
                break;
            case 2400: speed = B2400;
                break;
            case 4800: speed = B4800;
                break;
            case 9600: speed = B9600;
                break;
            case 19200: speed = B19200;
                break;
            case 38400: speed = B38400;
                break;
            case 57600: speed = B57600;
                break;
            case 115200: speed = B115200;
                break;
            case 230400: speed = B230400;
                break;
            case 460800: speed = B460800;
                break;
            case 500000: speed = B500000;
                break;
            default:    //默认配置为115200
                speed = B115200;
            break;
    }
    	/*第二步，配置串口参数*/
	cfmakeraw(&new_cfg);//设置为原始模式 
	new_cfg.c_cflag |= CREAD;// 使能接收 
	cfsetspeed(&new_cfg, speed);//将波特率设置为9600
	new_cfg.c_cflag &= ~CSIZE; //将数据位相关的比特位清零
	new_cfg.c_cflag |= CS8;    //将数据位数设置为8位
	new_cfg.c_cflag &= ~PARENB;
	new_cfg.c_iflag &= ~INPCK;//设置为无校验模式
	new_cfg.c_cflag &= ~CSTOPB;//将停止位设置为1位
	new_cfg.c_cc[VTIME] = 0;// 将 MIN 和 TIME 设置为 0
	new_cfg.c_cc[VMIN] = 0;
	int ret = tcflush(serial_fd_, TCIOFLUSH);//清空缓冲区域

    if(ret < 0)
    {
		printf("tcflush error\n");
		return -1;
	}
	ret = tcsetattr(serial_fd_, TCSANOW, &new_cfg);//写入配置、使配置生效
	if(ret < 0){
		printf("tcsetattr error\n");
		return -1;
	}
}

int OID::CloseDevice()
{
    if (serial_fd_ != -1)
    {
        std::cout<<"OID close" << std::endl;
        close(serial_fd_);
        serial_fd_ = -1;
    }   
    return 0;
}