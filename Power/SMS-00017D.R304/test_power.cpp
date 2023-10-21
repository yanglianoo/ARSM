#include "../../Smw/SmwApi.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include "PowerDevice.h"

void PrintComposeframe(DataBase* data)
{
    PowerData *powerdata = static_cast<PowerData *>(data);
    std::cout<<std::endl;
    std::cout<<"powerdata.freqency= "<<powerdata->frequency<<std::endl;
    std::cout<<"powerdata.fx= "<<powerdata->fx<<std::endl;
    std::cout<<"powerdata.fy= "<<powerdata->fy<<std::endl;
    std::cout<<"powerdata.fz= "<<powerdata->fz<<std::endl;
    std::cout<<"powerdata.Mx= "<<powerdata->Mx<<std::endl;
    std::cout<<"powerdata.My= "<<powerdata->My<<std::endl;
    std::cout<<"powerdata.Mz= "<<powerdata->Mz<<std::endl;
    std::cout<<std::endl;

}

int main()
{
    std::string path = "../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("Power");
    std::vector<DataBase*> data;
    while(1)
    {
        SensorState ret =  Auto_Monitor(dev);

         if(ret == RUNNING)
         {
            GetFrameData(dev,data);
           PrintComposeframe(data[0]);
         }

    }
    
    return 0;
}