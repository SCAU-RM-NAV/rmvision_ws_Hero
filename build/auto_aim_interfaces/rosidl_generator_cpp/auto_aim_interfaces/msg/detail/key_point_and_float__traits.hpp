// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from auto_aim_interfaces:msg/KeyPointAndFloat.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__TRAITS_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "auto_aim_interfaces/msg/detail/key_point_and_float__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'key_point'
#include "auto_aim_interfaces/msg/detail/key_point2_f__traits.hpp"

namespace auto_aim_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const KeyPointAndFloat & msg,
  std::ostream & out)
{
  out << "{";
  // member: key_point
  {
    if (msg.key_point.size() == 0) {
      out << "key_point: []";
    } else {
      out << "key_point: [";
      size_t pending_items = msg.key_point.size();
      for (auto item : msg.key_point) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const KeyPointAndFloat & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: key_point
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.key_point.size() == 0) {
      out << "key_point: []\n";
    } else {
      out << "key_point:\n";
      for (auto item : msg.key_point) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const KeyPointAndFloat & msg, bool use_flow_style = false)
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
  const auto_aim_interfaces::msg::KeyPointAndFloat & msg,
  std::ostream & out, size_t indentation = 0)
{
  auto_aim_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use auto_aim_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const auto_aim_interfaces::msg::KeyPointAndFloat & msg)
{
  return auto_aim_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<auto_aim_interfaces::msg::KeyPointAndFloat>()
{
  return "auto_aim_interfaces::msg::KeyPointAndFloat";
}

template<>
inline const char * name<auto_aim_interfaces::msg::KeyPointAndFloat>()
{
  return "auto_aim_interfaces/msg/KeyPointAndFloat";
}

template<>
struct has_fixed_size<auto_aim_interfaces::msg::KeyPointAndFloat>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<auto_aim_interfaces::msg::KeyPointAndFloat>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<auto_aim_interfaces::msg::KeyPointAndFloat>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__TRAITS_HPP_
