// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from auto_aim_interfaces:msg/Detections.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__TRAITS_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "auto_aim_interfaces/msg/detail/detections__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'detections'
#include "auto_aim_interfaces/msg/detail/detection__traits.hpp"

namespace auto_aim_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Detections & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: detections
  {
    if (msg.detections.size() == 0) {
      out << "detections: []";
    } else {
      out << "detections: [";
      size_t pending_items = msg.detections.size();
      for (auto item : msg.detections) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: inif_id
  {
    out << "inif_id: ";
    rosidl_generator_traits::value_to_yaml(msg.inif_id, out);
    out << ", ";
  }

  // member: my_id
  {
    out << "my_id: ";
    rosidl_generator_traits::value_to_yaml(msg.my_id, out);
    out << ", ";
  }

  // member: imu_to_camera
  {
    if (msg.imu_to_camera.size() == 0) {
      out << "imu_to_camera: []";
    } else {
      out << "imu_to_camera: [";
      size_t pending_items = msg.imu_to_camera.size();
      for (auto item : msg.imu_to_camera) {
        rosidl_generator_traits::value_to_yaml(item, out);
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
  const Detections & msg,
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

  // member: detections
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.detections.size() == 0) {
      out << "detections: []\n";
    } else {
      out << "detections:\n";
      for (auto item : msg.detections) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: inif_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "inif_id: ";
    rosidl_generator_traits::value_to_yaml(msg.inif_id, out);
    out << "\n";
  }

  // member: my_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "my_id: ";
    rosidl_generator_traits::value_to_yaml(msg.my_id, out);
    out << "\n";
  }

  // member: imu_to_camera
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.imu_to_camera.size() == 0) {
      out << "imu_to_camera: []\n";
    } else {
      out << "imu_to_camera:\n";
      for (auto item : msg.imu_to_camera) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Detections & msg, bool use_flow_style = false)
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
  const auto_aim_interfaces::msg::Detections & msg,
  std::ostream & out, size_t indentation = 0)
{
  auto_aim_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use auto_aim_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const auto_aim_interfaces::msg::Detections & msg)
{
  return auto_aim_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<auto_aim_interfaces::msg::Detections>()
{
  return "auto_aim_interfaces::msg::Detections";
}

template<>
inline const char * name<auto_aim_interfaces::msg::Detections>()
{
  return "auto_aim_interfaces/msg/Detections";
}

template<>
struct has_fixed_size<auto_aim_interfaces::msg::Detections>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<auto_aim_interfaces::msg::Detections>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<auto_aim_interfaces::msg::Detections>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__TRAITS_HPP_
