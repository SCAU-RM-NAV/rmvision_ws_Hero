// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from auto_aim_interfaces:msg/Bases.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__BASES__BUILDER_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__BASES__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "auto_aim_interfaces/msg/detail/bases__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace auto_aim_interfaces
{

namespace msg
{

namespace builder
{

class Init_Bases_bases
{
public:
  explicit Init_Bases_bases(::auto_aim_interfaces::msg::Bases & msg)
  : msg_(msg)
  {}
  ::auto_aim_interfaces::msg::Bases bases(::auto_aim_interfaces::msg::Bases::_bases_type arg)
  {
    msg_.bases = std::move(arg);
    return std::move(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Bases msg_;
};

class Init_Bases_header
{
public:
  Init_Bases_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Bases_bases header(::auto_aim_interfaces::msg::Bases::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Bases_bases(msg_);
  }

private:
  ::auto_aim_interfaces::msg::Bases msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::auto_aim_interfaces::msg::Bases>()
{
  return auto_aim_interfaces::msg::builder::Init_Bases_header();
}

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__BASES__BUILDER_HPP_
