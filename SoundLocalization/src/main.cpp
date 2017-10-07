
#include "AsyncSerial.h"

#include <iostream>
#include <boost/thread.hpp>

#include <ros/ros.h>
#include <ros/package.h>


using namespace std;

void SoundLocalizationAnalysis(const char data[], unsigned int len)
{
    //   boost::mutex::scoped_lock lock(mMutex);
        unsigned char current_str=0x00;
            int i=0;
    for(i=0;i<len;i++)
    {
        current_str=data[i];
        unsigned int temp=(unsigned int)current_str;
        std::cout<<temp<<std::endl;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "SoundLocalization");
    ros::start();

    //获取串口参数
    std::string port;
    ros::param::param<std::string>("~port", port, "/dev/ttyUSB0");//SoundLocalization
    int baud;
    ros::param::param<int>("~baud", baud, 115200);
    //cout<<"port:"<<port<<" baud:"<<baud<<endl;
    // //获取参数
    // double separation=0,radius=0;
    // bool DebugFlag = false;
    // ros::param::param<double>("~wheel_separation", separation, 0.37);
    // ros::param::param<double>("~wheel_radius", radius, 0.0625);
    // ros::param::param<bool>("~debug_flag", DebugFlag, false);

    try {
        CallbackAsyncSerial serial(port,baud);
        serial.setCallback(boost::bind(SoundLocalizationAnalysis,_1,_2));
        // send test flag
        // char debugFlagCmd[] = {(char)0xcd, (char)0xeb, (char)0xd7, (char)0x01, 'T'};
        // if(DebugFlag){
        //   std::cout << "Debug mode Enabled" << std::endl;
        //   serial.write(debugFlagCmd, 5);
        // }
        // send reset cmd
        // char resetCmd[] = {(char)0xcd, (char)0xeb, (char)0xd7, (char)0x01, 'I'};
        // serial.write(resetCmd, 5);

        ros::Rate r(50);//发布周期为50hz
        while (ros::ok())
        {
            if(serial.errorStatus() || serial.isOpen()==false)
            {
                cerr<<"Error: serial port closed unexpectedly"<<endl;
                break;
            }
            // Refresh();//定时发布状态
            r.sleep();
        }

        quit:
        serial.close();

    } catch (std::exception& e) {
        cerr<<"Exception: "<<e.what()<<endl;
    }

    ros::shutdown();
    return 0;
}


