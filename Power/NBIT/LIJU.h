#pragma once 
#include <unistd.h>
#include <pthread.h>
#include  <netdb.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <ctype.h>
#include<errno.h>


#define DEST_ADDR "192.168.1.1"
#define UDP_PORT 1024

#define LOCAL_ADDR "192.168.1.107"
#define LOCAL_PORT 8080


class LIJU
{
public:
    LIJU(){};
    ~LIJU();
    void UDPDirectInit(char addr);
    void init(const char* ip,size_t port);
    void UDP_GetData();//UDP发送指令，获取数据
    void calc_resault();//原始数据处理
    void sensor_close();
    void print_RAWData();//打印原始数据
    void print_resault();//打印LIJU数据
    double Sum[6] = {0};  //力矩数据
private:
    char sendInit[5];             //初始化指令;
    char sendData[5];            //取得数据指令;
    char  sendClear[5];          //设备清除零点指令;
    char  sendSetClear[5];       //设备设置零点指令;
    char sendSearch[5];          //搜索IP/Port指令;
    char sendRestar[5];          //设备重启指令;
    char buffer[23] = {0};  //原始数据
	int sockfd;         
	struct sockaddr_in dest_addr; //目标服务器的地址信息，就是力矩传感器
    struct sockaddr_in localaddr;
    struct sockaddr_in Bcast_addr;
// 用于获取客户端信息
    sockaddr_in clientaddr;
    socklen_t   socklen=sizeof(clientaddr);

	socklen_t len;
    int ret_value = 0;  //异常值返回
    int read_len = 0; //读取的数据个数
};


