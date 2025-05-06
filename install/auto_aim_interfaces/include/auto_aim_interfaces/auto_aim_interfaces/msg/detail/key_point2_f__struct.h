// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/KeyPoint2F.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT2_F__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT2_F__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/KeyPoint2F in the package auto_aim_interfaces.
typedef struct auto_aim_interfaces__msg__KeyPoint2F
{
  float x;
  float y;
} auto_aim_interfaces__msg__KeyPoint2F;

// Struct for a sequence of auto_aim_interfaces__msg__KeyPoint2F.
typedef struct auto_aim_interfaces__msg__KeyPoint2F__Sequence
{
  auto_aim_interfaces__msg__KeyPoint2F * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__KeyPoint2F__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__KEY_POINT2_F__STRUCT_H_
