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

#define __Debug_info
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



int getIndexOfSigns(char ch);
long hexToDec(char* source);
/***************************main主函数*********************************/
int main(int argc, char *argv[])
{
	int fd,ret;
	struct termios old_cfg={0}; //用于保存终端配置之前的参数
	struct termios new_cfg={0}; //用于保存终端新配置的参数
	speed_t speed = B9600;		//定义波特率为9600
    /*第一步，串口初始化*/
	fd = open(argv[1],O_RDWR | O_NOCTTY | O_NDELAY);//O_NOCTTY 标志，告知系统该节点不会成为进程的控制终端
        if(fd < 0)
        {
                printf("uart device open error\n");
                return -1;
        }
	ret = tcgetattr(fd, &old_cfg);
	if(ret < 0)
        {
		printf("tcgetattr error\n");
		close(fd);
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
        Save_Data.order[0] = 0x01;
        Save_Data.order[1] = 0x03;
        Save_Data.order[2] = 0x00;
        Save_Data.order[3] = 0x20;
        Save_Data.order[4] = 0x00;
        Save_Data.order[5] = 0x02;
        Save_Data.order[6] = 0xC5;
        Save_Data.order[7] = 0xC1;
	
        while(1)
        { 
                int ret = write(fd,Save_Data.order,8);
                if(ret < 0)
                {
                        printf("write fail\r\n");
                }
                else
                {
                        printf("write data: %d\r\n",ret);
                }
                bzero(Save_Data.SUDU_Buffer,sizeof(Save_Data.SUDU_Buffer));
                ret = read(fd,Save_Data.SUDU_Buffer,10);
                if(ret < 0)
                {
                        printf("read fail\r\n");
                }
                else
                {
                        printf("read data: %d\r\n",ret);
#ifdef  __Debug_info
                        for(int i = 0;i < ret;i++)
                        {
                                printf("0x%x ",Save_Data.SUDU_Buffer[i]);
                        }
                        printf("\r\n");
#endif

                        for(int i = 0;i < ret;i++)
                        {
                                if(Save_Data.SUDU_Buffer[i] == 0x03 && Save_Data.SUDU_Buffer[i + 1] == 0x04)
                                {
                                        Save_Data.pos = i + 1;
                                        break;
                                }
                        }
                        sprintf(Save_Data.buf, "%x%x%x%x",Save_Data.SUDU_Buffer[ Save_Data.pos + 1],Save_Data.SUDU_Buffer[ Save_Data.pos + 2],Save_Data.SUDU_Buffer[ Save_Data.pos +3],Save_Data.SUDU_Buffer[ Save_Data.pos + 4]);

                        Save_Data.Data =  hexToDec(Save_Data.buf);
                        printf("SUDU_Data: %d\r\n", Save_Data.Data);
                        Save_Data.n =   (float)Save_Data.Data*0.00915;
                        printf("n: %f\r\n",Save_Data.n);
                }
                usleep(100000);
                printf("\r\n");
        }

        tcsetattr(fd, TCSANOW, &old_cfg);//恢复到之前的配置
        close(fd);
        return 0;
}

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
    printf("len:%d\r\n",len);
    for (i = len - 1; i >= 0; i--)
    {
        sum += t * getIndexOfSigns(*(source + i));
        t *= 16;
    }

    return sum;
}
