// Copyright 2022 Chen Jun

#ifndef ARMOR_PROCESSOR__TRACKER_HPP_
#define ARMOR_PROCESSOR__TRACKER_HPP_

#include <chrono>

// Eigen
#include <Eigen/Eigen>

// ROS
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <angles/angles.h>

// STD
#include <memory>
#include <string>

#include "armor_tracker/extended_kalman_filter.hpp"
#include "auto_aim_interfaces/msg/armors.hpp"
#include "auto_aim_interfaces/msg/target.hpp"

namespace rm_auto_aim
{

enum class ArmorsNum { NORMAL_4 = 4, BALANCE_2 = 2, OUTPOST_3 = 3 };

class Tracker
{
public:
  Tracker(double max_match_distance, double max_match_yaw_diff, double yaw_filter_alpha);

  using Armors = auto_aim_interfaces::msg::Armors;
  using Armor = auto_aim_interfaces::msg::Armor;

  void init(const Armors::SharedPtr & armors_msg);

  void update(const Armors::SharedPtr & armors_msg);

  void tracker_priority_number(const Armors::SharedPtr & armors_msg,int number);

  ExtendedKalmanFilter ekf;

  int tracking_thres;
  int lost_thres;

  enum State {
    LOST,
    DETECTING,
    TRACKING,
    TEMP_LOST,
  } tracker_state;

  std::string tracked_id;
  Armor tracked_armor;
  ArmorsNum tracked_armors_num;
  ArmorsNum last_tracked_armors_num;

  double info_position_diff;
  double info_yaw_diff;

  Eigen::VectorXd measurement;

  Eigen::VectorXd target_state;

  // To store another pair of armors message
  double dz, another_r;

  bool armor_jump;
  int jump_count_;// 记录切装甲板的次数
  int find_hero_=0; //


  int find_hero_count = 0;

  Eigen::Vector3d getArmorPositionFromState(const Eigen::VectorXd & x);//放在public为了在trackernode调用

private:
  void initEKF(const Armor & a);

  void updateArmorsNum(const Armor & a);

  void handleArmorJump(const Armor & a);

  double orientationToYaw(const geometry_msgs::msg::Quaternion & q);

  double max_match_distance_;
  double max_match_yaw_diff_;
  double yaw_filter_alpha_; // yaw低通滤波系数

  int detect_count_;
  int lost_count_;

  double last_yaw_; // Make yaw change continuous
  double last_measure_yaw_;  // 用于低通滤波器
};

}  // namespace rm_auto_aim

#endif  // ARMOR_PROCESSOR__TRACKER_HPP_
