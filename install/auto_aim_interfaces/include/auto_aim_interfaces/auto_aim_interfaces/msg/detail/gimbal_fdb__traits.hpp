// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from auto_aim_interfaces:msg/GimbalFdb.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__TRAITS_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "auto_aim_interfaces/msg/detail/gimbal_fdb__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace auto_aim_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const GimbalFdb & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: imu_yaw
  {
    out << "imu_yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_yaw, out);
    out << ", ";
  }

  // member: imu_yaw_spd
  {
    out << "imu_yaw_spd: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_yaw_spd, out);
    out << ", ";
  }

  // member: imu_pit
  {
    out << "imu_pit: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_pit, out);
    out << ", ";
  }

  // member: imu_pit_spd
  {
    out << "imu_pit_spd: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_pit_spd, out);
    out << ", ";
  }

  // member: camp
  {
    out << "camp: ";
    rosidl_generator_traits::value_to_yaml(msg.camp, out);
    out << ", ";
  }

  // member: priority_number
  {
    out << "priority_number: ";
    rosidl_generator_traits::value_to_yaml(msg.priority_number, out);
    out << ", ";
  }

  // member: aiming_mode
  {
    out << "aiming_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.aiming_mode, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GimbalFdb & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: imu_yaw
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "imu_yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_yaw, out);
    out << "\n";
  }

  // member: imu_yaw_spd
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "imu_yaw_spd: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_yaw_spd, out);
    out << "\n";
  }

  // member: imu_pit
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "imu_pit: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_pit, out);
    out << "\n";
  }

  // member: imu_pit_spd
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "imu_pit_spd: ";
    rosidl_generator_traits::value_to_yaml(msg.imu_pit_spd, out);
    out << "\n";
  }

  // member: camp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "camp: ";
    rosidl_generator_traits::value_to_yaml(msg.camp, out);
    out << "\n";
  }

  // member: priority_number
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "priority_number: ";
    rosidl_generator_traits::value_to_yaml(msg.priority_number, out);
    out << "\n";
  }

  // member: aiming_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "aiming_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.aiming_mode, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GimbalFdb & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace auto_aim_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use auto_aim_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const auto_aim_interfaces::msg::GimbalFdb & msg,
  std::ostream & out, size_t indentation = 0)
{
  auto_aim_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use auto_aim_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const auto_aim_interfaces::msg::GimbalFdb & msg)
{
  return auto_aim_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<auto_aim_interfaces::msg::GimbalFdb>()
{
  return "auto_aim_interfaces::msg::GimbalFdb";
}

template<>
inline const char * name<auto_aim_interfaces::msg::GimbalFdb>()
{
  return "auto_aim_interfaces/msg/GimbalFdb";
}

template<>
struct has_fixed_size<auto_aim_interfaces::msg::GimbalFdb>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<auto_aim_interfaces::msg::GimbalFdb>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<auto_aim_interfaces::msg::GimbalFdb>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__TRAITS_HPP_
