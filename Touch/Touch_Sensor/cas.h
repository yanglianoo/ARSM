#include "CasTouchSensor.h"
#include "../../Smw/SensorDevice.h"
#include "../../Smw/utility/ClassFactory.hpp"

class Cas:public SensorDevice
{
    public:
        bool Init(std::string &configPath)  override;
        int GetFrameData(std::vector<DataBase *> &data) override ;
        int OpenDevice() override;
        int CloseDevice() override;
    private:
        CasTouchSensor cas;
};

CLASS_LOADER_REGISTER_CLASS(Cas,SensorDevice)

