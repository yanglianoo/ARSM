#include "SL_S1.h"
#include "../Smw/utility/ini_file.hpp"
#include "../Smw/utility/singleton.hpp"
#include <iostream>
#include <thread>
#include "math.h"

#define DEG2RAD(x) ((x)*M_PI/180.)

using namespace sl;
using namespace SMW;
using namespace std;

bool SL_lidar_S1::Init(std::string &configPath) 
{
    bool connectSuccess = false;
    sl_lidar_response_device_info_t devinfo;
    std::cout<<"SL_lidar_S1"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    int baudRate = (*ini)["SL_lidar_S1"]["baudRate"];
    string serial_port = (*ini)["SL_lidar_S1"]["serial_port"];
    
    std::cout<<"serial_port:"<<serial_port<< "  baudRate: " << baudRate << std::endl;
    
    drv = *createLidarDriver();
    if(!drv)
    {
        std::cout << "drv Create fail"<<std::endl;
        exit(-2);
    }
    _channel = *createSerialPortChannel(serial_port,baudRate);
    if (SL_IS_OK((drv)->connect(_channel))) {
        op_result = drv->getDeviceInfo(devinfo);
        if (SL_IS_OK(op_result)) 
        {
	       connectSuccess = true;
           std::cout<<"succes"<<std::endl;
        }
    else{
            delete drv;
		    drv = NULL;
        }
    }
     if (!connectSuccess) {
        (opt_channel_type == CHANNEL_TYPE_SERIALPORT)?
			(fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
				,serial_port )):(fprintf(stderr, "Error, cannot connect to the specified ip addr %s.\n"
				, serial_port));
		
        //goto on_finished;
    }
}

int SL_lidar_S1::OpenDevice()
{
    std::cout<<"SL_Lidar OpenDevice"<<std::endl;
        if(drv->isConnected())
    {
      std::cout<<"Start motor"<<std::endl;
      sl_result ans=drv->setMotorSpeed();
      if(ans == SL_RESULT_OK)
      {
        if(SL_IS_FAIL(drv->startScan( 0,1 )))
        {
            std::cout<<"fail" << std::endl;
        }
      }
      else{
        std::cout<<"Start motor fail"<<std::endl;
      }
   }
}
int SL_lidar_S1::GetFrameData(std::vector<DataBase*> &data)
{
        if(data.size() <= 1 )
    {
        data.push_back(new LidarScan());
    }
    // std::cout<<"debug"<<std::endl;
    LidarScan* RPLidar_S1 = static_cast<LidarScan*>(data[0]);
        auto start_time = std::time(nullptr);

        sl_lidar_response_measurement_node_hq_t nodes[8192];
        size_t count = _countof(nodes);
        // std::cout<<"count"<<count<<std::endl;
        op_result = drv->grabScanDataHq(nodes,count);
        auto end_time = std::time(nullptr);
        // std::cout<<"debug"<<std::endl;
        auto scan_duration = end_time - start_time;
        
    if (op_result == SL_RESULT_OK )
     {
        op_result = drv->ascendScanData(nodes,count);
        float angle_min = DEG2RAD(0.0f);
        float angle_max = DEG2RAD(360.0f);
        int start_node = 0, end_node = 0;
        int i = 0;
        
        // find the first valid node and last valid node
        while (nodes[i++].dist_mm_q2 == 0);
            start_node = i-1;
            i = count -1;
            while (nodes[i--].dist_mm_q2 == 0);
                end_node = i+1;

                angle_min = DEG2RAD(getAngle(nodes[start_node]));
                angle_max = DEG2RAD(getAngle(nodes[end_node]));
                RPLidar_S1->timestamp = start_time; 
                RPLidar_S1->frame_id = "laser_frame";
               bool reversed = (angle_max > angle_min);
                if ( reversed )
                {
                    RPLidar_S1->angle_min = M_PI - angle_max;
                    RPLidar_S1->angle_max = M_PI - angle_min;
                }else
                {
                    RPLidar_S1->angle_min = M_PI - angle_min;
                    RPLidar_S1->angle_max = M_PI - angle_max;
                }
                RPLidar_S1->angle_increment = (RPLidar_S1->angle_max - RPLidar_S1->angle_min)/(double)(end_node-start_node+1);
                RPLidar_S1->scan_time = scan_duration;
                RPLidar_S1->time_increment = scan_duration/(double)(end_node-start_node+1);
                RPLidar_S1->range_min = 0.15;
                RPLidar_S1->range_max = 8.0;
                
                RPLidar_S1->intensities.resize(end_node-start_node+1);
                RPLidar_S1->ranges.resize(end_node-start_node+1);
                // std::cout<<"data count:" <<end_node-start_node+1<<std::endl;
                bool reverse_data = (!inverted && reversed) || (inverted && !reversed);
            if (!reverse_data) {
        for (size_t i = 0; i < (end_node-start_node+1); i++) {
            float read_value = (float) nodes[i].dist_mm_q2/4.0f/1000;
            if (read_value == 0.0)
                RPLidar_S1->ranges[i] = std::numeric_limits<float>::infinity();
            else
                 RPLidar_S1->ranges[i] = read_value;
             RPLidar_S1->intensities[i] = (float) (nodes[i].quality >> 2);
            }
         } else {
        for (size_t i = 0; i < (end_node-start_node+1); i++) {
            float read_value = (float)nodes[i].dist_mm_q2/4.0f/1000;
            if (read_value == 0.0)
                 RPLidar_S1->ranges[end_node-start_node+1-1-i] = std::numeric_limits<float>::infinity();
            else
                 RPLidar_S1->ranges[end_node-start_node+1-1-i] = read_value;
                RPLidar_S1->intensities[end_node-start_node+1-1-i] = (float) (nodes[i].quality >> 2);
         }
        }

    }
        
}
int SL_lidar_S1::CloseDevice()
{
    
    drv->setMotorSpeed(0);
    drv->stop();
    drv->disconnect();
    
}