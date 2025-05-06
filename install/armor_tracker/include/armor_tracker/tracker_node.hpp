// Copyright 2022 Chen Jun

#ifndef ARMOR_PROCESSOR__PROCESSOR_NODE_HPP_
#define ARMOR_PROCESSOR__PROCESSOR_NODE_HPP_

#include <ceres/jet.h>

// ROS
#include <message_filters/subscriber.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/create_timer_ros.h>
#include <tf2_ros/message_filter.h>
#include <tf2_ros/transform_listener.h>

#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

// STD
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "armor_tracker/tracker.hpp"
#include "armor_tracker/common_tracker.hpp"
#include "auto_aim_interfaces/msg/armors.hpp"
#include "auto_aim_interfaces/msg/target.hpp"
#include "auto_aim_interfaces/msg/tracker_info.hpp"
#include "auto_aim_interfaces/msg/gimbal_ctrl.hpp"
#include "auto_aim_interfaces/msg/gimbal_fdb.hpp"
#include "armor_tracker/solve_trajectory.hpp"

namespace rm_auto_aim
{
using tf2_filter = tf2_ros::MessageFilter<auto_aim_interfaces::msg::Armors>;
using namespace std;

struct SpinMeasureFunc {
    /*
     * 工具函数的类封装, 整车状态估计观测方程
     */
    template<class T>
    void operator()(const T x[9], T y[4]) {

        T xc = x[0], yc = x[2], yaw = x[6], r = x[8];

        // 转换为球面坐标系
        T xyz[3];
        xyz[0] = xc - r * ceres::cos(yaw);  // xa
        xyz[1] = yc - r * ceres::sin(yaw);  // ya
        xyz[2] = x[4];                      // za

        T pyd[3];
        xyz2pyd(xyz, pyd);

        y[0] = pyd[0];    // pitch 
        y[1] = pyd[1];    // yaw
        y[2] = pyd[2];    // dis
        y[3] = x[6];      // yaw_armor
    }
};

struct CommonMeasureFunc {
    /*
     * 工具函数的类封装, 普通跟踪器观测方程
     */
    template<class T>
    void operator()(const T x[6], T y[3]) {

        // 转换为球面坐标系
        T xyz[3];
        xyz[0] = x[0];  // xa
        xyz[1] = x[2];  // ya
        xyz[2] = x[4];                      // za

        T pyd[3];
        xyz2pyd(xyz, pyd);

        y[0] = pyd[0];    // pitch 
        y[1] = pyd[1];    // yaw
        y[2] = pyd[2];    // dis
    }
};



class ArmorTrackerNode : public rclcpp::Node
{
public:
  explicit ArmorTrackerNode(const rclcpp::NodeOptions & options);

private:
  void armorsCallback(const auto_aim_interfaces::msg::Armors::SharedPtr armors_ptr);
  void gimbalFdbCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg);

  void publishMarkers(const auto_aim_interfaces::msg::Target & target_msg);

  // Maximum allowable armor distance in the XOY plane
  double max_armor_distance_;

  // The time when the last message was received
  rclcpp::Time last_time_;
  double dt_;

  // Armor spin tracker 
  double spin_s2qxyz_, spin_s2qyaw_, spin_s2qr_;
  double spin_r_py, spin_r_dis_factor, spin_r_yaw;

  // Armor com tracker 
  double com_s2qxyz_, com_s2qyaw_, com_s2qr_;
  double com_r_py, com_r_dis_factor, com_r_yaw;

  double lost_time_thres_;
  std::unique_ptr<Tracker> spin_tracker_;
  std::unique_ptr<CommonTracker> com_tracker_;

  // Reset tracker service
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr reset_tracker_srv_;

  // Subscriber with tf2 message_filter
  std::string target_frame_;
  std::shared_ptr<tf2_ros::Buffer> tf2_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
  message_filters::Subscriber<auto_aim_interfaces::msg::Armors> armors_sub_;
  // rclcpp::Subscription<auto_aim_interfaces::msg::Armors>::SharedPtr armors_sub_;
  std::shared_ptr<tf2_filter> tf2_filter_;

  // Tracker info publisher
  rclcpp::Publisher<auto_aim_interfaces::msg::TrackerInfo>::SharedPtr info_pub_;

  // Publisher
  rclcpp::Publisher<auto_aim_interfaces::msg::Target>::SharedPtr target_pub_;
  rclcpp::Publisher<auto_aim_interfaces::msg::GimbalCtrl>::SharedPtr gimbal_control_pub_;

  rclcpp::Subscription<auto_aim_interfaces::msg::GimbalFdb>::SharedPtr tracker_gimbal_fdb_sub_;

  // Visualization marker publisher
  visualization_msgs::msg::Marker position_marker_;
  visualization_msgs::msg::Marker linear_v_marker_;
  visualization_msgs::msg::Marker armor_position_marker_;
  visualization_msgs::msg::Marker armor_linear_v_marker_;
  visualization_msgs::msg::Marker angular_v_marker_;
  visualization_msgs::msg::Marker armor_marker_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;

  //solve_trajectory
  std::unique_ptr<SolveTrajectory> solve_trajectory_;
  // rclcpp::Publisher<auto_aim_interfaces::msg::PitchYaw>::SharedPtr send_pitch_yaw_pub_;
  visualization_msgs::msg::Marker aim_marker_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr aim_marker_pub_;

  double shoot_yaw_tole_;
  double choose_armor_shoot_yaw_tole;
  int priority_number;
  bool have_priroity_number = false;
};

}  // namespace rm_auto_aim

#endif  // ARMOR_PROCESSOR__PROCESSOR_NODE_HPP_
