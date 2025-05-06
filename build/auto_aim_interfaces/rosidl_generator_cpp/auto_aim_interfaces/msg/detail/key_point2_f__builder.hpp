// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/KeyPoint2F.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT2_F__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT2_F__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/key_point2_f__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_KeyPoint2F_y
{
public:
  explicit Init_KeyPoint2F_y(::auto_aim_interfaces::msg::KeyPoint2F & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::KeyPoint2F y(::auto_aim_interfaces::msg::KeyPoint2F::_y_type arg)
  {
    msg_.y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::KeyPoint2F msg_;
};

class Init_KeyPoint2F_x
{
public:
  Init_KeyPoint2F_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_KeyPoint2F_y x(::auto_aim_interfaces::msg::KeyPoint2F::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_KeyPoint2F_y(msg_);
  }

private:
  ::auto_aim_interfaces::msg::KeyPoint2F msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::KeyPoint2F>()
{
  return auto_aim_interfaces::msg::builder::Init_KeyPoint2F_x();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT2_F__BUILDER_HPP_
