// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from auto_aim_interfaces:msg/NaviFdb.idl
// generated code does not contain a copyright notice
#include "auto_aim_interfaces/msg/detail/navi_fdb__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
auto_aim_interfaces__msg__NaviFdb__init(auto_aim_interfaces__msg__NaviFdb * msg)
{
  if (!msg) {
    return false;
  }
  // z
  // x
  return true;
}

void
auto_aim_interfaces__msg__NaviFdb__fini(auto_aim_interfaces__msg__NaviFdb * msg)
{
  if (!msg) {
    return;
  }
  // z
  // x
}

bool
auto_aim_interfaces__msg__NaviFdb__are_equal(const auto_aim_interfaces__msg__NaviFdb * lhs, const auto_aim_interfaces__msg__NaviFdb * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // z
  if (lhs->z != rhs->z) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  return true;
}

bool
auto_aim_interfaces__msg__NaviFdb__copy(
  const auto_aim_interfaces__msg__NaviFdb * input,
  auto_aim_interfaces__msg__NaviFdb * output)
{
  if (!input || !output) {
    return false;
  }
  // z
  output->z = input->z;
  // x
  output->x = input->x;
  return true;
}

auto_aim_interfaces__msg__NaviFdb *
auto_aim_interfaces__msg__NaviFdb__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__NaviFdb * msg = (auto_aim_interfaces__msg__NaviFdb *)allocator.allocate(sizeof(auto_aim_interfaces__msg__NaviFdb), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(auto_aim_interfaces__msg__NaviFdb));
  bool success = auto_aim_interfaces__msg__NaviFdb__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
auto_aim_interfaces__msg__NaviFdb__destroy(auto_aim_interfaces__msg__NaviFdb * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    auto_aim_interfaces__msg__NaviFdb__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
auto_aim_interfaces__msg__NaviFdb__Sequence__init(auto_aim_interfaces__msg__NaviFdb__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__NaviFdb * data = NULL;

  if (size) {
    data = (auto_aim_interfaces__msg__NaviFdb *)allocator.zero_allocate(size, sizeof(auto_aim_interfaces__msg__NaviFdb), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = auto_aim_interfaces__msg__NaviFdb__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        auto_aim_interfaces__msg__NaviFdb__fini(&data[i - 1]);
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
auto_aim_interfaces__msg__NaviFdb__Sequence__fini(auto_aim_interfaces__msg__NaviFdb__Sequence * array)
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
      auto_aim_interfaces__msg__NaviFdb__fini(&array->data[i]);
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

auto_aim_interfaces__msg__NaviFdb__Sequence *
auto_aim_interfaces__msg__NaviFdb__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  auto_aim_interfaces__msg__NaviFdb__Sequence * array = (auto_aim_interfaces__msg__NaviFdb__Sequence *)allocator.allocate(sizeof(auto_aim_interfaces__msg__NaviFdb__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = auto_aim_interfaces__msg__NaviFdb__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
auto_aim_interfaces__msg__NaviFdb__Sequence__destroy(auto_aim_interfaces__msg__NaviFdb__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    auto_aim_interfaces__msg__NaviFdb__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
auto_aim_interfaces__msg__NaviFdb__Sequence__are_equal(const auto_aim_interfaces__msg__NaviFdb__Sequence * lhs, const auto_aim_interfaces__msg__NaviFdb__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!auto_aim_interfaces__msg__NaviFdb__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
auto_aim_interfaces__msg__NaviFdb__Sequence__copy(
  const auto_aim_interfaces__msg__NaviFdb__Sequence * input,
  auto_aim_interfaces__msg__NaviFdb__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(auto_aim_interfaces__msg__NaviFdb);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    auto_aim_interfaces__msg__NaviFdb * data =
      (auto_aim_interfaces__msg__NaviFdb *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!auto_aim_interfaces__msg__NaviFdb__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          auto_aim_interfaces__msg__NaviFdb__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!auto_aim_interfaces__msg__NaviFdb__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
