// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from auto_aim_interfaces:msg/GimbalCtrl.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__TRAITS_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "auto_aim_interfaces/msg/detail/gimbal_ctrl__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace auto_aim_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const GimbalCtrl & msg,
  std::ostream & out)
{
  out << "{";
  // member: yaw
  {
    out << "yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw, out);
    out << ", ";
  }

  // member: pit
  {
    out << "pit: ";
    rosidl_generator_traits::value_to_yaml(msg.pit, out);
    out << ", ";
  }

  // member: shoot_yaw_tole
  {
    out << "shoot_yaw_tole: ";
    rosidl_generator_traits::value_to_yaml(msg.shoot_yaw_tole, out);
    out << ", ";
  }

  // member: fire_flag
  {
    out << "fire_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.fire_flag, out);
    out << ", ";
  }

  // member: pos
  {
    out << "pos: ";
    rosidl_generator_traits::value_to_yaml(msg.pos, out);
    out << ", ";
  }

  // member: empty
  {
    out << "empty: ";
    rosidl_generator_traits::value_to_yaml(msg.empty, out);
    out << ", ";
  }

  // member: cnt
  {
    out << "cnt: ";
    rosidl_generator_traits::value_to_yaml(msg.cnt, out);
    out << ", ";
  }

  // member: ist_flag
  {
    out << "ist_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.ist_flag, out);
    out << ", ";
  }

  // member: aim_flag
  {
    out << "aim_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.aim_flag, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: base_flag
  {
    out << "base_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.base_flag, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const GimbalCtrl & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: yaw
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "yaw: ";
    rosidl_generator_traits::value_to_yaml(msg.yaw, out);
    out << "\n";
  }

  // member: pit
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pit: ";
    rosidl_generator_traits::value_to_yaml(msg.pit, out);
    out << "\n";
  }

  // member: shoot_yaw_tole
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "shoot_yaw_tole: ";
    rosidl_generator_traits::value_to_yaml(msg.shoot_yaw_tole, out);
    out << "\n";
  }

  // member: fire_flag
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "fire_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.fire_flag, out);
    out << "\n";
  }

  // member: pos
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pos: ";
    rosidl_generator_traits::value_to_yaml(msg.pos, out);
    out << "\n";
  }

  // member: empty
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "empty: ";
    rosidl_generator_traits::value_to_yaml(msg.empty, out);
    out << "\n";
  }

  // member: cnt
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "cnt: ";
    rosidl_generator_traits::value_to_yaml(msg.cnt, out);
    out << "\n";
  }

  // member: ist_flag
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ist_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.ist_flag, out);
    out << "\n";
  }

  // member: aim_flag
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "aim_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.aim_flag, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: base_flag
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "base_flag: ";
    rosidl_generator_traits::value_to_yaml(msg.base_flag, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const GimbalCtrl & msg, bool use_flow_style = false)
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
  const auto_aim_interfaces::msg::GimbalCtrl & msg,
  std::ostream & out, size_t indentation = 0)
{
  auto_aim_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use auto_aim_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const auto_aim_interfaces::msg::GimbalCtrl & msg)
{
  return auto_aim_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<auto_aim_interfaces::msg::GimbalCtrl>()
{
  return "auto_aim_interfaces::msg::GimbalCtrl";
}

template<>
inline const char * name<auto_aim_interfaces::msg::GimbalCtrl>()
{
  return "auto_aim_interfaces/msg/GimbalCtrl";
}

template<>
struct has_fixed_size<auto_aim_interfaces::msg::GimbalCtrl>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<auto_aim_interfaces::msg::GimbalCtrl>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<auto_aim_interfaces::msg::GimbalCtrl>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__TRAITS_HPP_
