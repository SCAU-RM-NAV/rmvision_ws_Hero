#include <cv_bridge/cv_bridge.h>
#include <rmw/qos_profiles.h>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <image_transport/image_transport.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <rclcpp/duration.hpp>
#include <rclcpp/qos.hpp>

// STD
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "base_detector/base_light.hpp"
#include "base_detector/base_node.hpp"
#include "base_detector/base.hpp"

namespace rm_auto_aim
{
BaseDetectorNode::BaseDetectorNode(const rclcpp::NodeOptions & options)
: Node("base_detector", options)
{
    RCLCPP_INFO(this->get_logger(), "Starting base_detect");
    // Detector
    detector_ = initDetector();

    // Visualization Marker Publisher
    // Debug Publishers
    debug_ = this->declare_parameter("debug", true);
    if (debug_) {
    createDebugPublishers();
    }

    cam_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
    "/camera_info", rclcpp::SensorDataQoS(),
    [this](sensor_msgs::msg::CameraInfo::SharedPtr camera_info) {
    cam_center_ = cv::Point2f(camera_info->k[2], camera_info->k[5]);
    cam_info_ = std::make_shared<sensor_msgs::msg::CameraInfo>(*camera_info);
    cam_info_sub_.reset();
    });

    img_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      "/image_raw_green", rclcpp::ClockQoS(),
      std::bind(&BaseDetectorNode::imageCallback, this, std::placeholders::_1));

    gimbal_control_pub_ = this->create_publisher<auto_aim_interfaces::msg::GimbalCtrl>(
      "/gimbal_ctrl", rclcpp::SensorDataQoS());
}


void BaseDetectorNode::imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr img_msg)
{
  
  try {
    rclcpp::Time target_time = img_msg->header.stamp;


} catch (...) {
    std::cout << "Something Wrong when lookUpTransform!!!!! " << std::endl;
    return;
}
//   // update params
   //updateDetectorParams();

    auto bases = detectBases(img_msg);
    
    auto_aim_interfaces::msg::GimbalCtrl gimbal_ctrl_msg;
    if (bases.empty()) {
      RCLCPP_WARN(this->get_logger(), "No bases detected!");
      gimbal_ctrl_msg.aim_flag = 0;
      gimbal_control_pub_->publish(gimbal_ctrl_msg);
      return;
    }
    //std::cout<<"bases:"<<bases.size()<<std::endl;
    //for (auto & base : bases) {
      target_x = bases[0].center.x - 640;
      if(target_x >= 0)
      {
        gimbal_ctrl_msg.base_flag = 0;
      }
      else
      {
        gimbal_ctrl_msg.base_flag = 1;
      }
      //gimbal_ctrl_msg.empty = 1;
      gimbal_ctrl_msg.aim_flag = 1;
      gimbal_ctrl_msg.x = target_x;
      RCLCPP_WARN(this->get_logger(), "SBSBSBS!");
      //gimbal_control_pub_->publish(gimbal_ctrl_msg);
    //}
}


std::unique_ptr<Detector> BaseDetectorNode::initDetector()
{
  rcl_interfaces::msg::ParameterDescriptor param_desc;
  param_desc.integer_range.resize(1);
  param_desc.integer_range[0].step = 1;
  param_desc.integer_range[0].from_value = 0;
  param_desc.integer_range[0].to_value = 255;
  int binary_thres = this->declare_parameter("binary_thres", 160, param_desc);
  //this->declare_parameter("binary_thres_green", 160, param_desc);

  Detector::LightParams l_params = {
    .min_ratio = declare_parameter("light.min_ratio", 0.1),
    .max_ratio = declare_parameter("light.max_ratio", 0.8)};

  auto detector = std::make_unique<Detector>(binary_thres, l_params);

  return detector;
}

std::vector<Base> BaseDetectorNode::detectBases(const sensor_msgs::msg::Image::ConstSharedPtr& img_msg)
{
  // Convert ROS img to cv::Mat
  auto img = cv_bridge::toCvShare(img_msg, "rgb8")->image;//image是？
  //std::cout<<"1"<<std::endl;

  detector_->binary_thres = get_parameter("binary_thres").as_int();


  auto bases = detector_->detect(img);//为什么detector_可以指detect？

  auto final_time = this->now();
  auto latency = (final_time - img_msg->header.stamp).seconds() * 1000;

  RCLCPP_INFO_STREAM(
    this->get_logger(), 
    "Latency: " << latency << " ms"
);
  //std::cout<<"延迟"<<latency<<std::endl;
  detector_->drawResults(img);
  // Draw camera center
  cv::circle(img, cam_center_, 5, cv::Scalar(255, 0, 0), 2);
  // Draw latency
  std::stringstream latency_ss;
  latency_ss << "Latency: " << std::fixed << std::setprecision(2) << latency << "ms";
  auto latency_s = latency_ss.str();

  binary_img_pub_.publish(cv_bridge::CvImage(img_msg->header, "mono8", detector_->binary_img).toImageMsg());
  result_img_pub_.publish(cv_bridge::CvImage(img_msg->header, "rgb8", img).toImageMsg());
  return bases;
}

void BaseDetectorNode::createDebugPublishers()//debug用
{
  bases_data_pub_ =
    this->create_publisher<auto_aim_interfaces::msg::DebugBases>("/base/debug_lights", 10);

  binary_img_pub_ = image_transport::create_publisher(this, "/base/binary_img");
  result_img_pub_ = image_transport::create_publisher(this, "/base/result_img");
}

void BaseDetectorNode::destroyDebugPublishers()
{
  bases_data_pub_.reset();

  binary_img_pub_.shutdown();
  result_img_pub_.shutdown();
}


}

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(rm_auto_aim::BaseDetectorNode)