// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/Detections.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'detections'
#include "auto_aim_interfaces/msg/detail/detection__struct.h"

/// Struct defined in msg/Detections in the package auto_aim_interfaces.
typedef struct auto_aim_interfaces__msg__Detections
{
  std_msgs__msg__Header header;
  auto_aim_interfaces__msg__Detection__Sequence detections;
  int16_t inif_id;
  int16_t my_id;
  /// 按行优先展开3x3矩阵
  double imu_to_camera[9];
} auto_aim_interfaces__msg__Detections;

// Struct for a sequence of auto_aim_interfaces__msg__Detections.
typedef struct auto_aim_interfaces__msg__Detections__Sequence
{
  auto_aim_interfaces__msg__Detections * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__Detections__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTIONS__STRUCT_H_
