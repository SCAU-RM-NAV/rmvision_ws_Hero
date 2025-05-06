// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/Bases.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__BASES__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__BASES__STRUCT_H_

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
// Member 'bases'
#include "auto_aim_interfaces/msg/detail/base__struct.h"

/// Struct defined in msg/Bases in the package auto_aim_interfaces.
typedef struct auto_aim_interfaces__msg__Bases
{
  std_msgs__msg__Header header;
  auto_aim_interfaces__msg__Base__Sequence bases;
} auto_aim_interfaces__msg__Bases;

// Struct for a sequence of auto_aim_interfaces__msg__Bases.
typedef struct auto_aim_interfaces__msg__Bases__Sequence
{
  auto_aim_interfaces__msg__Bases * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__Bases__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__BASES__STRUCT_H_
