// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/GimbalCtrl.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/gimbal_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_GimbalCtrl_base_flag
{
public:
  explicit Init_GimbalCtrl_base_flag(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::GimbalCtrl base_flag(::auto_aim_interfaces::msg::GimbalCtrl::_base_flag_type arg)
  {
    msg_.base_flag = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_x
{
public:
  explicit Init_GimbalCtrl_x(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_base_flag x(::auto_aim_interfaces::msg::GimbalCtrl::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_GimbalCtrl_base_flag(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_aim_flag
{
public:
  explicit Init_GimbalCtrl_aim_flag(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_x aim_flag(::auto_aim_interfaces::msg::GimbalCtrl::_aim_flag_type arg)
  {
    msg_.aim_flag = std::move(arg);
    return Init_GimbalCtrl_x(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_ist_flag
{
public:
  explicit Init_GimbalCtrl_ist_flag(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_aim_flag ist_flag(::auto_aim_interfaces::msg::GimbalCtrl::_ist_flag_type arg)
  {
    msg_.ist_flag = std::move(arg);
    return Init_GimbalCtrl_aim_flag(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_cnt
{
public:
  explicit Init_GimbalCtrl_cnt(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_ist_flag cnt(::auto_aim_interfaces::msg::GimbalCtrl::_cnt_type arg)
  {
    msg_.cnt = std::move(arg);
    return Init_GimbalCtrl_ist_flag(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_empty
{
public:
  explicit Init_GimbalCtrl_empty(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_cnt empty(::auto_aim_interfaces::msg::GimbalCtrl::_empty_type arg)
  {
    msg_.empty = std::move(arg);
    return Init_GimbalCtrl_cnt(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_pos
{
public:
  explicit Init_GimbalCtrl_pos(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_empty pos(::auto_aim_interfaces::msg::GimbalCtrl::_pos_type arg)
  {
    msg_.pos = std::move(arg);
    return Init_GimbalCtrl_empty(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_fire_flag
{
public:
  explicit Init_GimbalCtrl_fire_flag(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_pos fire_flag(::auto_aim_interfaces::msg::GimbalCtrl::_fire_flag_type arg)
  {
    msg_.fire_flag = std::move(arg);
    return Init_GimbalCtrl_pos(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_shoot_yaw_tole
{
public:
  explicit Init_GimbalCtrl_shoot_yaw_tole(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_fire_flag shoot_yaw_tole(::auto_aim_interfaces::msg::GimbalCtrl::_shoot_yaw_tole_type arg)
  {
    msg_.shoot_yaw_tole = std::move(arg);
    return Init_GimbalCtrl_fire_flag(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_pit
{
public:
  explicit Init_GimbalCtrl_pit(::auto_aim_interfaces::msg::GimbalCtrl & msg)
  : msg_(msg)
  {}
  Init_GimbalCtrl_shoot_yaw_tole pit(::auto_aim_interfaces::msg::GimbalCtrl::_pit_type arg)
  {
    msg_.pit = std::move(arg);
    return Init_GimbalCtrl_shoot_yaw_tole(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

class Init_GimbalCtrl_yaw
{
public:
  Init_GimbalCtrl_yaw()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_GimbalCtrl_pit yaw(::auto_aim_interfaces::msg::GimbalCtrl::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_GimbalCtrl_pit(msg_);
  }

private:
  ::auto_aim_interfaces::msg::GimbalCtrl msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::GimbalCtrl>()
{
  return auto_aim_interfaces::msg::builder::Init_GimbalCtrl_yaw();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__BUILDER_HPP_
