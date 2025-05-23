// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from auto_aim_interfaces:msg/TrackerInfo.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "auto_aim_interfaces/msg/detail/tracker_info__rosidl_typesupport_introspection_c.h"
#include "auto_aim_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "auto_aim_interfaces/msg/detail/tracker_info__functions.h"
#include "auto_aim_interfaces/msg/detail/tracker_info__struct.h"


// Include directives for member types
// Member `position`
#include "geometry_msgs/msg/point.h"
// Member `position`
#include "geometry_msgs/msg/detail/point__rosidl_typesupport_introspection_c.h"
// Member `sigma2_state`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  auto_aim_interfaces__msg__TrackerInfo__init(message_memory);
}

void auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_fini_function(void * message_memory)
{
  auto_aim_interfaces__msg__TrackerInfo__fini(message_memory);
}

size_t auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__size_function__TrackerInfo__sigma2_state(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__get_const_function__TrackerInfo__sigma2_state(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__get_function__TrackerInfo__sigma2_state(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__fetch_function__TrackerInfo__sigma2_state(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__get_const_function__TrackerInfo__sigma2_state(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__assign_function__TrackerInfo__sigma2_state(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__get_function__TrackerInfo__sigma2_state(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__resize_function__TrackerInfo__sigma2_state(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_member_array[5] = {
  {
    "position_diff",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__TrackerInfo, position_diff),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "yaw_diff",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__TrackerInfo, yaw_diff),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__TrackerInfo, position),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "yaw",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__TrackerInfo, yaw),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "sigma2_state",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__TrackerInfo, sigma2_state),  // bytes offset in struct
    NULL,  // default value
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__size_function__TrackerInfo__sigma2_state,  // size() function pointer
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__get_const_function__TrackerInfo__sigma2_state,  // get_const(index) function pointer
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__get_function__TrackerInfo__sigma2_state,  // get(index) function pointer
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__fetch_function__TrackerInfo__sigma2_state,  // fetch(index, &value) function pointer
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__assign_function__TrackerInfo__sigma2_state,  // assign(index, value) function pointer
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__resize_function__TrackerInfo__sigma2_state  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_members = {
  "auto_aim_interfaces__msg",  // message namespace
  "TrackerInfo",  // message name
  5,  // number of fields
  sizeof(auto_aim_interfaces__msg__TrackerInfo),
  auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_member_array,  // message members
  auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_init_function,  // function to initialize message memory (memory has to be allocated)
  auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_type_support_handle = {
  0,
  &auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_auto_aim_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, auto_aim_interfaces, msg, TrackerInfo)() {
  auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, Point)();
  if (!auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_type_support_handle.typesupport_identifier) {
    auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &auto_aim_interfaces__msg__TrackerInfo__rosidl_typesupport_introspection_c__TrackerInfo_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
