// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from auto_aim_interfaces:msg/GimbalFdb.idl
// generated code does not contain a copyright notice
#include "auto_aim_interfaces/msg/detail/gimbal_fdb__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
auto_aim_interfaces__msg__GimbalFdb__init(auto_aim_interfaces__msg__GimbalFdb * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    auto_aim_interfaces__msg__GimbalFdb__fini(msg);
    return false;
  }
  // imu_yaw
  // imu_yaw_spd
  // imu_pit
  // imu_pit_spd
  // camp
  // priority_number
  // aiming_mode
  return true;
}

void
auto_aim_interfaces__msg__GimbalFdb__fini(auto_aim_interfaces__msg__GimbalFdb * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // imu_yaw
  // imu_yaw_spd
  // imu_pit
  // imu_pit_spd
  // camp
  // priority_number
  // aiming_mode
}

bool
auto_aim_interfaces__msg__GimbalFdb__are_equal(const auto_aim_interfaces__msg__GimbalFdb * lhs, const auto_aim_interfaces__msg__GimbalFdb * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // imu_yaw
  if (lhs->imu_yaw != rhs->imu_yaw) {
    return false;
  }
  // imu_yaw_spd
  if (lhs->imu_yaw_spd != rhs->imu_yaw_spd) {
    return false;
  }
  // imu_pit
  if (lhs->imu_pit != rhs->imu_pit) {
    return false;
  }
  // imu_pit_spd
  if (lhs->imu_pit_spd != rhs->imu_pit_spd) {
    return false;
  }
  // camp
  if (lhs->camp != rhs->camp) {
    return false;
  }
  // priority_number
  if (lhs->priority_number != rhs->priority_number) {
    return false;
  }
  // aiming_mode
  if (lhs->aiming_mode != rhs->aiming_mode) {
    return false;
  }
  return true;
}

bool
auto_aim_interfaces__msg__GimbalFdb__copy(
  const auto_aim_interfaces__msg__GimbalFdb * input,
  auto_aim_interfaces__msg__GimbalFdb * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // imu_yaw
  output->imu_yaw = input->imu_yaw;
  // imu_yaw_spd
  output->imu_yaw_spd = input->imu_yaw_spd;
  // imu_pit
  output->imu_pit = input->imu_pit;
  // imu_pit_spd
  output->imu_pit_spd = input->imu_pit_spd;
  // camp
  output->camp = input->camp;
  // priority_number
  output->priority_number = input->priority_number;
  // aiming_mode
  output->aiming_mode = input->aiming_mode;
  return true;
}

auto_aim_interfaces__msg__GimbalFdb *
auto_aim_interfaces__msg__GimbalFdb__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__GimbalFdb * msg = (auto_aim_interfaces__msg__GimbalFdb *)allocator.allocate(sizeof(auto_aim_interfaces__msg__GimbalFdb), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(auto_aim_interfaces__msg__GimbalFdb));
  bool success = auto_aim_interfaces__msg__GimbalFdb__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
auto_aim_interfaces__msg__GimbalFdb__destroy(auto_aim_interfaces__msg__GimbalFdb * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    auto_aim_interfaces__msg__GimbalFdb__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
auto_aim_interfaces__msg__GimbalFdb__Sequence__init(auto_aim_interfaces__msg__GimbalFdb__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__GimbalFdb * data = NULL;

  if (size) {
    data = (auto_aim_interfaces__msg__GimbalFdb *)allocator.zero_allocate(size, sizeof(auto_aim_interfaces__msg__GimbalFdb), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = auto_aim_interfaces__msg__GimbalFdb__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        auto_aim_interfaces__msg__GimbalFdb__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
auto_aim_interfaces__msg__GimbalFdb__Sequence__fini(auto_aim_interfaces__msg__GimbalFdb__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      auto_aim_interfaces__msg__GimbalFdb__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

auto_aim_interfaces__msg__GimbalFdb__Sequence *
auto_aim_interfaces__msg__GimbalFdb__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__GimbalFdb__Sequence * array = (auto_aim_interfaces__msg__GimbalFdb__Sequence *)allocator.allocate(sizeof(auto_aim_interfaces__msg__GimbalFdb__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = auto_aim_interfaces__msg__GimbalFdb__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
auto_aim_interfaces__msg__GimbalFdb__Sequence__destroy(auto_aim_interfaces__msg__GimbalFdb__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    auto_aim_interfaces__msg__GimbalFdb__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
auto_aim_interfaces__msg__GimbalFdb__Sequence__are_equal(const auto_aim_interfaces__msg__GimbalFdb__Sequence * lhs, const auto_aim_interfaces__msg__GimbalFdb__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!auto_aim_interfaces__msg__GimbalFdb__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
auto_aim_interfaces__msg__GimbalFdb__Sequence__copy(
  const auto_aim_interfaces__msg__GimbalFdb__Sequence * input,
  auto_aim_interfaces__msg__GimbalFdb__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(auto_aim_interfaces__msg__GimbalFdb);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    auto_aim_interfaces__msg__GimbalFdb * data =
      (auto_aim_interfaces__msg__GimbalFdb *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!auto_aim_interfaces__msg__GimbalFdb__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          auto_aim_interfaces__msg__GimbalFdb__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!auto_aim_interfaces__msg__GimbalFdb__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
