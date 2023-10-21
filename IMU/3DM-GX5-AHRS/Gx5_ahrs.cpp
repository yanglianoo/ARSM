#include "Gx5_ahrs.h"
#include "../../Smw/utility/ini_file.hpp"
#include "../../Smw/utility/singleton.hpp"
#include "../../Smw/SmwApi.h"
#include <iostream>


using namespace std;
using namespace mscl;
using namespace SMW;
//Example: Get Current Configuration
//  Shows how to read current configuration settings an Inertial Device.
static void getCurrentConfig(mscl::InertialNode& node)
{
    //many other settings are available than shown below
    //reference the documentation for the full list of commands

    //if the node supports AHRS/IMU
    if(node.features().supportsCategory(mscl::MipTypes::CLASS_AHRS_IMU))
    {
        //get a list of the AHRS/IMU channels currently active on the Node
        mscl::MipChannels ahrsImuActiveChs = node.getActiveChannelFields(mscl::MipTypes::CLASS_AHRS_IMU);

        cout << "AHRS/IMU Channels" << endl;
        cout << "-----------------" << endl;
        for(mscl::MipChannel ch : ahrsImuActiveChs)
        {
            cout << "Channel Field: " << std::hex << ch.channelField() << endl;
            cout << "Sample Rate: " << ch.sampleRate().prettyStr() << endl << endl;
        }
    }
        //if the node supports Estimation Filter
    if(node.features().supportsCategory(mscl::MipTypes::CLASS_ESTFILTER))
    {
        //get a list of the Estimation Filter channels currently active on the Node
        mscl::MipChannels estFilterActiveChs = node.getActiveChannelFields(mscl::MipTypes::CLASS_ESTFILTER);

        cout << endl;
        cout << "Estimation Filter Channels" << endl;
        cout << "--------------------------" << endl;
        for(mscl::MipChannel ch : estFilterActiveChs)
        {
            cout << "Channel Field: " << std::hex << ch.channelField() << endl;
            cout << "Sample Rate: " << ch.sampleRate().prettyStr() << endl << endl;
        }
    }

    cout << "Pitch/Roll Aiding enabled?: " << node.getPitchRollAid() << endl;
}
//Example: Parse Data
//  Shows how to parse incoming data from an Inertial Device.
//  Note that this example does not start a Node sampling (assumes it already is).
static void parseData(mscl::InertialNode& node)
{
    //endless loop of reading in data
    while(true)
    {
        //get all the data packets from the node, with a timeout of 500 milliseconds
        mscl::MipDataPackets packets = node.getDataPackets(500);

        for(mscl::MipDataPacket packet : packets)
        {
            //print out the data
            cout << "Packet Received: ";

            //get the data in the packet
            mscl::MipDataPoints data = packet.data();
            mscl::MipDataPoint dataPoint;

            //loop through all the data points in the packet
            for(unsigned int itr = 0; itr < data.size(); itr++)
            {
                dataPoint = data[itr];

                cout << dataPoint.channelName() << ": ";

                //print out the channel data
                //Note: The as_string() function is being used here for simplicity. 
                //      Other methods (ie. as_float, as_uint16, as_Vector) are also available.
                //      To determine the format that a dataPoint is stored in, use dataPoint.storedAs().
                cout << dataPoint.as_string() << " ";

                //if the dataPoint is invalid
                if(!dataPoint.valid())
                {
                    cout << "[Invalid] ";
                }
            }
            cout << endl;
        }
    }
}

static void setCurrentConfig(mscl::InertialNode& node)
{
    //many other settings are available than shown below
    //reference the documentation for the full list of commands

    //if the node supports AHRS/IMU
    if(node.features().supportsCategory(mscl::MipTypes::CLASS_AHRS_IMU))
    {   
        //加速度计和陀螺仪
        mscl::MipChannels ahrsImuChs;
        // ahrsImuChs.push_back(mscl::MipChannel(mscl::MipTypes::CH_FIELD_SENSOR_SCALED_ACCEL_VEC, mscl::SampleRate::Hertz(500)));   
        // ahrsImuChs.push_back(mscl::MipChannel(mscl::MipTypes::CH_FIELD_SENSOR_SCALED_GYRO_VEC, mscl::SampleRate::Hertz(100)));
         ahrsImuChs.push_back(mscl::MipChannel(mscl::MipTypes::CH_FIELD_SENSOR_EULER_ANGLES, mscl::SampleRate::Hertz(100)));
        //apply to the node
        node.setActiveChannelFields(mscl::MipTypes::CLASS_AHRS_IMU, ahrsImuChs);
        std::cout<<"AHRS/IMU setconfig success!"<<std::endl;
    }

    //if the node supports Estimation Filter
    if(node.features().supportsCategory(mscl::MipTypes::CLASS_ESTFILTER))
    {
        mscl::MipChannels estFilterChs;
        estFilterChs.push_back(mscl::MipChannel(mscl::MipTypes::CH_FIELD_ESTFILTER_ESTIMATED_GYRO_BIAS, mscl::SampleRate::Hertz(100)));

        //apply to the node
        node.setActiveChannelFields(mscl::MipTypes::CLASS_ESTFILTER, estFilterChs);
        std::cout<<"Estimation Filter setconfig success!"<<std::endl;
    }

    node.setPitchRollAid(true);


}

static void setToIdle(mscl::InertialNode& node)
{
    node.setToIdle();
}

static void startSampling(mscl::InertialNode& node)
{
    //each class/category is separated into its own command.
    //you can enable them individually if, say, you only wanted the Estimation Filter channels to be streamed

    if(node.features().supportsCategory(mscl::MipTypes::CLASS_AHRS_IMU))
    {
        node.enableDataStream(mscl::MipTypes::CLASS_AHRS_IMU);
    }

    if(node.features().supportsCategory(mscl::MipTypes::CLASS_ESTFILTER))
    {
        node.enableDataStream(mscl::MipTypes::CLASS_ESTFILTER);
    }
    
}
bool GX5_AHRS::Init(std::string &configPath) 
{

    std::cout<<"GX5_AHRS"<<std::endl;
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);

    int baudRate = (*ini)["GX5_AHRS"]["baudRate"];
    string serial_port = (*ini)["GX5_AHRS"]["serial_port"];

    std::cout<<"serial_port:"<<serial_port<< "  baudRate: " << baudRate << std::endl;
    connection = mscl::Connection::Serial(serial_port,baudRate);
    node = mscl::InertialNode(connection);
}



int GX5_AHRS::GetFrameData(std::vector<DataBase*> &data)
{
    if(data.size() <= 1 )
    {
        data.push_back(new ImuData());
    }
    ImuData* GX5_AHRS_data = dynamic_cast<ImuData*>(data[0]);
    GX5_AHRS_data->frequency = 100;
    mscl::MipDataPackets packets = node.getDataPackets();

        for(mscl::MipDataPacket packet : packets)
        {
            mscl::MipDataPoints data = packet.data();
            mscl::MipDataPoint dataPoint;
            
            for(unsigned int itr = 0; itr < data.size(); itr++)
            {
                dataPoint = data[itr];
                if(dataPoint.channelName() == "roll")
                {
                    GX5_AHRS_data->roll=dataPoint.as_float();
                }
                if(dataPoint.channelName() == "pitch")
                {
                    GX5_AHRS_data->pitch=dataPoint.as_float();
                }
                if(dataPoint.channelName() == "yaw")
                {
                    GX5_AHRS_data->yaw=dataPoint.as_float();
                }
            }
    }

}


int GX5_AHRS::OpenDevice()
{
    bool success = node.ping();
    cout<<"通信是否成功："<< success <<endl;
    cout << "Node Information: " << endl;
    cout << "Model Name: " << node.modelName() << endl;
    cout << "Model Number: " << node.modelNumber() << endl;
    cout << "Serial: " << node.serialNumber() << endl;
    cout << "Firmware: " << node.firmwareVersion().str() << endl << endl;
    //配置IMU信息
    setCurrentConfig(node);
    //获取IMU配置信息
    getCurrentConfig(node);
}


int GX5_AHRS::CloseDevice()
{
    connection.disconnect();
    cout<<"disconnect"<<endl;
}

void PrintComposeframe(DataBase *data)
{
    ImuData *frameData = static_cast<ImuData *>(data);
    printf("freqency: %d ",frameData->frequency);
    // std::cout<<"freqency: "<<frameData->frequency<<" ";
    std::cout << "roll: " << frameData->roll << " ";
    std::cout << "pitch: " << frameData->pitch <<" ";
    std::cout << "yaw: " << frameData->yaw <<" ";
    std::cout << std::endl;
    return;
}


int main()
{
    std::string path = "../../sensor_config.ini";
    SmwInit(path);
    auto dev = GetDevice("GX5_AHRS");
    std::vector<DataBase*> data;
    // OpenDevice(dev);
err:
    try
    {
        while(1)
        {
            SensorState ret =  Auto_Monitor(dev); 
            //std::cout<<"ret:"<<ret<<std::endl;
            if(ret == RUNNING)
             {
                GetFrameData(dev,data);
                PrintComposeframe(data[0]);
            }
        }
    }
    catch(mscl::Error& e)
    {
        // std::cout << "?????????????? "  << " ";
        goto err;
    }
    return 0;
}
