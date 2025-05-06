// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/Fdb.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__FDB__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__FDB__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/fdb__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_Fdb_pitch
{
public:
  explicit Init_Fdb_pitch(::auto_aim_interfaces::msg::Fdb & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::Fdb pitch(::auto_aim_interfaces::msg::Fdb::_pitch_type arg)
  {
    msg_.pitch = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Fdb msg_;
};

class Init_Fdb_yaw
{
public:
  Init_Fdb_yaw()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Fdb_pitch yaw(::auto_aim_interfaces::msg::Fdb::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_Fdb_pitch(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Fdb msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::Fdb>()
{
  return auto_aim_interfaces::msg::builder::Init_Fdb_yaw();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__FDB__BUILDER_HPP_
