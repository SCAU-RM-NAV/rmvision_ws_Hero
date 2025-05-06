// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/KeyPointAndFloat.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/key_point_and_float__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_KeyPointAndFloat_key_point
{
public:
  Init_KeyPointAndFloat_key_point()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::auto_aim_interfaces::msg::KeyPointAndFloat key_point(::auto_aim_interfaces::msg::KeyPointAndFloat::_key_point_type arg)
  {
    msg_.key_point = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::KeyPointAndFloat msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::KeyPointAndFloat>()
{
  return auto_aim_interfaces::msg::builder::Init_KeyPointAndFloat_key_point();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__BUILDER_HPP_
