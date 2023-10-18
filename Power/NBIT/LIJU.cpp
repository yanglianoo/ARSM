#include "LIJU.h"
#include <fcntl.h>
void LIJU::init(const char* ip,size_t port)
{
    printf("ip:%s port:%d \n",ip,port);
    //初始化命令 
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        printf("socket create fail\r\n");
        return;
    }
    int nflag = 1;

    nflag = fcntl (sockfd, F_GETFL, 0);

    nflag = nflag | O_NONBLOCK;
 
    fcntl(sockfd, F_SETFL, nflag);

    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(ip);
    dest_addr.sin_port = htons(port);
}

void LIJU::sensor_close()
{
    close(sockfd);
}

LIJU::~LIJU()
{
    close(sockfd);
}

void LIJU::UDP_GetData()
{
    
    ret_value =  sendto(sockfd,sendData,sizeof(sendData),0,(const sockaddr*)&dest_addr,sizeof(dest_addr));
    if(ret_value  <  0)
    {
        printf("sendto error\r\n");
    }   
    memset(buffer,0,sizeof(buffer));
    read_len = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&clientaddr,&socklen);

    char *ip = inet_ntoa(clientaddr.sin_addr); 
    inet_ntop(clientaddr.sin_family, &clientaddr.sin_addr, ip, socklen);

    int port = ntohs(clientaddr.sin_port);
    sleep(1);
}

void LIJU::print_RAWData()
{
        for( int i = 0;i < read_len;i++)
        {
            printf("0x%x  ",buffer[i]);
        }
        printf("\r\n");
}
void LIJU::print_resault()
{
    printf("\r\n***************************************************************************\r\n");
    printf("fx:%f\r\n",Sum[0]);
    printf("fy:%f\r\n",Sum[1]);
    printf("fz:%f\r\n",Sum[2]);
    printf("Tx:%f\r\n",Sum[3]);
    printf("Tx:%f\r\n",Sum[4]);
    printf("Tx:%f\r\n",Sum[5]);
    printf("\r\n");
}

void LIJU:: calc_resault()
{
     int data[6] = {0};//数据解析后的中间变量值
     double sum[6] = {0};
     for(int i = 0;i < 6; ++i)
    {
        if (buffer[i*3+3] & 0x80)				// 判断数据的正负号
        {
            buffer[i*3+3] &= 0x7F;
        }
        for(int j = 0; j < 3; ++j)
        {
            int n = i*3+3+j;
            data[i] = (data[i] << 8) + buffer[n];
        }
        sum[i] = data[i]/1000.0;
        if (buffer[i*3+3] & 0x80)	sum[i] *= -1;
    }
    for(int i = 0; i < 6; ++i)
    {
        Sum[i] = sum[i];
    }
}
void LIJU::UDPDirectInit(char addr)
{
    /*******************初始化指令*********************/
    sendInit[0]=addr;
    sendInit[1]=0x13;
    sendInit[2]=0x00;
    sendInit[3]=0x00;
    sendInit[4]=0x01;

    /*******************取得数据指令*********************/
    sendData[0] = addr;
    sendData[1] = 0x03;
    sendData[2] = 0x02;
    sendData[3] = 0x00;
    sendData[4] = 0x12;

    /*******************设备清零指令*********************/
    sendClear[0] = addr;
    sendClear[1] = 0x19;
    sendClear[2] = 0x01;
    sendClear[3] = 0xA7;
    sendClear[4] = 0x18;

    /*******************设备设置零点指令*********************/

    sendSetClear[0] = addr;
    sendSetClear[1] = 0x12;
    sendSetClear[2] = 0x01;
    sendSetClear[3] = 0xA7;
    sendSetClear[4] = 0x01;

    /*******************搜索IP/Port指令*********************/
    sendSearch[0] = addr;
    sendSearch[1] = 0x15;
    sendSearch[2] = 0x00;
    sendSearch[3] = 0x00;
    sendSearch[4] = 0x01;

    /*******************设备重启指令*********************/
    sendRestar[0] = addr;
    sendRestar[1] = 0x18;
    sendRestar[2] = 0x00;
    sendRestar[3] = 0x00;
    sendRestar[4] = 0x00;

}



