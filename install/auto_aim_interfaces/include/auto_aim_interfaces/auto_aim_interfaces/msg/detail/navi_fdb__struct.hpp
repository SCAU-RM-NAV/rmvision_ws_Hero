// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from auto_aim_interfaces:msg/NaviFdb.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__NAVI_FDB__STRUCT_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__NAVI_FDB__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__auto_aim_interfaces__msg__NaviFdb __attribute__((deprecated))
#else
# define DEPRECATED__auto_aim_interfaces__msg__NaviFdb __declspec(deprecated)
#endif

namespace auto_aim_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct NaviFdb_
{
  using Type = NaviFdb_<ContainerAllocator>;

  explicit NaviFdb_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->z = 0.0;
      this->x = 0.0;
    }
  }

  explicit NaviFdb_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->z = 0.0;
      this->x = 0.0;
    }
  }

  // field types and members
  using _z_type =
    double;
  _z_type z;
  using _x_type =
    double;
  _x_type x;

  // setters for named parameter idiom
  Type & set__z(
    const double & _arg)
  {
    this->z = _arg;
    return *this;
  }
  Type & set__x(
    const double & _arg)
  {
    this->x = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator> *;
  using ConstRawPtr =
    const auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__auto_aim_interfaces__msg__NaviFdb
    std::shared_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__auto_aim_interfaces__msg__NaviFdb
    std::shared_ptr<auto_aim_interfaces::msg::NaviFdb_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const NaviFdb_ & other) const
  {
    if (this->z != other.z) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    return true;
  }
  bool operator!=(const NaviFdb_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct NaviFdb_

// alias to use template instance with default allocator
using NaviFdb =
  auto_aim_interfaces::msg::NaviFdb_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__NAVI_FDB__STRUCT_HPP_
