// Copyright 2024 GuoHao Shen

#include "armor_tracker/common_tracker.hpp"

#include <rclcpp/logger.hpp>

namespace rm_auto_aim
{
CommonTracker::CommonTracker(double max_match_distance)
: tracked_id(std::string("")),
  max_match_distance_(max_match_distance)
{
  // Set initial position
  target_state = Eigen::VectorXd::Zero(6);
  target_state << 0, 0, 0, 0, 0, 0;
  ekf.setState(target_state);
}

void CommonTracker::init(const Armor & a)
{
  RCLCPP_INFO(rclcpp::get_logger("armor_tracker"), "Init CommonTracker!");
  initEKF(a);
}

void CommonTracker::initEKF(const Armor & a)
{
  RCLCPP_INFO(rclcpp::get_logger("armor_tracker"), "Init CommonTracker EKF!");
  target_state = Eigen::VectorXd::Zero(6);

  double xa = a.pose.position.x;
  double ya = a.pose.position.y;
  double za = a.pose.position.z;

  // Set initial position
  target_state << xa, 0, ya, 0, za, 0;
  ekf.setState(target_state);
}

void CommonTracker::update(const Armor & a)
{
  // KF predict
  Eigen::VectorXd ekf_prediction = ekf.predict();
  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "ekf_prediction(0) : " << ekf_prediction(0));

  auto p = a.pose.position;

  Eigen::Vector3d position_vec(p.x, p.y, p.z);
  Eigen::Vector3d predicted_position(ekf_prediction(0), ekf_prediction(2), ekf_prediction(4));
  double position_diff = (predicted_position - position_vec).norm();
  if (position_diff > max_match_distance_)
  {
    initEKF(a);
    return;
  }

  double measure_pyd[3], xyz[3] = {p.x, p.y, p.z};
  xyz2pyd(xyz, measure_pyd);   // 将xyz坐标转换为球面坐标系
  measurement = Eigen::Vector3d(measure_pyd[0], measure_pyd[1], measure_pyd[2]);  // pit, yaw, dis, yaw_armor
  target_state = ekf.update(measurement);
  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "update target_state(0) : " << target_state(0));
  RCLCPP_DEBUG(rclcpp::get_logger("armor_tracker"), "CommonTracker EKF update");
}

}