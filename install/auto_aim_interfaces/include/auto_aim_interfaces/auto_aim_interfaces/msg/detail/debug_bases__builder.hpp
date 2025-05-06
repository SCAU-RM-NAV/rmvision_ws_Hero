// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/DebugBases.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/debug_bases__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_DebugBases_data
{
public:
  Init_DebugBases_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::auto_aim_interfaces::msg::DebugBases data(::auto_aim_interfaces::msg::DebugBases::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::DebugBases msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::DebugBases>()
{
  return auto_aim_interfaces::msg::builder::Init_DebugBases_data();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__BUILDER_HPP_
