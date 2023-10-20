#include "Gps.h"
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

#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2

typedef struct SaveData
{
    char GPS_Buffer[GPS_Buffer_Length];
    char isGetData;		//是否获取到GPS数据
    char isParseData;	//是否解析完成
    char UTCTime[UTCTime_Length];		//UTC时间
    char latitude[latitude_Length];		//纬度
    char N_S[N_S_Length];		//N/S
    char longitude[longitude_Length];		//经度
    char E_W[E_W_Length];		//E/W
    char isUsefull;		//定位信息是否有效
} _SaveData;
_SaveData Save_Data;

void parseGpsBuffer(int pos)
{
    char *subString;
    char *subStringNext;
    char i = 0;
    if (Save_Data.isGetData)
    {
        Save_Data.isGetData = false;

        for (i = 0 ; i <= 6 ; i++)
        {
            if (i == 0)
            {
                if ((subString = strstr(Save_Data.GPS_Buffer + pos, ",")) == NULL)
                {
                    printf(" 解析错误 1\r\n");
                }
            }
            else
            {
                subString++;
                
                if ((subStringNext = strstr(subString, ",")) != NULL)
                {
                    char usefullBuffer[2];
                    switch(i)
                    {
                        case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
                        case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
                        case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
                        case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
                        case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
                        case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

                        default:break;
                    }

                    subString = subStringNext;
                    Save_Data.isParseData = true;
                    if(usefullBuffer[0] == 'A')
                        Save_Data.isUsefull = true;
                    else if(usefullBuffer[0] == 'V')
                        Save_Data.isUsefull = false;

                }
                else
                {
                   printf(" 解析错误 2\r\n");
                }
            }
        }
    }
}


// GPS数据转化单位为度。
double Convert_to_degrees(char* data)
{
    double temp_data = atof(data);
    int degree = (int)(temp_data / 100);
    double f_degree = (temp_data / 100.0 - degree)*100/60.0;
    double result = degree + f_degree;
    return result;
}

//打印相关数据
void printGpsBuffer()
{
    double f_latitude = 0.0;
    double f_longitude = 0.0;

    if (Save_Data.isParseData)
    {
        Save_Data.isParseData = false;

        printf("Save_Data.UTCTime = ");
        printf(Save_Data.UTCTime);
        printf("\r\n");

        if (Save_Data.isUsefull)
        {
            Save_Data.isUsefull = false;
            printf("Save_Data.latitude = ");
            f_latitude = Convert_to_degrees(Save_Data.latitude);
            printf("%lf%s", f_latitude, Save_Data.N_S);
            printf("\r\n");

            printf("Save_Data.N_S = ");
            printf(Save_Data.N_S);
            printf("\r\n");

            printf("Save_Data.longitude = ");
            f_longitude = Convert_to_degrees(Save_Data.longitude);
            printf("%lf%s", f_longitude, Save_Data.E_W);
            printf("\r\n");

            printf("Save_Data.E_W = ");
            printf(Save_Data.E_W);
            printf("\r\n");
        } else {
            printf("GPS DATA is not usefull!\r\n");
        }

    }
}

bool ATGM336H::Init(std::string &configPath)
{
    std::cout<<"Gps Sensor"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    string serial_port = (*ini)["ATGM336H"]["serial_port"];
    baudRate = (*ini)["Speed"]["baudRate"];
    std::cout<<"serial_port:" << serial_port <<std::endl;
    device_path_ = new char[serial_port.length() + 1];
    std::strcpy(device_path_,serial_port.c_str());

    serial_fd_ = -1;
    
    return true;
}

int ATGM336H::GetFrameData(std::vector<DataBase *> &data)
{
    //暂时先不定义力矩传感器的数据结构，用imu的代替
    if(data.size() <= 1 )
    {
        data.push_back(new GpsData());
    }
    GpsData* gps_data = dynamic_cast<GpsData*>(data[0]);

        int ret = read(serial_fd_, Save_Data.GPS_Buffer, sizeof(Save_Data.GPS_Buffer)-1);
        Save_Data.GPS_Buffer[ret] = 0;
        if (ret < 0)
        {
            printf("read failed\n");
        }
        else if (ret > 0)
        {
            int pos = 0;//获取关键数据相对位置
            for (int i = 0; i < GPS_Buffer_Length; i++)
            {
                if (Save_Data.GPS_Buffer[i] == '$' && Save_Data.GPS_Buffer[i + 1] == 'G' && Save_Data.GPS_Buffer[i + 2] == 'N' && Save_Data.GPS_Buffer[i + 3] == 'R' && Save_Data.GPS_Buffer[i + 4] == 'M' && Save_Data.GPS_Buffer[i + 5] == 'C')
                {

                    pos = i;      
                       if(Save_Data.GPS_Buffer[pos] == '$' && Save_Data.GPS_Buffer[pos + 4] == 'M' && Save_Data.GPS_Buffer[pos + 5] == 'C')//确定是否收到"GPRMC/GNRMC"这一帧数据
            {
                Save_Data.isGetData = true;
            
            
                printf("原始数据获取成功\r\n");
                if (Save_Data.isGetData)
                {
                    
                    parseGpsBuffer(pos);
                    printGpsBuffer();
                    memset(Save_Data.GPS_Buffer,0,GPS_Buffer_Length);             
               }           
            }          
                }
                
            }           
            
           
        }

        gps_data->sentenceHasFix = static_cast<bool>(Save_Data.isUsefull);
   
    return 0;
}


int ATGM336H::OpenDevice()
{  
   /*第一步，串口初始化*/
        while (1)
        {

           // printf("device_path_ :%s\n",device_path_);
            serial_fd_ = open(device_path_,O_RDWR | O_NOCTTY | O_NDELAY);
            if(serial_fd_ != -1) 
            break;
        }
        
         struct termios new_cfg;
         memset(&new_cfg, 0, sizeof(new_cfg));
       
        // 这个波特率是固定好了吗，不需要外部传入吗？
        cfmakeraw(&new_cfg);//设置为原始模式 
	    new_cfg.c_cflag |= CREAD;// 使能接收 
        speed_t speed ;
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
            printf("default baud rate: 115200\n");
            break;
         }
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
     printf("open\n");
    return 0;
}


int ATGM336H::CloseDevice()
{
     if (serial_fd_ != -1)
    {
        std::cout<<"Gps close" << std::endl;
        close(serial_fd_);
        serial_fd_ = -1;
    }   
    return 0;
}
