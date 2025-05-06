// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from auto_aim_interfaces:msg/GimbalFdb.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__STRUCT_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__auto_aim_interfaces__msg__GimbalFdb __attribute__((deprecated))
#else
# define DEPRECATED__auto_aim_interfaces__msg__GimbalFdb __declspec(deprecated)
#endif

namespace auto_aim_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct GimbalFdb_
{
  using Type = GimbalFdb_<ContainerAllocator>;

  explicit GimbalFdb_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->imu_yaw = 0.0;
      this->imu_yaw_spd = 0.0;
      this->imu_pit = 0.0;
      this->imu_pit_spd = 0.0;
      this->camp = 0ll;
      this->priority_number = 0ll;
      this->aiming_mode = 0ll;
    }
  }

  explicit GimbalFdb_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->imu_yaw = 0.0;
      this->imu_yaw_spd = 0.0;
      this->imu_pit = 0.0;
      this->imu_pit_spd = 0.0;
      this->camp = 0ll;
      this->priority_number = 0ll;
      this->aiming_mode = 0ll;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _imu_yaw_type =
    double;
  _imu_yaw_type imu_yaw;
  using _imu_yaw_spd_type =
    double;
  _imu_yaw_spd_type imu_yaw_spd;
  using _imu_pit_type =
    double;
  _imu_pit_type imu_pit;
  using _imu_pit_spd_type =
    double;
  _imu_pit_spd_type imu_pit_spd;
  using _camp_type =
    int64_t;
  _camp_type camp;
  using _priority_number_type =
    int64_t;
  _priority_number_type priority_number;
  using _aiming_mode_type =
    int64_t;
  _aiming_mode_type aiming_mode;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__imu_yaw(
    const double & _arg)
  {
    this->imu_yaw = _arg;
    return *this;
  }
  Type & set__imu_yaw_spd(
    const double & _arg)
  {
    this->imu_yaw_spd = _arg;
    return *this;
  }
  Type & set__imu_pit(
    const double & _arg)
  {
    this->imu_pit = _arg;
    return *this;
  }
  Type & set__imu_pit_spd(
    const double & _arg)
  {
    this->imu_pit_spd = _arg;
    return *this;
  }
  Type & set__camp(
    const int64_t & _arg)
  {
    this->camp = _arg;
    return *this;
  }
  Type & set__priority_number(
    const int64_t & _arg)
  {
    this->priority_number = _arg;
    return *this;
  }
  Type & set__aiming_mode(
    const int64_t & _arg)
  {
    this->aiming_mode = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator> *;
  using ConstRawPtr =
    const auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__auto_aim_interfaces__msg__GimbalFdb
    std::shared_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__auto_aim_interfaces__msg__GimbalFdb
    std::shared_ptr<auto_aim_interfaces::msg::GimbalFdb_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GimbalFdb_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->imu_yaw != other.imu_yaw) {
      return false;
    }
    if (this->imu_yaw_spd != other.imu_yaw_spd) {
      return false;
    }
    if (this->imu_pit != other.imu_pit) {
      return false;
    }
    if (this->imu_pit_spd != other.imu_pit_spd) {
      return false;
    }
    if (this->camp != other.camp) {
      return false;
    }
    if (this->priority_number != other.priority_number) {
      return false;
    }
    if (this->aiming_mode != other.aiming_mode) {
      return false;
    }
    return true;
  }
  bool operator!=(const GimbalFdb_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GimbalFdb_

// alias to use template instance with default allocator
using GimbalFdb =
  auto_aim_interfaces::msg::GimbalFdb_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_FDB__STRUCT_HPP_
