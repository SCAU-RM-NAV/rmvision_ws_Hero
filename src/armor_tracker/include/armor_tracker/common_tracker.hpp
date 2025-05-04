// Copyright 2024 GuoHao Shen

#ifndef ARMOR_PROCESSOR__COMMONTRACKER_HPP_
#define ARMOR_PROCESSOR__COMMONTRACKER_HPP_

#include <chrono>
#include <rclcpp/rclcpp.hpp>

// Eigen
#include <Eigen/Eigen>

#include <geometry_msgs/msg/point.hpp>
#include <angles/angles.h>

#include "armor_tracker/extended_kalman_filter.hpp"
#include "auto_aim_interfaces/msg/armors.hpp"
#include "auto_aim_interfaces/msg/target.hpp"

namespace rm_auto_aim
{

class CommonTracker
{
public:
  CommonTracker(double max_match_distance);

  using Armors = auto_aim_interfaces::msg::Armors;
  using Armor = auto_aim_interfaces::msg::Armor;

  void init(const Armor & a);

  void update(const Armor & a);

  ExtendedKalmanFilter ekf;

  Eigen::VectorXd measurement;

  Eigen::VectorXd target_state;

private:
  std::string tracked_id;
  double max_match_distance_;

  void initEKF(const Armor & a);
};

}  // namespace rm_auto_aim

#endif  // ARMOR_PROCESSOR__COMMONTRACKER_HPP_