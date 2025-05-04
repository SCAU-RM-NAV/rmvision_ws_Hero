#ifndef BASE_DETECTOR__DETECTOR_NODE_HPP_
#define BASE_DETECTOR__DETECTOR_NODE_HPP_

// ROS
#include <image_transport/image_transport.hpp>
#include <image_transport/publisher.hpp>
#include <image_transport/subscriber_filter.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>

#include <rcl_interfaces/msg/set_parameters_result.hpp>

// STD
#include <memory>
#include <string>
#include <vector>

#include "base_detector/base_light.hpp"
#include "base_detector/base.hpp"
#include "auto_aim_interfaces/msg/base.hpp"
#include "auto_aim_interfaces/msg/navi_fdb.hpp" 
#include "auto_aim_interfaces/msg/gimbal_ctrl.hpp" 
#include "auto_aim_interfaces/msg/target.hpp"

namespace rm_auto_aim
{
class BaseDetectorNode : public rclcpp::Node
{
public:
  BaseDetectorNode(const rclcpp::NodeOptions & options);


private:
  void imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr img_msg);
  //void naviFdbCallback(const auto_aim_interfaces::msg::NaviFdb::SharedPtr msg);

  rclcpp::Publisher<auto_aim_interfaces::msg::GimbalCtrl>::SharedPtr gimbal_control_pub_;

  std::unique_ptr<Detector> initDetector();
  std::vector<Base> detectBases(const sensor_msgs::msg::Image::ConstSharedPtr & img_msg);

  void createDebugPublishers();
  void destroyDebugPublishers();

  //void publishMarkers();
  std::unique_ptr<Detector> detector_;

  // Camera info part
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr cam_info_sub_;
  cv::Point2f cam_center_;
  std::shared_ptr<sensor_msgs::msg::CameraInfo> cam_info_;

  // Image subscrpition
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_sub_;
  bool debug_;
  double target_x;

  //Navi subscripition
  //rclcpp::Subscription<auto_aim_interfaces::msg::NaviFdb>::SharedPtr navi_fdb_sub_;
  //std::unique_ptr<NaviTrajectory> trajectory_;
  //debug数据
  rclcpp::Publisher<auto_aim_interfaces::msg::DebugBases>::SharedPtr bases_data_pub_;
  image_transport::Publisher binary_img_pub_;
  image_transport::Publisher result_img_pub_;
  void updateDetectorParams();
};

}  // namespace rm_auto_aim_base

#endif  // BASE_DETECTOR__DETECTOR_NODE_HPP_