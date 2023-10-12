#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"

class Speed:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override; 

        ~Speed()
        {
            delete [] device_path_;
        }     
    private:
        char* device_path_;
        int baudRate;
        int serial_fd_;
};

CLASS_LOADER_REGISTER_CLASS(Speed,SensorDevice)
