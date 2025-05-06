// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/NaviFdb.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__NAVI_FDB__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__NAVI_FDB__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/navi_fdb__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_NaviFdb_x
{
public:
  explicit Init_NaviFdb_x(::auto_aim_interfaces::msg::NaviFdb & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::NaviFdb x(::auto_aim_interfaces::msg::NaviFdb::_x_type arg)
  {
    msg_.x = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::NaviFdb msg_;
};

class Init_NaviFdb_z
{
public:
  Init_NaviFdb_z()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_NaviFdb_x z(::auto_aim_interfaces::msg::NaviFdb::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_NaviFdb_x(msg_);
  }

private:
  ::auto_aim_interfaces::msg::NaviFdb msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::NaviFdb>()
{
  return auto_aim_interfaces::msg::builder::Init_NaviFdb_z();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__NAVI_FDB__BUILDER_HPP_
