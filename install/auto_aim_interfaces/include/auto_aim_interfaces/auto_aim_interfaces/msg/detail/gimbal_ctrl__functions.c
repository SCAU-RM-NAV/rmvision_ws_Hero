// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from auto_aim_interfaces:msg/GimbalCtrl.idl
// generated code does not contain a copyright notice
#include "auto_aim_interfaces/msg/detail/gimbal_ctrl__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
auto_aim_interfaces__msg__GimbalCtrl__init(auto_aim_interfaces__msg__GimbalCtrl * msg)
{
  if (!msg) {
    return false;
  }
  // yaw
  // pit
  // shoot_yaw_tole
  // fire_flag
  // pos
  // empty
  // cnt
  // ist_flag
  // aim_flag
  // x
  // base_flag
  return true;
}

void
auto_aim_interfaces__msg__GimbalCtrl__fini(auto_aim_interfaces__msg__GimbalCtrl * msg)
{
  if (!msg) {
    return;
  }
  // yaw
  // pit
  // shoot_yaw_tole
  // fire_flag
  // pos
  // empty
  // cnt
  // ist_flag
  // aim_flag
  // x
  // base_flag
}

bool
auto_aim_interfaces__msg__GimbalCtrl__are_equal(const auto_aim_interfaces__msg__GimbalCtrl * lhs, const auto_aim_interfaces__msg__GimbalCtrl * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // pit
  if (lhs->pit != rhs->pit) {
    return false;
  }
  // shoot_yaw_tole
  if (lhs->shoot_yaw_tole != rhs->shoot_yaw_tole) {
    return false;
  }
  // fire_flag
  if (lhs->fire_flag != rhs->fire_flag) {
    return false;
  }
  // pos
  if (lhs->pos != rhs->pos) {
    return false;
  }
  // empty
  if (lhs->empty != rhs->empty) {
    return false;
  }
  // cnt
  if (lhs->cnt != rhs->cnt) {
    return false;
  }
  // ist_flag
  if (lhs->ist_flag != rhs->ist_flag) {
    return false;
  }
  // aim_flag
  if (lhs->aim_flag != rhs->aim_flag) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // base_flag
  if (lhs->base_flag != rhs->base_flag) {
    return false;
  }
  return true;
}

bool
auto_aim_interfaces__msg__GimbalCtrl__copy(
  const auto_aim_interfaces__msg__GimbalCtrl * input,
  auto_aim_interfaces__msg__GimbalCtrl * output)
{
  if (!input || !output) {
    return false;
  }
  // yaw
  output->yaw = input->yaw;
  // pit
  output->pit = input->pit;
  // shoot_yaw_tole
  output->shoot_yaw_tole = input->shoot_yaw_tole;
  // fire_flag
  output->fire_flag = input->fire_flag;
  // pos
  output->pos = input->pos;
  // empty
  output->empty = input->empty;
  // cnt
  output->cnt = input->cnt;
  // ist_flag
  output->ist_flag = input->ist_flag;
  // aim_flag
  output->aim_flag = input->aim_flag;
  // x
  output->x = input->x;
  // base_flag
  output->base_flag = input->base_flag;
  return true;
}

auto_aim_interfaces__msg__GimbalCtrl *
auto_aim_interfaces__msg__GimbalCtrl__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__GimbalCtrl * msg = (auto_aim_interfaces__msg__GimbalCtrl *)allocator.allocate(sizeof(auto_aim_interfaces__msg__GimbalCtrl), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(auto_aim_interfaces__msg__GimbalCtrl));
  bool success = auto_aim_interfaces__msg__GimbalCtrl__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
auto_aim_interfaces__msg__GimbalCtrl__destroy(auto_aim_interfaces__msg__GimbalCtrl * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    auto_aim_interfaces__msg__GimbalCtrl__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
auto_aim_interfaces__msg__GimbalCtrl__Sequence__init(auto_aim_interfaces__msg__GimbalCtrl__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__GimbalCtrl * data = NULL;

  if (size) {
    data = (auto_aim_interfaces__msg__GimbalCtrl *)allocator.zero_allocate(size, sizeof(auto_aim_interfaces__msg__GimbalCtrl), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = auto_aim_interfaces__msg__GimbalCtrl__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        auto_aim_interfaces__msg__GimbalCtrl__fini(&data[i - 1]);
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
auto_aim_interfaces__msg__GimbalCtrl__Sequence__fini(auto_aim_interfaces__msg__GimbalCtrl__Sequence * array)
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
      auto_aim_interfaces__msg__GimbalCtrl__fini(&array->data[i]);
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

auto_aim_interfaces__msg__GimbalCtrl__Sequence *
auto_aim_interfaces__msg__GimbalCtrl__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__GimbalCtrl__Sequence * array = (auto_aim_interfaces__msg__GimbalCtrl__Sequence *)allocator.allocate(sizeof(auto_aim_interfaces__msg__GimbalCtrl__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = auto_aim_interfaces__msg__GimbalCtrl__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
auto_aim_interfaces__msg__GimbalCtrl__Sequence__destroy(auto_aim_interfaces__msg__GimbalCtrl__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    auto_aim_interfaces__msg__GimbalCtrl__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
auto_aim_interfaces__msg__GimbalCtrl__Sequence__are_equal(const auto_aim_interfaces__msg__GimbalCtrl__Sequence * lhs, const auto_aim_interfaces__msg__GimbalCtrl__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!auto_aim_interfaces__msg__GimbalCtrl__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
auto_aim_interfaces__msg__GimbalCtrl__Sequence__copy(
  const auto_aim_interfaces__msg__GimbalCtrl__Sequence * input,
  auto_aim_interfaces__msg__GimbalCtrl__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(auto_aim_interfaces__msg__GimbalCtrl);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    auto_aim_interfaces__msg__GimbalCtrl * data =
      (auto_aim_interfaces__msg__GimbalCtrl *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!auto_aim_interfaces__msg__GimbalCtrl__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          auto_aim_interfaces__msg__GimbalCtrl__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!auto_aim_interfaces__msg__GimbalCtrl__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
