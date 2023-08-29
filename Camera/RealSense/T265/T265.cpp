#include "T265.h"
#include "../../../Smw/utility/ini_file.hpp"
#include "../../../Smw/utility/singleton.hpp"
#include <iostream>
// #include "realsenseopensourcelib/example.h"

using namespace std;
using namespace SMW;
using namespace rs2;

bool RealSenseT265::Init(std::string &configPath)
{
    std::cout << "RealSense T265 Init" << std::endl; 
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(configPath);
    return true;
}

int RealSenseT265::OpenDevice()
{

    std::cout << "RealSenseD435 OpenDevice" << std::endl;
    // 使能 左右目图像数据
    cfg.enable_stream(RS2_STREAM_FISHEYE,1, RS2_FORMAT_Y8);
    cfg.enable_stream(RS2_STREAM_FISHEYE,2, RS2_FORMAT_Y8);
    // 使能 传感器的POSE和6DOF IMU数据
    cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
    //启动管道
    pipe.start(cfg);
}

int RealSenseT265::GetFrameData(std::vector<DataBase*> &data)
{
     // 构建D435数据结构
    if (data.size() < 1)
        data.push_back(new T265ComposeFrame());
    dataframe = pipe.wait_for_frames();

    T265ComposeFrame* T265 = dynamic_cast<T265ComposeFrame*>(data[0]);

	// Get a frame from the pose stream
	auto f = dataframe.first_or_default(RS2_STREAM_POSE);
	auto pose = f.as<rs2::pose_frame>().get_pose_data();


    T265->pose_data->translation_x = pose.translation.x;
    T265->pose_data->translation_y = pose.translation.y;
    T265->pose_data->translation_z = pose.translation.z;

    T265->pose_data->velocity_x = pose.velocity.x;
    T265->pose_data->velocity_y = pose.velocity.y;
    T265->pose_data->velocity_z = pose.velocity.z;

    T265->imu_data->gyroscope_x = pose.angular_velocity.x;
    T265->imu_data->gyroscope_y = pose.angular_velocity.y;
    T265->imu_data->gyroscope_z = pose.angular_velocity.z;

    T265->imu_data->accelerometer_x = pose.acceleration.x;
    T265->imu_data->accelerometer_y = pose.acceleration.y;
    T265->imu_data->accelerometer_z = pose.acceleration.z;


	// cout<<"px: "<<pose.translation.x<<"   py: "<<pose.translation.y<<"   pz: "<<pose.translation.z<<
	// "vx: "<<pose.velocity.x<<"   vy: "<<pose.velocity.y<<"   vz: "<<pose.velocity.z<<endl;


	// cout<<"ax: "<<pose.acceleration.x<<"   ay: "<<pose.acceleration.y<<"   az: "<<pose.acceleration.z<<
	// "gx: "<<pose.angular_velocity.x<<"   gy: "<<pose.angular_velocity.y<<"   gz: "<<pose.angular_velocity.z<<endl;



    image_left = dataframe.get_fisheye_frame(1);
    image_right = dataframe.get_fisheye_frame(2);

    T265->image_left->data = (void*)image_left.get_data();
    T265->image_right->data = (void*)image_right.get_data();

    int width_left = image_left.as<rs2::video_frame>().get_width();   // 图像宽度
    int height_left = image_left.as<rs2::video_frame>().get_height(); // 图像高度

    int width_right = image_right.as<rs2::video_frame>().get_width();   // 图像宽度
    int height_right = image_right.as<rs2::video_frame>().get_height(); // 图像高度

    T265->image_left->width =  width_left;
    T265->image_left->height =  height_left;
    T265->image_left->size =  height_left * width_left;

    T265->image_right->width =  width_right;
    T265->image_right->height =  height_right;
    T265->image_right->size =  width_right * height_right;


    // 用于释放设备资源
    ctx.query_devices();
}
int RealSenseT265::CloseDevice()
{
    std::cout << "RealSenseD435 CloseDevice" << std::endl;
    pipe.stop();
}

