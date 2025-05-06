// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from auto_aim_interfaces:msg/KeyPoint2F.idl
// generated code does not contain a copyright notice
#include "auto_aim_interfaces/msg/detail/key_point2_f__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
auto_aim_interfaces__msg__KeyPoint2F__init(auto_aim_interfaces__msg__KeyPoint2F * msg)
{
  if (!msg) {
    return false;
  }
  // x
  // y
  return true;
}

void
auto_aim_interfaces__msg__KeyPoint2F__fini(auto_aim_interfaces__msg__KeyPoint2F * msg)
{
  if (!msg) {
    return;
  }
  // x
  // y
}

bool
auto_aim_interfaces__msg__KeyPoint2F__are_equal(const auto_aim_interfaces__msg__KeyPoint2F * lhs, const auto_aim_interfaces__msg__KeyPoint2F * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  return true;
}

bool
auto_aim_interfaces__msg__KeyPoint2F__copy(
  const auto_aim_interfaces__msg__KeyPoint2F * input,
  auto_aim_interfaces__msg__KeyPoint2F * output)
{
  if (!input || !output) {
    return false;
  }
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  return true;
}

auto_aim_interfaces__msg__KeyPoint2F *
auto_aim_interfaces__msg__KeyPoint2F__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__KeyPoint2F * msg = (auto_aim_interfaces__msg__KeyPoint2F *)allocator.allocate(sizeof(auto_aim_interfaces__msg__KeyPoint2F), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(auto_aim_interfaces__msg__KeyPoint2F));
  bool success = auto_aim_interfaces__msg__KeyPoint2F__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
auto_aim_interfaces__msg__KeyPoint2F__destroy(auto_aim_interfaces__msg__KeyPoint2F * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    auto_aim_interfaces__msg__KeyPoint2F__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
auto_aim_interfaces__msg__KeyPoint2F__Sequence__init(auto_aim_interfaces__msg__KeyPoint2F__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__KeyPoint2F * data = NULL;

  if (size) {
    data = (auto_aim_interfaces__msg__KeyPoint2F *)allocator.zero_allocate(size, sizeof(auto_aim_interfaces__msg__KeyPoint2F), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = auto_aim_interfaces__msg__KeyPoint2F__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        auto_aim_interfaces__msg__KeyPoint2F__fini(&data[i - 1]);
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
auto_aim_interfaces__msg__KeyPoint2F__Sequence__fini(auto_aim_interfaces__msg__KeyPoint2F__Sequence * array)
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
      auto_aim_interfaces__msg__KeyPoint2F__fini(&array->data[i]);
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

auto_aim_interfaces__msg__KeyPoint2F__Sequence *
auto_aim_interfaces__msg__KeyPoint2F__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__KeyPoint2F__Sequence * array = (auto_aim_interfaces__msg__KeyPoint2F__Sequence *)allocator.allocate(sizeof(auto_aim_interfaces__msg__KeyPoint2F__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = auto_aim_interfaces__msg__KeyPoint2F__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
auto_aim_interfaces__msg__KeyPoint2F__Sequence__destroy(auto_aim_interfaces__msg__KeyPoint2F__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    auto_aim_interfaces__msg__KeyPoint2F__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
auto_aim_interfaces__msg__KeyPoint2F__Sequence__are_equal(const auto_aim_interfaces__msg__KeyPoint2F__Sequence * lhs, const auto_aim_interfaces__msg__KeyPoint2F__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!auto_aim_interfaces__msg__KeyPoint2F__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
auto_aim_interfaces__msg__KeyPoint2F__Sequence__copy(
  const auto_aim_interfaces__msg__KeyPoint2F__Sequence * input,
  auto_aim_interfaces__msg__KeyPoint2F__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(auto_aim_interfaces__msg__KeyPoint2F);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    auto_aim_interfaces__msg__KeyPoint2F * data =
      (auto_aim_interfaces__msg__KeyPoint2F *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!auto_aim_interfaces__msg__KeyPoint2F__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          auto_aim_interfaces__msg__KeyPoint2F__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!auto_aim_interfaces__msg__KeyPoint2F__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
