// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from auto_aim_interfaces:msg/GimbalCtrl.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__STRUCT_H_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/GimbalCtrl in the package auto_aim_interfaces.
/**
  * test vision status
 */
typedef struct auto_aim_interfaces__msg__GimbalCtrl
{
  double yaw;
  double pit;
  double shoot_yaw_tole;
  double fire_flag;
  double pos;
  uint8_t empty;
  uint8_t cnt;
  uint8_t ist_flag;
  uint8_t aim_flag;
  int32_t x;
  uint8_t base_flag;
} auto_aim_interfaces__msg__GimbalCtrl;

// Struct for a sequence of auto_aim_interfaces__msg__GimbalCtrl.
typedef struct auto_aim_interfaces__msg__GimbalCtrl__Sequence
{
  auto_aim_interfaces__msg__GimbalCtrl * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} auto_aim_interfaces__msg__GimbalCtrl__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__STRUCT_H_
