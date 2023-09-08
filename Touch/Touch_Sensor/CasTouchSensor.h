#pragma once

class CasTouchSensor
{

    public:
        void init(const char * serial);
        bool Open();
        float ReadData();
        void Close();

        ~CasTouchSensor()
        {
            delete [] device_path_;
        }
    private:
        const char* device_path_;
        int serial_fd_;
        
};