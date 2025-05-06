// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/GimbalFdb.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/gimbal_fdb__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_GimbalFdb_aiming_mode
{
public:
  explicit Init_GimbalFdb_aiming_mode(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::GimbalFdb aiming_mode(::auto_aim_interfaces::msg::GimbalFdb::_aiming_mode_type arg)
  {
    msg_.aiming_mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_priority_number
{
public:
  explicit Init_GimbalFdb_priority_number(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  Init_GimbalFdb_aiming_mode priority_number(::auto_aim_interfaces::msg::GimbalFdb::_priority_number_type arg)
  {
    msg_.priority_number = std::move(arg);
    return Init_GimbalFdb_aiming_mode(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_camp
{
public:
  explicit Init_GimbalFdb_camp(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  Init_GimbalFdb_priority_number camp(::auto_aim_interfaces::msg::GimbalFdb::_camp_type arg)
  {
    msg_.camp = std::move(arg);
    return Init_GimbalFdb_priority_number(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_imu_pit_spd
{
public:
  explicit Init_GimbalFdb_imu_pit_spd(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  Init_GimbalFdb_camp imu_pit_spd(::auto_aim_interfaces::msg::GimbalFdb::_imu_pit_spd_type arg)
  {
    msg_.imu_pit_spd = std::move(arg);
    return Init_GimbalFdb_camp(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_imu_pit
{
public:
  explicit Init_GimbalFdb_imu_pit(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  Init_GimbalFdb_imu_pit_spd imu_pit(::auto_aim_interfaces::msg::GimbalFdb::_imu_pit_type arg)
  {
    msg_.imu_pit = std::move(arg);
    return Init_GimbalFdb_imu_pit_spd(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_imu_yaw_spd
{
public:
  explicit Init_GimbalFdb_imu_yaw_spd(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  Init_GimbalFdb_imu_pit imu_yaw_spd(::auto_aim_interfaces::msg::GimbalFdb::_imu_yaw_spd_type arg)
  {
    msg_.imu_yaw_spd = std::move(arg);
    return Init_GimbalFdb_imu_pit(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_imu_yaw
{
public:
  explicit Init_GimbalFdb_imu_yaw(::auto_aim_interfaces::msg::GimbalFdb & msg)
  : msg_(msg)
  {}
  Init_GimbalFdb_imu_yaw_spd imu_yaw(::auto_aim_interfaces::msg::GimbalFdb::_imu_yaw_type arg)
  {
    msg_.imu_yaw = std::move(arg);
    return Init_GimbalFdb_imu_yaw_spd(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

class Init_GimbalFdb_header
{
public:
  Init_GimbalFdb_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GimbalFdb_imu_yaw header(::auto_aim_interfaces::msg::GimbalFdb::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_GimbalFdb_imu_yaw(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalFdb msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::GimbalFdb>()
{
  return auto_aim_interfaces::msg::builder::Init_GimbalFdb_header();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__BUILDER_HPP_
