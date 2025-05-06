// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/DebugBases.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'data'
#include "auto_aim_interfaces/msg/detail/debug_base__struct.h"

/// Struct defined in msg/DebugBases in the package auto_aim_interfaces.
typedef struct auto_aim_interfaces__msg__DebugBases
{
  auto_aim_interfaces__msg__DebugBase__Sequence data;
} auto_aim_interfaces__msg__DebugBases;

// Struct for a sequence of auto_aim_interfaces__msg__DebugBases.
typedef struct auto_aim_interfaces__msg__DebugBases__Sequence
{
  auto_aim_interfaces__msg__DebugBases * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__DebugBases__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__STRUCT_H_
