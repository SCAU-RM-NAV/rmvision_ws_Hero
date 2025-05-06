// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/Detections.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/detections__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_Detections_imu_to_camera
{
public:
  explicit Init_Detections_imu_to_camera(::auto_aim_interfaces::msg::Detections & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::Detections imu_to_camera(::auto_aim_interfaces::msg::Detections::_imu_to_camera_type arg)
  {
    msg_.imu_to_camera = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detections msg_;
};

class Init_Detections_my_id
{
public:
  explicit Init_Detections_my_id(::auto_aim_interfaces::msg::Detections & msg)
  : msg_(msg)
  {}
  Init_Detections_imu_to_camera my_id(::auto_aim_interfaces::msg::Detections::_my_id_type arg)
  {
    msg_.my_id = std::move(arg);
    return Init_Detections_imu_to_camera(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detections msg_;
};

class Init_Detections_inif_id
{
public:
  explicit Init_Detections_inif_id(::auto_aim_interfaces::msg::Detections & msg)
  : msg_(msg)
  {}
  Init_Detections_my_id inif_id(::auto_aim_interfaces::msg::Detections::_inif_id_type arg)
  {
    msg_.inif_id = std::move(arg);
    return Init_Detections_my_id(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detections msg_;
};

class Init_Detections_detections
{
public:
  explicit Init_Detections_detections(::auto_aim_interfaces::msg::Detections & msg)
  : msg_(msg)
  {}
  Init_Detections_inif_id detections(::auto_aim_interfaces::msg::Detections::_detections_type arg)
  {
    msg_.detections = std::move(arg);
    return Init_Detections_inif_id(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detections msg_;
};

class Init_Detections_header
{
public:
  Init_Detections_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Detections_detections header(::auto_aim_interfaces::msg::Detections::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Detections_detections(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detections msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::Detections>()
{
  return auto_aim_interfaces::msg::builder::Init_Detections_header();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__BUILDER_HPP_
