#include "PowerDevice.h"


unsigned char send_buffer[8] = {0x01,0x03,0x04,0x00,0x00,0x0c,0x44,0xff};


void PowerSensor::LIJU_data_printf(unsigned char *buffer, int len)
{
	int i;
	for(i = 0;i < len;i++)
	{
		printf("0x%x ",buffer[i]);
	}
	printf("\n");
}
void PowerSensor::Compon_data(unsigned int* desc,unsigned char* buffer,int i)
{
    if(i == 1)
	{
		*desc = buffer[3];
		*desc <<= 8;
		*desc |= buffer[4];
		*desc <<= 8;
		*desc |= buffer[5];
		*desc <<= 8;
		*desc |= buffer[6];
	}
	else if(i == 2)
	{
		*desc = buffer[7];
		*desc <<= 8;
		*desc |= buffer[8];
		*desc <<= 8;
		*desc |= buffer[9];
		*desc <<= 8;
		*desc |= buffer[10];
	}	
	else if(i == 3)
	{
		*desc = buffer[11];
		*desc <<= 8;
		*desc |= buffer[12];
		*desc <<= 8;
		*desc |= buffer[13];
		*desc <<= 8;
		*desc |= buffer[14];
	}
	
	else if(i == 4)
	{
		*desc = buffer[15];
		*desc <<= 8;
		*desc |= buffer[16];
		*desc <<= 8;
		*desc |= buffer[17];
		*desc <<= 8;
		*desc |= buffer[18];
	}
	
	else if(i == 5)
	{
		*desc = buffer[19];
		*desc <<= 8;
		*desc |= buffer[20];
		*desc <<= 8;
		*desc |= buffer[21];
		*desc <<= 8;
		*desc |= buffer[22];
	}
	
	else if(i == 6)
	{
		*desc = buffer[23];
		*desc <<= 8;
		*desc |= buffer[24];
		*desc <<= 8;
		*desc |= buffer[25];
		*desc <<= 8;
		*desc |= buffer[26];
	}
}
void PowerSensor::Init(const char * serial){
	strcpy(device_path_, serial);
    serial_fd_ = -1;
}

bool PowerSensor::OpenDevice(){
    while(1)
    {
		
        serial_fd_ = open(device_path_, O_RDWR | O_NOCTTY);
        if (serial_fd_ != -1)
			break;

    }
    /* 获取串口当前的配置参数 */
    if (0 > tcgetattr(serial_fd_, &old_cfg)) {
        fprintf(stderr, "tcgetattr error: %s\n", strerror(errno));
        close(serial_fd_);
        return -1;
    }
	uart_cfg(&cfg);
	
    return true;
}

int PowerSensor::ReadData(float* data){
	
    unsigned char buffer_1[SERIAL_BUFFER_LEN_1];
                                 /* 一次读操作读到的数据 */
    unsigned int read_count_1 = 29; 
    int flag_read = 0;
    int flag_write = 0;

     if (-1 == (flag_write = write(serial_fd_, send_buffer, 8)))
		{
				// perror("[ERROR] write serial");
				// return -1;
		}

    memset(buffer_1,0,read_count_1);
    if (-1 == (flag_read = read(serial_fd_,buffer_1,read_count_1)))
    {
        // printf("[ERROR] read serial: %s\n",SERIAL_DEVICE_NAME_1);
		// perror("[ERROR] read serial");
    }

    Compon_data(&Save_Data.fx,buffer_1,1);
	Compon_data(&Save_Data.fy,buffer_1,2);
	Compon_data(&Save_Data.fz,buffer_1,3);
	Compon_data(&Save_Data.Mx,buffer_1,4);
	Compon_data(&Save_Data.My,buffer_1,5);
	Compon_data(&Save_Data.Mz,buffer_1,6);
    
    data[0] = hex_flo(Save_Data.fx);
    data[1] = hex_flo(Save_Data.fy);
    data[2] = hex_flo(Save_Data.fz);
    data[3] = hex_flo(Save_Data.Mx);
    data[4] = hex_flo(Save_Data.My);
    data[5] = hex_flo(Save_Data.Mz);
    usleep(100000);
    return 0;
}

void PowerSensor::CloseDevice(){
    if (serial_fd_ != -1)
        {
            std::cout<<"power close" << std::endl;
            close(serial_fd_);
            serial_fd_ = -1;
        }   
}


//extern void io_handler(int sig, siginfo_t *info, void *context);
// void PowerSensor::async_io_init()
// {
//     struct sigaction sigatn;
//     int flag;

//     /* 使能异步I/O */
//     flag = fcntl(serial_fd_, F_GETFL);  //使能串口的异步I/O功能
//     flag |= O_ASYNC;
//     fcntl(serial_fd_, F_SETFL, flag);

//     /* 设置异步I/O的所有者 */
//     fcntl(serial_fd_, F_SETOWN, getpid());

//     /* 指定实时信号SIGRTMIN作为异步I/O通知信号 */
//     fcntl(serial_fd_, F_SETSIG, SIGRTMIN);

//     /* 为实时信号SIGRTMIN注册信号处理函数 */
//     sigatn.sa_sigaction = io_handler;   //当串口有数据可读时，会跳转到io_handler函数
//     sigatn.sa_flags = SA_SIGINFO;
//     sigemptyset(&sigatn.sa_mask);
//     sigaction(SIGRTMIN, &sigatn, NULL);
// }


// void PowerSensor::send_data()
// {

// 	write(serial_fd_,send_buffer,sizeof(send_buffer));
// 	sleep(1);

// }
/**
 ** 串口配置
 ** 参数cfg指向一个uart_cfg_t结构体对象
 **/
int PowerSensor::uart_cfg(const uart_cfg_t *cfg)
{
    struct termios new_cfg = {0};   //将new_cfg对象清零
    speed_t speed;

    /* 设置为原始模式 */
    cfmakeraw(&new_cfg);

    /* 使能接收 */
    new_cfg.c_cflag |= CREAD;

    /* 设置波特率 */
    switch (cfg->baudrate) {
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

    if (0 > cfsetspeed(&new_cfg, speed)) {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置数据位大小 */
    new_cfg.c_cflag &= ~CSIZE;  //将数据位相关的比特位清零
    switch (cfg->dbit) {
        case 5:
            new_cfg.c_cflag |= CS5;
            break;
        case 6:
            new_cfg.c_cflag |= CS6;
            break;
        case 7:
            new_cfg.c_cflag |= CS7;
            break;
        case 8:
            new_cfg.c_cflag |= CS8;
            break;
        default:    //默认数据位大小为8
            new_cfg.c_cflag |= CS8;
            printf("default data bit size: 8\n");
            break;
    }

    /* 设置奇偶校验 */
    switch (cfg->parity) {
        case 'N':       //无校验
            new_cfg.c_cflag &= ~PARENB;
            new_cfg.c_iflag &= ~INPCK;
            break;
        case 'O':       //奇校验
            new_cfg.c_cflag |= (PARODD | PARENB);
            new_cfg.c_iflag |= INPCK;
            break;
        case 'E':       //偶校验
            new_cfg.c_cflag |= PARENB;
            new_cfg.c_cflag &= ~PARODD; /* 清除PARODD标志，配置为偶校验 */
            new_cfg.c_iflag |= INPCK;
            break;
        default:    //默认配置为无校验
            new_cfg.c_cflag &= ~PARENB;
            new_cfg.c_iflag &= ~INPCK;
            printf("default parity: N\n");
            break;
    }

    /* 设置停止位 */
    switch (cfg->sbit) {
        case 1:     //1个停止位
            new_cfg.c_cflag &= ~CSTOPB;
            break;
        case 2:     //2个停止位
            new_cfg.c_cflag |= CSTOPB;
            break;
        default:    //默认配置为1个停止位
            new_cfg.c_cflag &= ~CSTOPB;
            printf("default stop bit size: 1\n");
            break;
    }


    /* 清空缓冲区 */
    if (0 > tcflush(serial_fd_, TCIOFLUSH)) {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        return -1;
    }

    /* 写入配置、使配置生效 */
    if (0 > tcsetattr(serial_fd_, TCSANOW, &new_cfg)) {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        return -1;
    }

    /* 配置OK 退出 */
    return 0;
}