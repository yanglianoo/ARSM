
#pragma once

#include <string>

typedef struct _DataBase {
    virtual ~_DataBase() = default;
    int dataType = 0;        // 参考枚举 DataType
    uint32_t frequency = 0;  // 采集频率
    uint32_t frameId = 0;    // 帧号ID
    uint64_t timestamp = 0;  // 时间戳 ms
    std::string devName;     // 哪个设备来的数据。初始化的时候，配置。
} DataBase;