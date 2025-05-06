// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from auto_aim_interfaces:msg/Detections.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "auto_aim_interfaces/msg/detail/detections__rosidl_typesupport_introspection_c.h"
#include "auto_aim_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "auto_aim_interfaces/msg/detail/detections__functions.h"
#include "auto_aim_interfaces/msg/detail/detections__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `detections`
#include "auto_aim_interfaces/msg/detection.h"
// Member `detections`
#include "auto_aim_interfaces/msg/detail/detection__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  auto_aim_interfaces__msg__Detections__init(message_memory);
}

void auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_fini_function(void * message_memory)
{
  auto_aim_interfaces__msg__Detections__fini(message_memory);
}

size_t auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__size_function__Detections__detections(
  const void * untyped_member)
{
  const auto_aim_interfaces__msg__Detection__Sequence * member =
    (const auto_aim_interfaces__msg__Detection__Sequence *)(untyped_member);
  return member->size;
}

const void * auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_const_function__Detections__detections(
  const void * untyped_member, size_t index)
{
  const auto_aim_interfaces__msg__Detection__Sequence * member =
    (const auto_aim_interfaces__msg__Detection__Sequence *)(untyped_member);
  return &member->data[index];
}

void * auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_function__Detections__detections(
  void * untyped_member, size_t index)
{
  auto_aim_interfaces__msg__Detection__Sequence * member =
    (auto_aim_interfaces__msg__Detection__Sequence *)(untyped_member);
  return &member->data[index];
}

void auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__fetch_function__Detections__detections(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto_aim_interfaces__msg__Detection * item =
    ((const auto_aim_interfaces__msg__Detection *)
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_const_function__Detections__detections(untyped_member, index));
  auto_aim_interfaces__msg__Detection * value =
    (auto_aim_interfaces__msg__Detection *)(untyped_value);
  *value = *item;
}

void auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__assign_function__Detections__detections(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto_aim_interfaces__msg__Detection * item =
    ((auto_aim_interfaces__msg__Detection *)
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_function__Detections__detections(untyped_member, index));
  const auto_aim_interfaces__msg__Detection * value =
    (const auto_aim_interfaces__msg__Detection *)(untyped_value);
  *item = *value;
}

bool auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__resize_function__Detections__detections(
  void * untyped_member, size_t size)
{
  auto_aim_interfaces__msg__Detection__Sequence * member =
    (auto_aim_interfaces__msg__Detection__Sequence *)(untyped_member);
  auto_aim_interfaces__msg__Detection__Sequence__fini(member);
  return auto_aim_interfaces__msg__Detection__Sequence__init(member, size);
}

size_t auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__size_function__Detections__imu_to_camera(
  const void * untyped_member)
{
  (void)untyped_member;
  return 9;
}

const void * auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_const_function__Detections__imu_to_camera(
  const void * untyped_member, size_t index)
{
  const double * member =
    (const double *)(untyped_member);
  return &member[index];
}

void * auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_function__Detections__imu_to_camera(
  void * untyped_member, size_t index)
{
  double * member =
    (double *)(untyped_member);
  return &member[index];
}

void auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__fetch_function__Detections__imu_to_camera(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_const_function__Detections__imu_to_camera(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__assign_function__Detections__imu_to_camera(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_function__Detections__imu_to_camera(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_member_array[5] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__Detections, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "detections",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__Detections, detections),  // bytes offset in struct
    NULL,  // default value
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__size_function__Detections__detections,  // size() function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_const_function__Detections__detections,  // get_const(index) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_function__Detections__detections,  // get(index) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__fetch_function__Detections__detections,  // fetch(index, &value) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__assign_function__Detections__detections,  // assign(index, value) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__resize_function__Detections__detections  // resize(index) function pointer
  },
  {
    "inif_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__Detections, inif_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "my_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__Detections, my_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "imu_to_camera",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    9,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__Detections, imu_to_camera),  // bytes offset in struct
    NULL,  // default value
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__size_function__Detections__imu_to_camera,  // size() function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_const_function__Detections__imu_to_camera,  // get_const(index) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__get_function__Detections__imu_to_camera,  // get(index) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__fetch_function__Detections__imu_to_camera,  // fetch(index, &value) function pointer
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__assign_function__Detections__imu_to_camera,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_members = {
  "auto_aim_interfaces__msg",  // message namespace
  "Detections",  // message name
  5,  // number of fields
  sizeof(auto_aim_interfaces__msg__Detections),
  auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_member_array,  // message members
  auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_init_function,  // function to initialize message memory (memory has to be allocated)
  auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_type_support_handle = {
  0,
  &auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_auto_aim_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, auto_aim_interfaces, msg, Detections)() {
  auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, auto_aim_interfaces, msg, Detection)();
  if (!auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_type_support_handle.typesupport_identifier) {
    auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &auto_aim_interfaces__msg__Detections__rosidl_typesupport_introspection_c__Detections_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
