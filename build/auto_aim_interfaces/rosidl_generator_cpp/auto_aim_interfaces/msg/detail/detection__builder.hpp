// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/Detection.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTION__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/detection__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_Detection_key_point
{
public:
  explicit Init_Detection_key_point(::auto_aim_interfaces::msg::Detection & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::Detection key_point(::auto_aim_interfaces::msg::Detection::_key_point_type arg)
  {
    msg_.key_point = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detection msg_;
};

class Init_Detection_class_id
{
public:
  Init_Detection_class_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Detection_key_point class_id(::auto_aim_interfaces::msg::Detection::_class_id_type arg)
  {
    msg_.class_id = std::move(arg);
    return Init_Detection_key_point(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Detection msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::Detection>()
{
  return auto_aim_interfaces::msg::builder::Init_Detection_class_id();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTION__BUILDER_HPP_
