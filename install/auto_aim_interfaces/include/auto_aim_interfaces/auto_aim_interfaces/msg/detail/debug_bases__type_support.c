// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from auto_aim_interfaces:msg/DebugBases.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "auto_aim_interfaces/msg/detail/debug_bases__rosidl_typesupport_introspection_c.h"
#include "auto_aim_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "auto_aim_interfaces/msg/detail/debug_bases__functions.h"
#include "auto_aim_interfaces/msg/detail/debug_bases__struct.h"


// Include directives for member types
// Member `data`
#include "auto_aim_interfaces/msg/debug_base.h"
// Member `data`
#include "auto_aim_interfaces/msg/detail/debug_base__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  auto_aim_interfaces__msg__DebugBases__init(message_memory);
}

void auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_fini_function(void * message_memory)
{
  auto_aim_interfaces__msg__DebugBases__fini(message_memory);
}

size_t auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__size_function__DebugBases__data(
  const void * untyped_member)
{
  const auto_aim_interfaces__msg__DebugBase__Sequence * member =
    (const auto_aim_interfaces__msg__DebugBase__Sequence *)(untyped_member);
  return member->size;
}

const void * auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__get_const_function__DebugBases__data(
  const void * untyped_member, size_t index)
{
  const auto_aim_interfaces__msg__DebugBase__Sequence * member =
    (const auto_aim_interfaces__msg__DebugBase__Sequence *)(untyped_member);
  return &member->data[index];
}

void * auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__get_function__DebugBases__data(
  void * untyped_member, size_t index)
{
  auto_aim_interfaces__msg__DebugBase__Sequence * member =
    (auto_aim_interfaces__msg__DebugBase__Sequence *)(untyped_member);
  return &member->data[index];
}

void auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__fetch_function__DebugBases__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto_aim_interfaces__msg__DebugBase * item =
    ((const auto_aim_interfaces__msg__DebugBase *)
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__get_const_function__DebugBases__data(untyped_member, index));
  auto_aim_interfaces__msg__DebugBase * value =
    (auto_aim_interfaces__msg__DebugBase *)(untyped_value);
  *value = *item;
}

void auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__assign_function__DebugBases__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto_aim_interfaces__msg__DebugBase * item =
    ((auto_aim_interfaces__msg__DebugBase *)
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__get_function__DebugBases__data(untyped_member, index));
  const auto_aim_interfaces__msg__DebugBase * value =
    (const auto_aim_interfaces__msg__DebugBase *)(untyped_value);
  *item = *value;
}

bool auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__resize_function__DebugBases__data(
  void * untyped_member, size_t size)
{
  auto_aim_interfaces__msg__DebugBase__Sequence * member =
    (auto_aim_interfaces__msg__DebugBase__Sequence *)(untyped_member);
  auto_aim_interfaces__msg__DebugBase__Sequence__fini(member);
  return auto_aim_interfaces__msg__DebugBase__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_member_array[1] = {
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(auto_aim_interfaces__msg__DebugBases, data),  // bytes offset in struct
    NULL,  // default value
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__size_function__DebugBases__data,  // size() function pointer
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__get_const_function__DebugBases__data,  // get_const(index) function pointer
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__get_function__DebugBases__data,  // get(index) function pointer
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__fetch_function__DebugBases__data,  // fetch(index, &value) function pointer
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__assign_function__DebugBases__data,  // assign(index, value) function pointer
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__resize_function__DebugBases__data  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_members = {
  "auto_aim_interfaces__msg",  // message namespace
  "DebugBases",  // message name
  1,  // number of fields
  sizeof(auto_aim_interfaces__msg__DebugBases),
  auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_member_array,  // message members
  auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_init_function,  // function to initialize message memory (memory has to be allocated)
  auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_type_support_handle = {
  0,
  &auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_auto_aim_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, auto_aim_interfaces, msg, DebugBases)() {
  auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, auto_aim_interfaces, msg, DebugBase)();
  if (!auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_type_support_handle.typesupport_identifier) {
    auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &auto_aim_interfaces__msg__DebugBases__rosidl_typesupport_introspection_c__DebugBases_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
