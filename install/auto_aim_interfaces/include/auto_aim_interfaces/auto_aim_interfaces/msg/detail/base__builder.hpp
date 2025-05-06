// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/Base.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__BASE__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__BASE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/base__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_Base_center_x
{
public:
  Init_Base_center_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::auto_aim_interfaces::msg::Base center_x(::auto_aim_interfaces::msg::Base::_center_x_type arg)
  {
    msg_.center_x = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Base msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::Base>()
{
  return auto_aim_interfaces::msg::builder::Init_Base_center_x();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__BASE__BUILDER_HPP_
