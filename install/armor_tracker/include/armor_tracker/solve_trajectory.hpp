#ifndef SOLVE_ANGLE_HPP_
#define SOLVE_ANGLE_HPP_

#define GRAVITY 9.78
#define CV_PI 3.1415
typedef unsigned char uint8_t;


#include <string>
#include <math.h>
#include <stdio.h>
#include <iostream>

#include <rclcpp/rclcpp.hpp>
#include <angles/angles.h>
#include "auto_aim_interfaces/msg/target.hpp"
#include "auto_aim_interfaces/msg/gimbal_fdb.hpp" 
#include "auto_aim_interfaces/msg/navi_fdb.hpp"

namespace rm_auto_aim
{

enum ARMOR_ID
{
    ARMOR_HERO = 1,
    ARMOR_ENGINEER = 2,
    ARMOR_INFANTRY3 = 3,
    ARMOR_INFANTRY4 = 4,
    ARMOR_INFANTRY5 = 5,
    ARMOR_GUARD = 6,
    ARMOR_OUTPOST =7,
    ARMOR_BASE = 8
};

enum ARMOR_NUM
{
    ARMOR_NUM_BALANCE = 2,
    ARMOR_NUM_OUTPOST = 3,
    ARMOR_NUM_NORMAL = 4
};

enum BULLET_TYPE
{
    BULLET_17 = 0,
    BULLET_42 = 1
};

//设置参数
struct SolveTrajectoryParams
{
    float k;             //弹道系数

    //自身参数
    enum BULLET_TYPE bullet_type;  //自身机器人类型 0-步兵 1-英雄
    float current_v;      //当前弹速
    float current_pitch;  //当前pitch
    float current_yaw;    //当前yaw

    //目标参数
    float xw;             //ROS坐标系下的x
    float yw;             //ROS坐标系下的y
    float zw;             //ROS坐标系下的z
    float vxw;            //ROS坐标系下的vx
    float vyw;            //ROS坐标系下的vy
    float vzw;            //ROS坐标系下的vz
    float axw;             //ROS坐标系下装甲板的x
    float ayw;             //ROS坐标系下装甲板的y
    float azw;             //ROS坐标系下装甲板的z
    float avxw;            //ROS坐标系下装甲板的vx
    float avyw;            //ROS坐标系下装甲板的vy
    float avzw;            //ROS坐标系下装甲板的vz
    float ayaw;            ////ROS坐标系下装甲板的ayaw
    float tar_yaw;        //目标yaw
    float v_yaw;          //目标yaw速度
    float r1;             //目标中心到前后装甲板的距离
    float r2;             //目标中心到左右装甲板的距离
    float dz;             //另一对装甲板的相对于被跟踪装甲板的高度差
    float vyaw;

    int bias_time;        //偏置时间
    int bias_time_com;
    float s_bias;         //枪口前推的距离
    float z_bias;         //yaw轴电机到枪口水平面的垂直距离
    float pitch_bias;     //gimbal_link到枪口pitch轴偏置
    enum ARMOR_ID armor_id;     //装甲板类型  6-哨兵 7-前哨站outpost 8-基地base                                
    //1-英雄 2-工程 3-4-5-步兵 
    enum ARMOR_NUM armor_num;   //装甲板数字  2-balance 3-outpost 4-normal

    float armor_jump_thre;   //装甲板切换阈值，限制反复切换目标装甲板，单位ms

};

//用于存储目标装甲板的信息
struct tar_pos
{
    float x;           //装甲板在世界坐标系下的x
    float y;           //装甲板在世界坐标系下的y
    float z;           //装甲板在世界坐标系下的z
    float yaw;         //装甲板坐标系相对于世界坐标系的yaw角
};

class SolveTrajectory : public rclcpp::Node
{
public:
    explicit SolveTrajectory(rclcpp::Time time);

    struct SolveTrajectoryParams st;
    struct tar_pos tar_position[4]; //最多只有四块装甲板
    float t = 0.5f; // 飞行时间
    double spin_count_ = 0;  // 角速度>阈值num 
    bool is_spin;
    bool selective_hitting;

    double shoot_yaw;
    double shoot_yaw_ratio;

    int last_idx = 0;  // 上一次选择的装甲板
    rclcpp::Time last_armor_jump_time; // 上一次切换装甲板时间
    float last_yaw = 0; // 上一帧目标云台yaw角
    float last_vyaw = 0; 

    float aim_x = 0, aim_y = 0, aim_z = 0; // aim point 落点，传回上位机用于可视化
    float pitch = 0; //输出控制量 pitch绝对角度 弧度
    float yaw = 0;   //输出控制量 yaw绝对角度 弧度
    int fire_flag = 1; //用于判断平步是否进入状态，模拟开火
    int control_diff_mode;//旋转选板后是否改变发枪角度
    int choose_armor_time = 0;
    double last_yv;
    double selective_hitting_yaw;  //装甲板在车体上的角度（角度）
    double linear_v;//击打目标的线速度
    double aim_distance;//击打目标离发射器的距离
    bool is_infantry = false;
    bool aim_center = false;  //判断是否该击打中心
    double linear_v_max = 0.7;  //小陀螺下可接受击打目标自身的线速度最大值
    double last_aim_x;
    double last_aim_y;
    double last_aim_z;
    // int center_error;//锁中心错误次数
    double aim_center_yaw_error = 2.0; //可击打yaw角范围的忍受误差
    int fire_center_time = 0;//用于记录是否真的可以击发，防止预测失误带来误判
    int aim_center_last_time = 0;//🔓中心持续时间
    int stop_aim_center = 0;

    int R_K_iter = 60; //步长 
    float temp_pitch;

    
    void gimbalCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg);
    void naviCallback(const auto_aim_interfaces::msg::NaviFdb::SharedPtr msg);
    rclcpp::Subscription<auto_aim_interfaces::msg::GimbalFdb>::SharedPtr gimbal_fdb_sub_;
    rclcpp::Subscription<auto_aim_interfaces::msg::NaviFdb>::SharedPtr navi_fdb_sub_;

    int aiming_mode=0;
    float z;
    float x;
    //单方向空气阻力模型
    float monoDirectionalAirResistanceModel(float s, float v,float angle);
    //完全空气阻力模型
    float completeAirResistanceModel(float s, float v, float angle);
    //pitch弹道补偿
    float pitchTrajectoryCompensation(float s, float y, float v);
    //根据最优决策得出被击打装甲板 自动解算弹道
    void autoSolveTrajectory(const auto_aim_interfaces::msg::Target & target_msg, const double calculate_delay);
};

} //namespace

#endif // SOLVE_ANGLE_HPP_ARN