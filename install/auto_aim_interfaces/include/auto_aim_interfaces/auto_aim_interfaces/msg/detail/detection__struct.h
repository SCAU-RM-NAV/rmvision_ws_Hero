// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/Detection.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTION__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTION__STRUCT_H_

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
#include "auto_aim_interfaces/msg/detail/key_point_and_float__struct.h"

/// Struct defined in msg/Detection in the package auto_aim_interfaces.
typedef struct auto_aim_interfaces__msg__Detection
{
  int16_t class_id;
  auto_aim_interfaces__msg__KeyPointAndFloat key_point;
} auto_aim_interfaces__msg__Detection;

// Struct for a sequence of auto_aim_interfaces__msg__Detection.
typedef struct auto_aim_interfaces__msg__Detection__Sequence
{
  auto_aim_interfaces__msg__Detection * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__Detection__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DETECTION__STRUCT_H_
