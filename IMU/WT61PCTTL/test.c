#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include <stdlib.h>

/***************************调试信息控制*********************************/
// #define __Debug_info

/**************************速度*****************************************/
#define GPS_Buffer_Length 60
typedef struct SaveData
{
    char GPS_Buffer[GPS_Buffer_Length];
    struct 
    {
       int16_t x;
       int16_t y;
       int16_t z;
    }raw;
    float x;
    float y;
    float z;
} _SaveData;
_SaveData Save_Data;



/***************************main主函数*********************************/
int main(int argc, char *argv[])
{
	int fd,ret;
	struct termios old_cfg={0}; //用于保存终端配置之前的参数
	struct termios new_cfg={0}; //用于保存终端新配置的参数
	speed_t speed = B9600;		//定义波特率为9600
    /*第一步，串口初始化*/
	fd = open(argv[1],O_RDWR | O_NOCTTY | O_NDELAY);//O_NOCTTY 标志，告知系统该节点不会成为进程的控制终端
    if(fd < 0){
		printf("uart device open error\n");
		return -1;
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
	ret = tcflush(fd, TCIOFLUSH);//清空缓冲区域

	if(ret < 0)
    {
		printf("tcflush error\n");
		return -1;
	}
	ret = tcsetattr(fd, TCSANOW, &new_cfg);//写入配置、使配置生效
	if(ret < 0){
		printf("tcsetattr error\n");
		return -1;
	}	

	
	while(1)
    { 
        ret = read(fd, Save_Data.GPS_Buffer, sizeof(Save_Data.GPS_Buffer)-1);
        Save_Data.GPS_Buffer[ret] = 0;
        if (ret < 0)
        {
            printf("read failed\n");
        }
        else if (ret > 0)
        {
            
           //原始数据
#ifdef __Debug_info
           for (int i = 0; i < GPS_Buffer_Length; i++)
           {
                printf("0x%x ", Save_Data.GPS_Buffer[i]);              
           } 
           printf("\r\n");         
#endif




        //查找关键数据位置
        int pos = 0;
        int i = 0;
        for (i = 0; i < GPS_Buffer_Length; i++)
        {
            if(Save_Data.GPS_Buffer[i] == 0x55 && Save_Data.GPS_Buffer[i + 1] == 0x52)
            {
               printf("获取到关键数据帧\r\n");            
#ifdef __Debug_info
               printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \r\n",Save_Data.GPS_Buffer[i + 2],Save_Data.GPS_Buffer[i + 3],Save_Data.GPS_Buffer[i + 4],Save_Data.GPS_Buffer[i + 5],Save_Data.GPS_Buffer[i + 6],Save_Data.GPS_Buffer[i + 7]);
#endif               
               pos = i + 2; 
               break;            
                
            }
            
        }
        //计算角速度
        Save_Data.raw.x =  (int16_t)(Save_Data.GPS_Buffer[pos+1] << 8)|Save_Data.GPS_Buffer[pos];
        pos = pos + 2;
        Save_Data.raw.y =  (int16_t)(Save_Data.GPS_Buffer[pos+1] << 8)|Save_Data.GPS_Buffer[pos];        
        pos = pos + 2;
        Save_Data.raw.z =  (int16_t)(Save_Data.GPS_Buffer[pos+1] << 8)|Save_Data.GPS_Buffer[pos];
        
        Save_Data.x = (float)Save_Data.raw.x / 32768 *2000;
        Save_Data.y = (float)Save_Data.raw.y / 32768 *2000;
        Save_Data.z = (float)Save_Data.raw.z / 32768 *2000;
            
        printf("Gx: %.02f Gy: %.02f Gz: %.02f\r\n",Save_Data.x,Save_Data.y,Save_Data.z);    
        sleep(1);        
            
         
            
        }
    }
	
	tcsetattr(fd, TCSANOW, &old_cfg);//恢复到之前的配置
	close(fd);
	return 0;
}


