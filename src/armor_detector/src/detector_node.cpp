// Copyright 2022 Chen Jun
// Licensed under the MIT License.

#include <cv_bridge/cv_bridge.h>
#include <rmw/qos_profiles.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/convert.h>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <image_transport/image_transport.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <rclcpp/duration.hpp>
#include <rclcpp/qos.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/time.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/create_timer_ros.h>
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

#include "armor_detector/armor.hpp"
#include "armor_detector/detector_node.hpp"
#include "armor_detector/utils.hpp"
#include "auto_aim_interfaces/msg/detections.hpp"

namespace rm_auto_aim
{
ArmorDetectorNode::ArmorDetectorNode(const rclcpp::NodeOptions & options)
: Node("armor_detector", options)
{
  RCLCPP_INFO(this->get_logger(), "Starting DetectorNode!");
  //创建pose pub
  rclcpp::QoS qos(10);
  detections_pub_ = this->create_publisher<auto_aim_interfaces::msg::Detections>(
    "/detector/detections", rclcpp::SensorDataQoS());


  // 初始化
  detector_ = initDetector();

  use_ba_ = this->declare_parameter("use_ba", true);

  // 创建Armors Publisher
  armors_pub_ = this->create_publisher<auto_aim_interfaces::msg::Armors>(
    "/detector/armors", rclcpp::SensorDataQoS());

  odom_frame_ = this->declare_parameter("target_frame", "world");
  
  imu_to_camera_ = Eigen::Matrix3d::Identity();

  // Visualization Marker Publisher
  // See http://wiki.ros.org/rviz/DisplayTypes/Marker
  armor_marker_.ns = "armors";
  armor_marker_.action = visualization_msgs::msg::Marker::ADD;
  armor_marker_.type = visualization_msgs::msg::Marker::CUBE;
  armor_marker_.scale.x = 0.05;
  armor_marker_.scale.z = 0.125;
  armor_marker_.color.a = 1.0;
  armor_marker_.color.g = 0.5;
  armor_marker_.color.b = 1.0;
  armor_marker_.lifetime = rclcpp::Duration::from_seconds(0.1);

  text_marker_.ns = "classification";
  text_marker_.action = visualization_msgs::msg::Marker::ADD;
  text_marker_.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
  text_marker_.scale.z = 0.1;
  text_marker_.color.a = 1.0;
  text_marker_.color.r = 1.0;
  text_marker_.color.g = 1.0;
  text_marker_.color.b = 1.0;
  text_marker_.lifetime = rclcpp::Duration::from_seconds(0.1);

  marker_pub_ =
    this->create_publisher<visualization_msgs::msg::MarkerArray>("/detector/marker", 10);

  test_pub_ = this->create_publisher<std_msgs::msg::Int8>("/detector/test", 10);

  To_opencv = this->declare_parameter("To_opencv", true);
  // Debug Publishers
  debug_ = this->declare_parameter("debug", true);
  if (debug_) {
    createDebugPublishers();
  }

  // Debug param change moniter
  debug_param_sub_ = std::make_shared<rclcpp::ParameterEventHandler>(this);
  debug_cb_handle_ =
    debug_param_sub_->add_parameter_callback("debug", [this](const rclcpp::Parameter & p) {
      debug_ = p.as_bool();
      debug_ ? createDebugPublishers() : destroyDebugPublishers();
    });

  ba_target_sub_ = this->create_subscription<auto_aim_interfaces::msg::Target>(
      "/tracker/target", rclcpp::SensorDataQoS(), 
      std::bind(&ArmorDetectorNode::targetCallback, this, std::placeholders::_1));

  cam_info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
    "/camera_info", rclcpp::SensorDataQoS(),
    [this](sensor_msgs::msg::CameraInfo::SharedPtr camera_info) {
      cam_center_ = cv::Point2f(camera_info->k[2], camera_info->k[5]);
      cam_info_ = std::make_shared<sensor_msgs::msg::CameraInfo>(*camera_info);
      pnp_solver_ = std::make_unique<PnPSolver>(camera_info->k, camera_info->d);
      ba_solver_ = std::make_unique<BaSolver>(camera_info->k, camera_info->d);
      cam_info_sub_.reset();
    });


    img_sub_a = this->create_subscription<sensor_msgs::msg::Image>(
    "/image_raw_a", rclcpp::SensorDataQoS(),
    std::bind(&ArmorDetectorNode::pose_imageCallback, this, std::placeholders::_1));//短焦输入

    img_sub_b = this->create_subscription<sensor_msgs::msg::Image>(
    "/image_raw_b", rclcpp::SensorDataQoS(),
    std::bind(&ArmorDetectorNode::pose_imageCallback, this, std::placeholders::_1));//长焦输入

  gimbal_fdb_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalFdb>(
    "gimbal_fdb", rclcpp::SensorDataQoS(),
    std::bind(&ArmorDetectorNode::gimbalFdbCallback, this, std::placeholders::_1));

   detections_sub_= this->create_subscription<auto_aim_interfaces::msg::Detections>(
    "/detector/detections", rclcpp::SensorDataQoS(),
    std::bind(&ArmorDetectorNode::pose_back_back, this, std::placeholders::_1)); 

  tf2_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
  auto timer_interface = std::make_shared<tf2_ros::CreateTimerROS>(
    this->get_node_base_interface(), this->get_node_timers_interface());
  tf2_buffer_->setCreateTimerInterface(timer_interface);
  tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
}

void ArmorDetectorNode::gimbalFdbCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg)
{
  auto detect_color = get_parameter("detect_color").as_int();
  int enemy_color = msg->camp == RED ? BLUE : RED;
  if (detect_color == AUTO && detector_->detect_color != enemy_color) 
  {  
    detector_->detect_color = enemy_color;
    RCLCPP_INFO_STREAM(this->get_logger(), "detect_color update : " << detector_->detect_color);
  }
  
}

/**
 * @brief 相机图像订阅回调函数，进行装甲板检测，pnp解算，装甲板话题发布
 * @param img_msg 图像数据
 */

void ArmorDetectorNode::pose_imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr img_msg)
{
  To_opencv = get_parameter("To_opencv").as_bool();
  if(To_opencv)
  {
  imageCallback(img_msg);
  }
  else{
//std::cout<<detector_->train <<" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ";
  // 寻找从相机坐标系到世界坐标系的旋转变换矩阵
  try {
    rclcpp::Time target_time = img_msg->header.stamp;
    auto odom_to_gimbal = tf2_buffer_->lookupTransform(
      odom_frame_, "camera_optical_frame", target_time, rclcpp::Duration::from_seconds(0.01));
    auto msg_q = odom_to_gimbal.transform.rotation;
    tf2::Quaternion tf_q;
    tf2::fromMsg(msg_q, tf_q);
    tf2::Matrix3x3 tf2_matrix = tf2::Matrix3x3(tf_q);
    imu_to_camera_ << tf2_matrix.getRow(0)[0], tf2_matrix.getRow(0)[1], tf2_matrix.getRow(0)[2],
      tf2_matrix.getRow(1)[0], tf2_matrix.getRow(1)[1], tf2_matrix.getRow(1)[2],
      tf2_matrix.getRow(2)[0], tf2_matrix.getRow(2)[1], tf2_matrix.getRow(2)[2];
  } catch (...) {
    std::cout << "Something Wrong when lookUpTransform!!!!! " << std::endl;
    return;
  }

  // 更新参数
  updateDetectorParams();
  
  this->Pose_Inference(img_msg);
  }


}

void ArmorDetectorNode::imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr img_msg)
{
  std_msgs::msg::Int8 test_msg;
  test_msg.data = (int8_t)123;
  test_pub_->publish(test_msg);
  // 寻找从相机坐标系到世界坐标系的旋转变换矩阵
  try {
    rclcpp::Time target_time = img_msg->header.stamp;
    auto odom_to_gimbal = tf2_buffer_->lookupTransform(
      odom_frame_, "camera_optical_frame", target_time, rclcpp::Duration::from_seconds(0.01));
    auto msg_q = odom_to_gimbal.transform.rotation;
    tf2::Quaternion tf_q;
    tf2::fromMsg(msg_q, tf_q);
    tf2::Matrix3x3 tf2_matrix = tf2::Matrix3x3(tf_q);
    imu_to_camera_ << tf2_matrix.getRow(0)[0], tf2_matrix.getRow(0)[1], tf2_matrix.getRow(0)[2],
      tf2_matrix.getRow(1)[0], tf2_matrix.getRow(1)[1], tf2_matrix.getRow(1)[2],
      tf2_matrix.getRow(2)[0], tf2_matrix.getRow(2)[1], tf2_matrix.getRow(2)[2];
  } catch (...) {
    std::cout << "Something Wrong when lookUpTransform!!!!! " << std::endl;
    return;
  }

  // 更新参数
  updateDetectorParams();

  // 计算延迟
  auto recv_time = this->now();
  auto latency = (recv_time - img_msg->header.stamp).seconds() * 1000;
  RCLCPP_INFO_STREAM(this->get_logger(), "Received image, latency: " << latency << "ms");


  // 检测装甲板
  auto armors = detectArmors(img_msg);

  // 进行pnp解算
  if (pnp_solver_ != nullptr)
  {
    // 同步消息头
    armors_msg_.header = armor_marker_.header = text_marker_.header = img_msg->header;

    armors_msg_.armors.clear();
    marker_array_.markers.clear();
    armor_marker_.id = 0;
    text_marker_.id = 0;

    auto_aim_interfaces::msg::Armor armor_msg;
    for (auto& armor : armors)
    {
      cv::Mat rvec(3, 1, CV_64F), tvec(3, 1, CV_64F);
      cv::Mat rotation_matrix(3, 3, CV_64F);

      // 进行pnp解算
      bool success = pnp_solver_->solvePnP(armor, rvec, tvec);
      
      // 获取装甲板世界坐标系下绕中心旋转的弧度
      armor.roll = rvecToRPY(rvec, 0) * 180 / M_PI;
      armor.imu2camera = imu_to_camera_;

      // 将旋转向量转为旋转矩阵（这里是相机坐标系到装甲板坐标系的旋转）
      cv::Rodrigues(rvec, rotation_matrix);
      armor.rmat = rotation_matrix.clone();
      armor.tvec = tvec.clone();

      // 对装甲板的旋转进行ba优化，要求当前tracker正在跟随装甲板的id和要处理的装甲板相同
      if (use_ba_  && (ba_number == armor.number || all_ba_ ==true))  {
        // 只优化差不多平行与地面的装甲板
        if (std::abs(armor.roll) < 25) {
          tracked_armors_.push_back(armor);
        }

        // 最初尝试通过多帧装甲板（多帧 BA 优化）提高精度，但发现效果不理想。
        // 多帧可能会引入历史误差的累积，特别是在目标移动快速或者观测误差较大的情况下。
        // 故限制，保证 tracked_armors_ 队列中的装甲板数量最多为 1
        if (tracked_armors_.size() > 1) {
          tracked_armors_.pop_front();
        }

        // 使用BA算法优化pnp求解到的旋转矩阵
        ba_solver_->solveBa(tracked_armors_, rotation_matrix);
      }

      if (success)
      {
        // 将解算结果填入消息体中
        armor_msg.type = ARMOR_TYPE_STR[static_cast<int>(armor.type)];
        armor_msg.number = armor.number;
        armor_msg.pose.position.x = tvec.at<double>(0);
        armor_msg.pose.position.y = tvec.at<double>(1);
        armor_msg.pose.position.z = tvec.at<double>(2);
        // rvec to 3x3 rotation matrix
        // cv::Mat rotation_matrix;
        // cv::Rodrigues(rvec, rotation_matrix);
        // rotation matrix to quaternion
        tf2::Matrix3x3 tf2_rotation_matrix(
          rotation_matrix.at<double>(0, 0), rotation_matrix.at<double>(0, 1),
          rotation_matrix.at<double>(0, 2), rotation_matrix.at<double>(1, 0),
          rotation_matrix.at<double>(1, 1), rotation_matrix.at<double>(1, 2),
          rotation_matrix.at<double>(2, 0), rotation_matrix.at<double>(2, 1),
          rotation_matrix.at<double>(2, 2));
        tf2::Quaternion tf2_q;
        tf2_rotation_matrix.getRotation(tf2_q);
        armor_msg.pose.orientation = tf2::toMsg(tf2_q);

        // 计算装甲板中心到图像中心的距离
        armor_msg.distance_to_image_center = pnp_solver_->calculateDistanceToCenter(armor.center);
        armor_msg.armor_ratio = armor.armor_ratio;
        
        // 装甲板，数字识别文本可视化
        armor_marker_.id++;
        armor_marker_.scale.y = armor.type == ArmorType::SMALL ? 0.135 : 0.23;
        armor_marker_.pose = armor_msg.pose;
        text_marker_.id++;
        text_marker_.pose.position = armor_msg.pose.position;
        text_marker_.pose.position.y -= 0.1;
        text_marker_.text = armor.classfication_result;
        armors_msg_.armors.emplace_back(armor_msg);
        marker_array_.markers.emplace_back(armor_marker_);
        marker_array_.markers.emplace_back(text_marker_);
      }
      else
      {
        RCLCPP_WARN(this->get_logger(), "PnP failed!");
      }
    }

    // 发布消息
    armors_pub_->publish(armors_msg_);
    all_ba_ =true;
    // 可视化发布
    publishMarkers();
  }
  
}

void ArmorDetectorNode::pose_back_back(auto_aim_interfaces::msg::Detections Detections)
{
std::vector<my_Detection> re;

// 遍历接收到的ROS消息中的每个检测结果
for (const auto& ros_detection : Detections.detections) {
    my_Detection detection;
    // 1. 填充基础信息
    detection.class_id = ros_detection.class_id;    
    // 2. 转换关键点
    detection.Key_Point.key_point.clear();
    for (const auto& ros_kp : ros_detection.key_point.key_point) {
        cv::Point2f kp;
        kp.x = ros_kp.x;
        kp.y = ros_kp.y;
        detection.Key_Point.key_point.push_back(kp);
    }

    re.push_back(detection);
}
  std::shared_ptr<cv::Mat> frame_ptr;
  if(Detections.my_id==1)
  {
  frame_ptr = detector_->pose_inference->my_inputs[Detections.inif_id];
  detector_->pose_inference->flages[Detections.inif_id]=true;
  }
  else if(Detections.my_id==2)
  {
  frame_ptr = detector_->Apose_inference->my_inputs[Detections.inif_id];
    detector_->Apose_inference->flages[Detections.inif_id]=true;
  }
  else if(Detections.my_id==3)
  {
  frame_ptr = detector_->Bpose_inference->my_inputs[Detections.inif_id];
    detector_->Bpose_inference->flages[Detections.inif_id]=true;
  }


  std::vector<Armor> armors_=detector_->pose_detect(re,*frame_ptr);
  //std::cout<<" 接受pose_detect armors_大小="<<armors_.size()<<std::endl;

 if (debug_) {
  // 假设 headers 是 vector，且 Detections.inif_id 是合法索引
    if(Detections.my_id==1)
  {
  binary_img_pub_.publish(
    cv_bridge::CvImage(detector_->pose_inference->headers[Detections.inif_id], "mono8", detector_->binary_img).toImageMsg());

  }
  else if(Detections.my_id==2)
  {
  binary_img_pub_.publish(
    cv_bridge::CvImage(detector_->Apose_inference->headers[Detections.inif_id], "mono8", detector_->binary_img).toImageMsg());

  }
  else if(Detections.my_id==3)
  {
  binary_img_pub_.publish(
    cv_bridge::CvImage(detector_->Bpose_inference->headers[Detections.inif_id], "mono8", detector_->binary_img).toImageMsg());

  }


  // Sort lights and armors data by x coordinate
  std::sort(detector_->debug_lights.data.begin(), detector_->debug_lights.data.end(),
            [](const auto& l1, const auto& l2) { return l1.center_x < l2.center_x; });
  std::sort(detector_->debug_armors.data.begin(), detector_->debug_armors.data.end(),
            [](const auto& a1, const auto& a2) { return a1.center_x < a2.center_x; });

  lights_data_pub_->publish(detector_->debug_lights);
  armors_data_pub_->publish(detector_->debug_armors);

  // 修正 armors_ 变量名
  if (!armors_.empty()) {
    auto all_num_img = detector_->getAllNumbersImage();

        if(Detections.my_id==1)
  {
    number_img_pub_.publish(
      *cv_bridge::CvImage(detector_->pose_inference->headers[Detections.inif_id], "mono8", all_num_img).toImageMsg());

  }
  else if(Detections.my_id==2)
  {
    number_img_pub_.publish(
      *cv_bridge::CvImage(detector_->Apose_inference->headers[Detections.inif_id], "mono8", all_num_img).toImageMsg());

  }
  else if(Detections.my_id==3)
  {
    number_img_pub_.publish(
      *cv_bridge::CvImage(detector_->Bpose_inference->headers[Detections.inif_id], "mono8", all_num_img).toImageMsg());

  }


  }

  // 解引用 frame_ptr
  detector_->drawResults(*frame_ptr);

  // 修正 OpenCV 函数参数
  cv::circle(*frame_ptr, cam_center_, 5, cv::Scalar(255, 0, 0), 2);

  // 确保 latency 变量已定义
  std::stringstream latency_ss;
  auto latency_s = latency_ss.str();
  cv::putText(*frame_ptr, latency_s, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

  if(Detections.my_id==1)
  {
  // 解引用 frame_ptr
  result_img_pub_.publish(
    cv_bridge::CvImage(detector_->pose_inference->headers[Detections.inif_id], "bgr8", *frame_ptr).toImageMsg());

  }
  else if(Detections.my_id==2)
  {
  // 解引用 frame_ptr
  result_img_pub_.publish(
    cv_bridge::CvImage(detector_->Apose_inference->headers[Detections.inif_id], "bgr8", *frame_ptr).toImageMsg());
  }
  else if(Detections.my_id==3)
  {
  // 解引用 frame_ptr
  result_img_pub_.publish(
    cv_bridge::CvImage(detector_->Bpose_inference->headers[Detections.inif_id], "bgr8", *frame_ptr).toImageMsg());

  }


  }
 
 Eigen::Matrix3d temp_imu_to_camera;
for (int i = 0; i < 3; i++) {
  for (int j = 0; j < 3; j++) {
    temp_imu_to_camera(i, j) = Detections.imu_to_camera[i * 3 + j];
  }
}
/*   if(Detections.my_id==1)
  {
  this->pnp(armors_,Detections.header,temp_imu_to_camera);
//  detector_->pose_inference->flages[Detections.inif_id]=true;
  }
  else if(Detections.my_id==2)
  {
  this->pnp(armors_,Detections.header,temp_imu_to_camera);
//  detector_->Apose_inference->flages[Detections.inif_id]=true;
  }
  else if(Detections.my_id==3)
  {
  this->pnp(armors_,Detections.header,temp_imu_to_camera);
//  detector_->Bpose_inference->flages[Detections.inif_id]=true;
  } */
  this->pnp(armors_,Detections.header,temp_imu_to_camera);
}

void ArmorDetectorNode::pnp(std::vector<Armor> armors,std_msgs::msg::Header header,Eigen::Matrix3d imu_to_camera_)
{

  // 获取时间戳的两个部分
auto sec = header.stamp.sec;
auto nanosec = header.stamp.nanosec;

RCLCPP_INFO(
  get_logger(),
  "pnp结算使用时间辍: %d.%09d", 
  sec, 
  nanosec
);

  // 进行pnp解算
  if (pnp_solver_ != nullptr)
  {
    // 同步消息头
    armors_msg_.header = armor_marker_.header = text_marker_.header = header;

    armors_msg_.armors.clear();
    marker_array_.markers.clear();
    armor_marker_.id = 0;
    text_marker_.id = 0;

    auto_aim_interfaces::msg::Armor armor_msg;
    for (auto& armor : armors)
    {
      cv::Mat rvec(3, 1, CV_64F), tvec(3, 1, CV_64F);
      cv::Mat rotation_matrix(3, 3, CV_64F);

      // 进行pnp解算
      bool success = pnp_solver_->solvePnP(armor, rvec, tvec);
      
      // 获取装甲板世界坐标系下绕中心旋转的弧度
      armor.roll = rvecToRPY(rvec, 0) * 180 / M_PI;
      armor.imu2camera = imu_to_camera_;

      // 将旋转向量转为旋转矩阵（这里是相机坐标系到装甲板坐标系的旋转）
      cv::Rodrigues(rvec, rotation_matrix);
      armor.rmat = rotation_matrix.clone();
      armor.tvec = tvec.clone();

      // 对装甲板的旋转进行ba优化，要求当前tracker正在跟随装甲板的id和要处理的装甲板相同
      if (use_ba_  && (ba_number == armor.number || all_ba_ ==true))  {
        // 只优化差不多平行与地面的装甲板
        if (std::abs(armor.roll) < 25) {
          tracked_armors_.push_back(armor);
        }

        // 最初尝试通过多帧装甲板（多帧 BA 优化）提高精度，但发现效果不理想。
        // 多帧可能会引入历史误差的累积，特别是在目标移动快速或者观测误差较大的情况下。
        // 故限制，保证 tracked_armors_ 队列中的装甲板数量最多为 1
        if (tracked_armors_.size() > 1) {
          tracked_armors_.pop_front();
        }

        // 使用BA算法优化pnp求解到的旋转矩阵
        ba_solver_->solveBa(tracked_armors_, rotation_matrix);
      }

      if (success)
      {
        // 将解算结果填入消息体中
        armor_msg.type = ARMOR_TYPE_STR[static_cast<int>(armor.type)];
        armor_msg.number = armor.number;
        armor_msg.pose.position.x = tvec.at<double>(0);
        armor_msg.pose.position.y = tvec.at<double>(1);
        armor_msg.pose.position.z = tvec.at<double>(2);
        // rvec to 3x3 rotation matrix
        // cv::Mat rotation_matrix;
        // cv::Rodrigues(rvec, rotation_matrix);
        // rotation matrix to quaternion
        tf2::Matrix3x3 tf2_rotation_matrix(
          rotation_matrix.at<double>(0, 0), rotation_matrix.at<double>(0, 1),
          rotation_matrix.at<double>(0, 2), rotation_matrix.at<double>(1, 0),
          rotation_matrix.at<double>(1, 1), rotation_matrix.at<double>(1, 2),
          rotation_matrix.at<double>(2, 0), rotation_matrix.at<double>(2, 1),
          rotation_matrix.at<double>(2, 2));
        tf2::Quaternion tf2_q;
        tf2_rotation_matrix.getRotation(tf2_q);
        armor_msg.pose.orientation = tf2::toMsg(tf2_q);

        // 计算装甲板中心到图像中心的距离
        armor_msg.distance_to_image_center = pnp_solver_->calculateDistanceToCenter(armor.center);
        armor_msg.armor_ratio = armor.armor_ratio;
        
        // 装甲板，数字识别文本可视化
        armor_marker_.id++;
        armor_marker_.scale.y = armor.type == ArmorType::SMALL ? 0.135 : 0.23;
        armor_marker_.pose = armor_msg.pose;
        text_marker_.id++;
        text_marker_.pose.position = armor_msg.pose.position;
        text_marker_.pose.position.y -= 0.1;
        text_marker_.text = armor.classfication_result;
        armors_msg_.armors.emplace_back(armor_msg);
        marker_array_.markers.emplace_back(armor_marker_);
        marker_array_.markers.emplace_back(text_marker_);
      }
      else
      {
        RCLCPP_WARN(this->get_logger(), "PnP failed!");
      }
    }

    // 发布消息
    armors_pub_->publish(armors_msg_);
    all_ba_ =true;
    // 可视化发布
    publishMarkers();
  }

  
}
/**
 * @brief 初始化
 */
std::unique_ptr<Detector> ArmorDetectorNode::initDetector()
{
  // 参数初始化
  rcl_interfaces::msg::ParameterDescriptor param_desc;
  param_desc.integer_range.resize(1);
  param_desc.integer_range[0].step = 1;
  param_desc.integer_range[0].from_value = 0;
  param_desc.integer_range[0].to_value = 255;
  int binary_thres = this->declare_parameter("binary_thres_red", 160, param_desc);
  this->declare_parameter("binary_thres_blue", 160, param_desc);

  param_desc.description = "0-RED, 1-BLUE, 2-AUTO";
  param_desc.integer_range[0].from_value = 0;
  param_desc.integer_range[0].to_value = 2;
  auto detect_color = this->declare_parameter("detect_color", BLUE, param_desc);

  Detector::LightParams l_params = { .min_ratio = declare_parameter("light.min_ratio", 0.1),
                                     .max_ratio = declare_parameter("light.max_ratio", 0.8),
                                     .max_angle = declare_parameter("light.max_angle", 40.0),
                                     .min_fill_ratio = declare_parameter("light.min_fill_ratio", 0.8) };

  Detector::ArmorParams a_params = { .min_light_ratio = declare_parameter("armor.min_light_ratio", 0.7),
                                     .min_small_armor_ratio = declare_parameter("armor.min_small_armor_ratio", 0.8),
                                     .max_small_armor_ratio = declare_parameter("armor.max_small_armor_ratio", 3.2),
                                     .min_large_armor_ratio = declare_parameter("armor.min_large_armor_ratio", 3.2),
                                     .max_large_armor_ratio = declare_parameter("armor.max_large_armor_ratio", 5.5),
                                     .max_angle = declare_parameter("armor.max_angle", 35.0) };
  // 构建detector对象
  auto detector = std::make_unique<Detector>(binary_thres, detect_color, l_params, a_params);

  // 构建数字检测器对象
  auto pkg_path = ament_index_cpp::get_package_share_directory("armor_detector");
  auto model_path = pkg_path + "/model/lenet5_threshold_32x32_20240731.onnx";
  auto label_path = pkg_path + "/model/label.txt";
  double threshold = this->declare_parameter("classifier_threshold", 0.7);
  std::vector<std::string> ignore_classes = this->declare_parameter("ignore_classes", std::vector<std::string>{ "0" });
  bool capture = this->declare_parameter("capture", false);
  detector->classifier = std::make_unique<NumberClassifier>(model_path, label_path, threshold, capture, ignore_classes);

    //对pose(四点)模型部署
  auto pose_model_path = pkg_path + "/model/20250512/best.xml"   ;
  const float pose_model_confidence_threshold = 0.65;
  const float pose_model_NMS_threshold = 0.5;
  std::string GPU = "GPU"; 
  std::string CPU = "CPU";
  detector->pose_inference = std::make_unique<Inference>(
  pose_model_path, cv::Size(640, 640), pose_model_confidence_threshold, pose_model_NMS_threshold,GPU,detections_pub_,detector_);

  detector->Apose_inference = std::make_unique<Inference>(
  pose_model_path, cv::Size(640, 640), pose_model_confidence_threshold, pose_model_NMS_threshold,GPU,detections_pub_,detector_);

  detector->Bpose_inference = std::make_unique<Inference>(
  pose_model_path, cv::Size(640, 640), pose_model_confidence_threshold, pose_model_NMS_threshold,GPU,detections_pub_,detector_);


  detector->pose_inference->inference_id=1;
  detector->Apose_inference->inference_id=2;
  detector->Bpose_inference->inference_id=3;


  detector->pose_inference->cpu_cores  = {6,9,10,18,19};
  detector->Apose_inference->cpu_cores = {7,11,12,16,17};
  detector->Bpose_inference->cpu_cores = {8,13,14,15};

  std::cout<<"openvino模型部署成功"<<std::endl;

  // 构建ba优化器对象
  bool use_pca = this->declare_parameter("use_pca", true);
  if (use_pca) {
    detector->corner_corrector = std::make_unique<LightCornerCorrector>();
  }

  return detector;
}

void ArmorDetectorNode::Pose_Inference(const sensor_msgs::msg::Image::ConstSharedPtr img_msg)
{

  auto img = cv_bridge::toCvShare(img_msg, "bgr8")->image;//std::cout << "img" << "成功" << std::endl;

    // 更新与图像预处理识别有关参数
  auto detect_color = get_parameter("detect_color").as_int();
  if (detect_color != AUTO)
  {
    detector_->detect_color = get_parameter("detect_color").as_int();
  }
  if(detector_->detect_color == RED) detector_->binary_thres = get_parameter("binary_thres_red").as_int();
  else detector_->binary_thres = get_parameter("binary_thres_blue").as_int();
  detector_->classifier->capture = get_parameter("capture") .as_bool();
  detector_->classifier->threshold = get_parameter("classifier_threshold").as_double();

//  std::cout << "img" << "成功" << std::endl;

/*   std_msgs::msg::Header header = img_msg->header;
  std::string timestamp = std::to_string(header.stamp.sec) + "_" + std::to_string(header.stamp.nanosec);
  RCLCPP_INFO(
      get_logger(),
      "进入时间辍: %s",  // 添加 %s 占位符
      timestamp.c_str()  // 转换 std::string 为 const char*
  );
 */


  if(detector_->pose_inference->Pose_Run_async_Inference(img,img_msg->header,imu_to_camera_))
  {
    return;
  }
  else if(detector_->Apose_inference->Pose_Run_async_Inference(img,img_msg->header,imu_to_camera_))
  {
    return;
  }
/*   else if(detector_->Bpose_inference->Pose_Run_async_Inference(img,img_msg->header,imu_to_camera_))
  {
    return;
  } 
 */


}

/**
 * @brief 检测装甲板
 * @param img_msg 图像数据
 * @return 所有检测到的装甲板构成的容器
 */
std::vector<Armor> ArmorDetectorNode::detectArmors(const sensor_msgs::msg::Image::ConstSharedPtr& img_msg)
{
  // 将ros图像数据转成opencv矩阵格式
  auto img = cv_bridge::toCvShare(img_msg, "rgb8")->image;
  std::cout << "img" << "成功" << std::endl;

  // 更新与图像预处理识别有关参数
  auto detect_color = get_parameter("detect_color").as_int();
  if (detect_color != AUTO)
  {
    detector_->detect_color = get_parameter("detect_color").as_int();
  }
  if(detector_->detect_color == RED) detector_->binary_thres = get_parameter("binary_thres_red").as_int();
  else detector_->binary_thres = get_parameter("binary_thres_blue").as_int();
  detector_->classifier->capture = get_parameter("capture") .as_bool();
  detector_->classifier->threshold = get_parameter("classifier_threshold").as_double();

  // 识别装甲板
  auto armors = detector_->detect(img);//在这里
 // auto armors = detector_->pose_detect(img); 

  // 计算图像预处理加识别后的整体延迟
  auto final_time = this->now();
  auto latency = (final_time - img_msg->header.stamp).seconds() * 1000;
  RCLCPP_DEBUG_STREAM(this->get_logger(), "Latency: " << latency << "ms");

  // 发布调试数据
  if (debug_)
  {
    binary_img_pub_.publish(cv_bridge::CvImage(img_msg->header, "mono8", detector_->binary_img).toImageMsg());

    // 根据x坐标（从左到右）排序调试的灯条和装甲板
    std::sort(detector_->debug_lights.data.begin(), detector_->debug_lights.data.end(),
              [](const auto& l1, const auto& l2) { return l1.center_x < l2.center_x; });
    std::sort(detector_->debug_armors.data.begin(), detector_->debug_armors.data.end(),
              [](const auto& a1, const auto& a2) { return a1.center_x < a2.center_x; });
    lights_data_pub_->publish(detector_->debug_lights);
    armors_data_pub_->publish(detector_->debug_armors);

    // 数字二值化图像发布
    if (!armors.empty())
    {
      auto all_num_img = detector_->getAllNumbersImage();
      number_img_pub_.publish(*cv_bridge::CvImage(img_msg->header, "mono8", all_num_img).toImageMsg());
    }

    // 发布识别结果图像
    detector_->drawResults(img);
    // 绘制中心
    cv::circle(img, cam_center_, 5, cv::Scalar(255, 0, 0), 2);
    // 绘制延迟
    std::stringstream latency_ss;
    latency_ss << "Latency: " << std::fixed << std::setprecision(2) << latency << "ms";
    auto latency_s = latency_ss.str();
    cv::putText(img, latency_s, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
    result_img_pub_.publish(cv_bridge::CvImage(img_msg->header, "rgb8", img).toImageMsg());
  }

  return armors;
}

/**
 * @brief 将装甲板坐标系到相机坐标系的旋转向量转为装甲板坐标系到世界坐标系的旋转矩阵后，获取装甲板R/P/Y
 * @param rvec 装甲板坐标系到相机坐标系的旋转向量
 * @param axis 要获取的旋转量下标：0 roll，1 pitch，2 yaw
 * @return 对应旋转量
 */
double ArmorDetectorNode::rvecToRPY(const cv::Mat &rvec, int axis) const noexcept {
  // 将旋转向量转为3x3的旋转矩阵
  cv::Mat R;
  cv::Rodrigues(rvec, R);
  Eigen::Matrix3d eigen_R = utils::cvToEigen(R);
  // 将装甲板坐标系到相机坐标系的旋转矩阵转为装甲板坐标系到世界坐标系的旋转矩阵
  eigen_R = imu_to_camera_ * eigen_R;

  // 将旋转矩阵转为三个旋转量
  Eigen::Quaterniond q(eigen_R);
  tf2::Quaternion tf_q(q.x(), q.y(), q.z(), q.w());
  std::array<double, 3> rpy;
  tf2::Matrix3x3(tf_q).getRPY(rpy[0], rpy[1], rpy[2]);
  return rpy[axis];
}

/**
 * @brief 创建调试话题发布者
 */
void ArmorDetectorNode::createDebugPublishers()
{
  lights_data_pub_ = this->create_publisher<auto_aim_interfaces::msg::DebugLights>("/detector/debug_lights", 10);
  armors_data_pub_ = this->create_publisher<auto_aim_interfaces::msg::DebugArmors>("/detector/debug_armors", 10);

  binary_img_pub_ = image_transport::create_publisher(this, "/detector/binary_img");
  number_img_pub_ = image_transport::create_publisher(this, "/detector/number_img");
  result_img_pub_ = image_transport::create_publisher(this, "/detector/result_img");

  pose_img_pub_ = image_transport::create_publisher(this, "/detector/pose_img");
}

/**
 * @brief 销毁调试话题发布者
 */
void ArmorDetectorNode::destroyDebugPublishers()
{
  lights_data_pub_.reset();
  armors_data_pub_.reset();

  binary_img_pub_.shutdown();
  number_img_pub_.shutdown();
  result_img_pub_.shutdown();

  //ran's CODE7.12
  //fixxed_img_pub_.shutdown();


}

/**
 * @brief 发布可视化数据
 */
void ArmorDetectorNode::publishMarkers()
{
  using Marker = visualization_msgs::msg::Marker;
  armor_marker_.action = armors_msg_.armors.empty() ? Marker::DELETE : Marker::ADD;
  marker_array_.markers.emplace_back(armor_marker_);
  marker_pub_->publish(marker_array_);
}

/**
 * @brief 更新检测相关参数，用于动态调参
 */
void ArmorDetectorNode::updateDetectorParams()
{

  Detector::ArmorParams a_params = { .min_light_ratio = get_parameter("armor.min_light_ratio").as_double(),
                                     .min_small_armor_ratio = get_parameter("armor.min_small_armor_ratio").as_double(),
                                     .max_small_armor_ratio = get_parameter("armor.max_small_armor_ratio").as_double(),
                                     .min_large_armor_ratio = get_parameter("armor.min_large_armor_ratio").as_double(),
                                     .max_large_armor_ratio = get_parameter("armor.max_large_armor_ratio").as_double(),
                                     .max_angle = get_parameter("armor.max_angle").as_double() };

  detector_->a = a_params;
}

void ArmorDetectorNode::targetCallback(const auto_aim_interfaces::msg::Target::SharedPtr msg){
  ba_number = msg->id;
  all_ba_ = false;
}

}  // namespace rm_auto_aim

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rm_auto_aim::ArmorDetectorNode)
