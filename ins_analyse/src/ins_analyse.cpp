#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <fstream>
#include <iomanip>
#include "ros/ros.h"
#include <fstream>
#include "preparse/INS_Data.h"
#include <rosbag/bag.h>
#include <rosbag/view.h>
std::vector<double> ins1,ins2;
std::vector<ros::Time> time1,time2,timeros;
void readDataFromBag(const std::string &bag_name){
    rosbag::Bag bag;
    try{
    bag.open(bag_name, rosbag::bagmode::Read);
  }
    catch (std::exception& e){
    std::cout << "ERROR!" << std::endl;
    exit(-1);
  }
    std::vector<std::string> topics;
    topics.push_back("/ins_data");
    topics.push_back("/ins_data2");
    topics.push_back("/clock");
//  std::cout<<topics.at(0)<<topics.at(1)<<topics.at(2)<<std::endl;

    rosbag::View view1(bag, rosbag::TopicQuery(topics.at(0)));
    for (rosbag::MessageInstance const m: view1){
        std::cout << m.getTime() << std::endl;
        preparse::INS_Data::ConstPtr ins_data=m.instantiate<preparse::INS_Data>();
        if(ins_data!=NULL){
            ins1.push_back(ins_data->gps_second);
            time1.push_back(ins_data->header.stamp);
//            printf("haha:%.3f\n",ins_data->gps_second);
            std::cout << ins_data->header.stamp << std::endl;
    }  
  }
    rosbag::View view2(bag, rosbag::TopicQuery(topics.at(1)));
    for (rosbag::MessageInstance const m: view2){
        ros::Time time_ros = m.getTime();
        timeros.push_back(time_ros);
//        std::cout << time_ros << std::endl;
//        std::cout << m.getTopic() << std::endl;
        preparse::INS_Data::ConstPtr ins_data=m.instantiate<preparse::INS_Data>();
        if(ins_data!=NULL){
            ins2.push_back(ins_data->gps_second);
            time2.push_back(ins_data->header.stamp);
//            printf("haha:%.3f\n",ins_data->gps_second);
//            std::cout << ins_data->nsv1 << std::endl;
        }
    }
    rosbag::View view3(bag, rosbag::TopicQuery(topics.at(2)));
    for (rosbag::MessageInstance const m: view3){
//        std::cout << m.getTopic() << std::endl;
//        ros::Time::ConstPtr time_ros=m.instantiate<ros::Time>();
//        if(ins_data!=NULL){
//            timeros.push_back(time_ros->header.stamp);
//            printf("haha:%.3f\n",ins_data->gps_second);
//            std::cout << time_ros->data << std::endl;
//        }
    }
    bag.close();
    return;
}
int main(int argc,char **argv) {
    ros::init(argc, argv, "ins_analyse");
//    ros::NodeHandle nh("~");
    std::string bagfile = "/home/usv/data/2021-01-27/Fusion-System-A/2021-01-27-09-46-39.bag";
    readDataFromBag(bagfile);
    std::cout <<ins1.size() << " " <<ins2.size() << std::endl;
    std::ofstream out;
    out.open("/home/usv/data/2021-01-27/Fusion-System-A/1.txt",std::ios::out);
    for (size_t i =0; i<ins1.size();i++){
//        printf("haha:%.2f\n",ins1[i]);
        out<< std::setiosflags(std::ios::fixed) << std::setprecision(2) << std::setfill('0')
            << ins1[i] << " " << ins2[i] << " " <<fabsf(ins1[i]-ins2[i]) << " "<< time1[i]
            << " " << time2[i] << " " << time1[i] - time2[i] <<std::endl;
    }
    out.close();
    std::cout << "ok";
//    ros::spin();
    return 0;
}