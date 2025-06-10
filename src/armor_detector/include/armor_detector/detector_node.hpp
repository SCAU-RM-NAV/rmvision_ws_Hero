// Copyright 2022 Chen Jun
// Licensed under the MIT License.

#ifndef ARMOR_DETECTOR__DETECTOR_NODE_HPP_
#define ARMOR_DETECTOR__DETECTOR_NODE_HPP_

// ROS
#include <image_transport/image_transport.hpp>
#include <image_transport/publisher.hpp>
#include <image_transport/subscriber_filter.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2_ros/buffer_interface.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>

#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rcl_interfaces/msg/set_parameters_result.hpp>
// STD
#include <memory>
#include <string>
#include <vector>

#include "std_msgs/msg/int8.hpp"
#include "armor_detector/detector.hpp"
#include "armor_detector/number_classifier.hpp"
#include "armor_detector/pnp_solver.hpp"
#include "auto_aim_interfaces/msg/armors.hpp"
#include "auto_aim_interfaces/msg/gimbal_fdb.hpp" 
#include "auto_aim_interfaces/msg/target.hpp"
#include "armor_detector/ba_solver.hpp"
#include "auto_aim_interfaces/msg/detections.hpp"
namespace rm_auto_aim
{

class ArmorDetectorNode : public rclcpp::Node
{
public:
  ArmorDetectorNode(const rclcpp::NodeOptions & options);

private:
  void pose_imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr img_msg);
  void imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr img_msg);
  void Pose_Inference(const sensor_msgs::msg::Image::ConstSharedPtr img_msg);
  void gimbalFdbCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg);
  void pose_back_back(const auto_aim_interfaces::msg::Detections Detections);
  void targetCallback(const auto_aim_interfaces::msg::Target::SharedPtr msg);

  // get RPY from rvec in imu frame
  double rvecToRPY(const cv::Mat & rvec, int axis) const noexcept;

  std::unique_ptr<Detector> initDetector();
  std::vector<Armor> detectArmors(const sensor_msgs::msg::Image::ConstSharedPtr & img_msg);

  void createDebugPublishers();
  void destroyDebugPublishers();
  void pnp(std::vector<Armor> armors,std_msgs::msg::Header header,Eigen::Matrix3d imu_to_camera_);
  void publishMarkers();

  // Armor Detector
  std::shared_ptr<Detector> detector_;

  // Detected armors publisher
  auto_aim_interfaces::msg::Armors armors_msg_;
  rclcpp::Publisher<auto_aim_interfaces::msg::Armors>::SharedPtr armors_pub_;
  rclcpp::Publisher<auto_aim_interfaces::msg::Detections>::SharedPtr detections_pub_;
  rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr test_pub_;
  rclcpp::Subscription<auto_aim_interfaces::msg::Detections>::SharedPtr detections_sub_;

  // Visualization marker publisher
  visualization_msgs::msg::Marker armor_marker_;
  visualization_msgs::msg::Marker text_marker_;
  visualization_msgs::msg::MarkerArray marker_array_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;

  // Camera info part
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr cam_info_sub_;
  cv::Point2f cam_center_;
  std::shared_ptr<sensor_msgs::msg::CameraInfo> cam_info_;


  //ran's CODE7.12
  /*
  
  std::vector<double> cam_mat;
  std::vector<double> dist_coeff;

  */
  rclcpp::Subscription<auto_aim_interfaces::msg::Target>::SharedPtr ba_target_sub_;
  std::unique_ptr<PnPSolver> pnp_solver_;
  std::unique_ptr<BaSolver> ba_solver_;

  bool use_ba_;
  bool all_ba_ = true;
  std::string ba_number;


  // Image subscrpition
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_sub_a;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_sub_b;


  // Target subscription
  // rclcpp::Subscription<rm_interfaces::msg::Target>::SharedPtr target_sub_;
  // rm_interfaces::msg::Target::SharedPtr tracked_target_;
  std::deque<Armor> tracked_armors_;

  // ReceiveData subscripiton
  std::string odom_frame_;
  Eigen::Matrix3d imu_to_camera_;
  std::shared_ptr<tf2_ros::Buffer> tf2_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
  geometry_msgs::msg::TransformStamped odom_to_gimbal;


  // GimbalFdb
  rclcpp::Subscription<auto_aim_interfaces::msg::GimbalFdb>::SharedPtr gimbal_fdb_sub_;

  // Debug information
  bool debug_;
  bool To_opencv;
  std::shared_ptr<rclcpp::ParameterEventHandler> debug_param_sub_;
  std::shared_ptr<rclcpp::ParameterCallbackHandle> debug_cb_handle_;
  rclcpp::Publisher<auto_aim_interfaces::msg::DebugLights>::SharedPtr lights_data_pub_;
  rclcpp::Publisher<auto_aim_interfaces::msg::DebugArmors>::SharedPtr armors_data_pub_;
  image_transport::Publisher binary_img_pub_;
  image_transport::Publisher number_img_pub_;
  image_transport::Publisher result_img_pub_;
  image_transport::Publisher pose_img_pub_;
  //ran's CODE7.12
  //image_transport::Publisher fixxed_img_pub_;

  void updateDetectorParams();
};

}  // namespace rm_auto_aim

#endif  // ARMOR_DETECTOR__DETECTOR_NODE_HPP_
