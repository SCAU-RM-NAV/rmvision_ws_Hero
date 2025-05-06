#ifndef ARMOR_CREATER_NODE_HPP_
#define ARMOR_CREATER_NODE_HPP_

#include <deque>

// ROS
#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <geometry_msgs/msg/quaternion.hpp>

#include "auto_aim_interfaces/msg/armors.hpp"
#include "auto_aim_interfaces/msg/target.hpp"
#include "armor_creater/robot_simulation.hpp"

#include "UdpSend.hpp"

namespace rm_auto_aim
{

class ArmorCreaterNode : public rclcpp::Node
{
private:
  std::unique_ptr<RobotSumulation> robot_sumulation_;

  rclcpp::TimerBase::SharedPtr timer_;
  double freq_;

  // std::shared_ptr<auto_aim_interfaces::msg::Target> creater_ = std::make_shared<auto_aim_interfaces::msg::Target>(42);
  std::deque<auto_aim_interfaces::msg::Target::SharedPtr> creater_deque_;
  std::deque<auto_aim_interfaces::msg::Target::SharedPtr> tracker_deque_;

  rclcpp::Publisher<auto_aim_interfaces::msg::Armors>::SharedPtr armors_pub_;
  rclcpp::Publisher<auto_aim_interfaces::msg::Target>::SharedPtr robot_state_pub_;


  // Visualization state marker publisher
  visualization_msgs::msg::Marker position_marker_;
  visualization_msgs::msg::Marker linear_v_marker_;
  visualization_msgs::msg::Marker angular_v_marker_;
  visualization_msgs::msg::Marker armor_marker_;
  visualization_msgs::msg::Marker measure_marker_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr real_marker_pub_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr measure_marker_pub_;

  rclcpp::Subscription<auto_aim_interfaces::msg::Target>::SharedPtr target_sub_;

public:
  ArmorCreaterNode(const rclcpp::NodeOptions & options);

  void timerCallback();

  void targetCallback(const auto_aim_interfaces::msg::Target::SharedPtr target_msg);

  void updateParams();

  void publishStateMarkers(Eigen::VectorXd & state);

  void publishMeasureMarkers(auto_aim_interfaces::msg::Armor armor);

  // 工具函数，从ros话题数据获取时间
  double get_time_sec(const builtin_interfaces::msg::Time &time)
  {
      return rclcpp::Time(time).seconds();
  }

  // 计算均方误差
  std::vector<double> meanSquareError(const std::deque<auto_aim_interfaces::msg::Target::SharedPtr>& actual, const std::deque<auto_aim_interfaces::msg::Target::SharedPtr>& predicted);

  UDPSender creater_sender;
  VofaData creater_vofa_data;
};

}

#endif // ARMOR_CREATER_NODE_HPP_