// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from auto_aim_interfaces:msg/DebugBases.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__STRUCT_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'data'
#include "auto_aim_interfaces/msg/detail/debug_base__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__auto_aim_interfaces__msg__DebugBases __attribute__((deprecated))
#else
# define DEPRECATED__auto_aim_interfaces__msg__DebugBases __declspec(deprecated)
#endif

namespace auto_aim_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct DebugBases_
{
  using Type = DebugBases_<ContainerAllocator>;

  explicit DebugBases_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit DebugBases_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _data_type =
    std::vector<auto_aim_interfaces::msg::DebugBase_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<auto_aim_interfaces::msg::DebugBase_<ContainerAllocator>>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__data(
    const std::vector<auto_aim_interfaces::msg::DebugBase_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<auto_aim_interfaces::msg::DebugBase_<ContainerAllocator>>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    auto_aim_interfaces::msg::DebugBases_<ContainerAllocator> *;
  using ConstRawPtr =
    const auto_aim_interfaces::msg::DebugBases_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::DebugBases_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::DebugBases_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__auto_aim_interfaces__msg__DebugBases
    std::shared_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__auto_aim_interfaces__msg__DebugBases
    std::shared_ptr<auto_aim_interfaces::msg::DebugBases_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const DebugBases_ & other) const
  {
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const DebugBases_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct DebugBases_

// alias to use template instance with default allocator
using DebugBases =
  auto_aim_interfaces::msg::DebugBases_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__DEBUG_BASES__STRUCT_HPP_
