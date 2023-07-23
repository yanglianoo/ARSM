/**
 * @File Name: DataBase.h
 * @brief  
 * @Author : Timer email:330070781@qq.com
 * @Version : 1.0
 * @Creat Date : 2023-07-15
 * 
 */
#pragma once
#include <cfloat>
#include <string>
#include <vector>

typedef struct _DataBase {
    virtual ~_DataBase() = default;
    int dataType = 0;        // 参考枚举 DataType
    uint32_t frequency = 0;  // 采集频率
    uint32_t frameId = 0;    // 帧号ID
    uint64_t timestamp = 0;  // 时间戳 ms
    std::string devName;     // 哪个设备来的数据。初始化的时候，配置。
} DataBase;

struct ImuData :public DataBase {
    float gyroscope_x;           // 机体系X轴角速度
    float gyroscope_y;           // 机体系Y轴角速度
    float gyroscope_z;           // 机体系Z轴角速度
    float accelerometer_x;       // 机体系X轴加速度(未分离重力加速度)
    float accelerometer_y;       // 机体系Y轴加速度(未分离重力加速度)
    float accelerometer_z;       // 机体系Z轴加速度(未分离重力加速度)
    float magnetometer_x;        // 机体系X轴磁感应强度
    float magnetometer_y;        // 机体系Y轴磁感应强度
    float magnetometer_z;        // 机体系Z轴磁感应强度
    float imu_temperature;       // 如果IMU数据由多个传感器组成则该值为这些传感 器的平均温度
    float Pressure;              // 气压值
    float pressure_temperature;  // 气压计的温度值
    int64_t Timestamp;           // 数据的时间戳
};

struct LidarScan :public DataBase
{
    std::string frame_id;
    _Float32 angle_min;
    _Float32 angle_max;
    _Float32 angle_increment;
    _Float32 time_increment;
    _Float32 scan_time;
    _Float32 range_min;
    _Float32 range_max;
    std::vector<_Float32> ranges;
    std::vector<_Float32> intensities; 
};
