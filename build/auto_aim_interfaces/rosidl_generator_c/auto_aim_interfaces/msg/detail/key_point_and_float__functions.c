// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from auto_aim_interfaces:msg/KeyPointAndFloat.idl
// generated code does not contain a copyright notice
#include "auto_aim_interfaces/msg/detail/key_point_and_float__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `key_point`
#include "auto_aim_interfaces/msg/detail/key_point2_f__functions.h"

bool
auto_aim_interfaces__msg__KeyPointAndFloat__init(auto_aim_interfaces__msg__KeyPointAndFloat * msg)
{
  if (!msg) {
    return false;
  }
  // key_point
  if (!auto_aim_interfaces__msg__KeyPoint2F__Sequence__init(&msg->key_point, 0)) {
    auto_aim_interfaces__msg__KeyPointAndFloat__fini(msg);
    return false;
  }
  return true;
}

void
auto_aim_interfaces__msg__KeyPointAndFloat__fini(auto_aim_interfaces__msg__KeyPointAndFloat * msg)
{
  if (!msg) {
    return;
  }
  // key_point
  auto_aim_interfaces__msg__KeyPoint2F__Sequence__fini(&msg->key_point);
}

bool
auto_aim_interfaces__msg__KeyPointAndFloat__are_equal(const auto_aim_interfaces__msg__KeyPointAndFloat * lhs, const auto_aim_interfaces__msg__KeyPointAndFloat * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // key_point
  if (!auto_aim_interfaces__msg__KeyPoint2F__Sequence__are_equal(
      &(lhs->key_point), &(rhs->key_point)))
  {
    return false;
  }
  return true;
}

bool
auto_aim_interfaces__msg__KeyPointAndFloat__copy(
  const auto_aim_interfaces__msg__KeyPointAndFloat * input,
  auto_aim_interfaces__msg__KeyPointAndFloat * output)
{
  if (!input || !output) {
    return false;
  }
  // key_point
  if (!auto_aim_interfaces__msg__KeyPoint2F__Sequence__copy(
      &(input->key_point), &(output->key_point)))
  {
    return false;
  }
  return true;
}

auto_aim_interfaces__msg__KeyPointAndFloat *
auto_aim_interfaces__msg__KeyPointAndFloat__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__KeyPointAndFloat * msg = (auto_aim_interfaces__msg__KeyPointAndFloat *)allocator.allocate(sizeof(auto_aim_interfaces__msg__KeyPointAndFloat), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(auto_aim_interfaces__msg__KeyPointAndFloat));
  bool success = auto_aim_interfaces__msg__KeyPointAndFloat__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
auto_aim_interfaces__msg__KeyPointAndFloat__destroy(auto_aim_interfaces__msg__KeyPointAndFloat * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    auto_aim_interfaces__msg__KeyPointAndFloat__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__init(auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__KeyPointAndFloat * data = NULL;

  if (size) {
    data = (auto_aim_interfaces__msg__KeyPointAndFloat *)allocator.zero_allocate(size, sizeof(auto_aim_interfaces__msg__KeyPointAndFloat), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = auto_aim_interfaces__msg__KeyPointAndFloat__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        auto_aim_interfaces__msg__KeyPointAndFloat__fini(&data[i - 1]);
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
auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__fini(auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * array)
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
      auto_aim_interfaces__msg__KeyPointAndFloat__fini(&array->data[i]);
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

auto_aim_interfaces__msg__KeyPointAndFloat__Sequence *
auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * array = (auto_aim_interfaces__msg__KeyPointAndFloat__Sequence *)allocator.allocate(sizeof(auto_aim_interfaces__msg__KeyPointAndFloat__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__destroy(auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__are_equal(const auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * lhs, const auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!auto_aim_interfaces__msg__KeyPointAndFloat__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
auto_aim_interfaces__msg__KeyPointAndFloat__Sequence__copy(
  const auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * input,
  auto_aim_interfaces__msg__KeyPointAndFloat__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(auto_aim_interfaces__msg__KeyPointAndFloat);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    auto_aim_interfaces__msg__KeyPointAndFloat * data =
      (auto_aim_interfaces__msg__KeyPointAndFloat *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!auto_aim_interfaces__msg__KeyPointAndFloat__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          auto_aim_interfaces__msg__KeyPointAndFloat__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!auto_aim_interfaces__msg__KeyPointAndFloat__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
