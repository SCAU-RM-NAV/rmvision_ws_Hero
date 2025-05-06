// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/KeyPointAndFloat.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'key_point'
#include "auto_aim_interfaces/msg/detail/key_point2_f__struct.h"

/// Struct defined in msg/KeyPointAndFloat in the package auto_aim_interfaces.
typedef struct auto_aim_interfaces__msg__KeyPointAndFloat
{
  /// 数组类型用 []
  auto_aim_interfaces__msg__KeyPoint2F__Sequence key_point;
} auto_aim_interfaces__msg__KeyPointAndFloat;

// Struct for a sequence of auto_aim_interfaces__msg__KeyPointAndFloat.
typedef struct auto_aim_interfaces__msg__KeyPointAndFloat__Sequence
{
  auto_aim_interfaces__msg__KeyPointAndFloat * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__KeyPointAndFloat__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT_AND_FLOAT__STRUCT_H_
