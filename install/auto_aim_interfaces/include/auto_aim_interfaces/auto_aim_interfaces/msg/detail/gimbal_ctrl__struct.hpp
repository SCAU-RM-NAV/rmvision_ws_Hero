// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from auto_aim_interfaces:msg/GimbalCtrl.idl
// generated code does not contain a copyright notice

#ifndef AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__STRUCT_HPP_
#define AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__auto_aim_interfaces__msg__GimbalCtrl __attribute__((deprecated))
#else
# define DEPRECATED__auto_aim_interfaces__msg__GimbalCtrl __declspec(deprecated)
#endif

namespace auto_aim_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct GimbalCtrl_
{
  using Type = GimbalCtrl_<ContainerAllocator>;

  explicit GimbalCtrl_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->yaw = 0.0;
      this->pit = 0.0;
      this->shoot_yaw_tole = 0.0;
      this->fire_flag = 0.0;
      this->pos = 0.0;
      this->empty = 0;
      this->cnt = 0;
      this->ist_flag = 0;
      this->aim_flag = 0;
      this->x = 0l;
      this->base_flag = 0;
    }
  }

  explicit GimbalCtrl_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->yaw = 0.0;
      this->pit = 0.0;
      this->shoot_yaw_tole = 0.0;
      this->fire_flag = 0.0;
      this->pos = 0.0;
      this->empty = 0;
      this->cnt = 0;
      this->ist_flag = 0;
      this->aim_flag = 0;
      this->x = 0l;
      this->base_flag = 0;
    }
  }

  // field types and members
  using _yaw_type =
    double;
  _yaw_type yaw;
  using _pit_type =
    double;
  _pit_type pit;
  using _shoot_yaw_tole_type =
    double;
  _shoot_yaw_tole_type shoot_yaw_tole;
  using _fire_flag_type =
    double;
  _fire_flag_type fire_flag;
  using _pos_type =
    double;
  _pos_type pos;
  using _empty_type =
    uint8_t;
  _empty_type empty;
  using _cnt_type =
    uint8_t;
  _cnt_type cnt;
  using _ist_flag_type =
    uint8_t;
  _ist_flag_type ist_flag;
  using _aim_flag_type =
    uint8_t;
  _aim_flag_type aim_flag;
  using _x_type =
    int32_t;
  _x_type x;
  using _base_flag_type =
    uint8_t;
  _base_flag_type base_flag;

  // setters for named parameter idiom
  Type & set__yaw(
    const double & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__pit(
    const double & _arg)
  {
    this->pit = _arg;
    return *this;
  }
  Type & set__shoot_yaw_tole(
    const double & _arg)
  {
    this->shoot_yaw_tole = _arg;
    return *this;
  }
  Type & set__fire_flag(
    const double & _arg)
  {
    this->fire_flag = _arg;
    return *this;
  }
  Type & set__pos(
    const double & _arg)
  {
    this->pos = _arg;
    return *this;
  }
  Type & set__empty(
    const uint8_t & _arg)
  {
    this->empty = _arg;
    return *this;
  }
  Type & set__cnt(
    const uint8_t & _arg)
  {
    this->cnt = _arg;
    return *this;
  }
  Type & set__ist_flag(
    const uint8_t & _arg)
  {
    this->ist_flag = _arg;
    return *this;
  }
  Type & set__aim_flag(
    const uint8_t & _arg)
  {
    this->aim_flag = _arg;
    return *this;
  }
  Type & set__x(
    const int32_t & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__base_flag(
    const uint8_t & _arg)
  {
    this->base_flag = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator> *;
  using ConstRawPtr =
    const auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__auto_aim_interfaces__msg__GimbalCtrl
    std::shared_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__auto_aim_interfaces__msg__GimbalCtrl
    std::shared_ptr<auto_aim_interfaces::msg::GimbalCtrl_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const GimbalCtrl_ & other) const
  {
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->pit != other.pit) {
      return false;
    }
    if (this->shoot_yaw_tole != other.shoot_yaw_tole) {
      return false;
    }
    if (this->fire_flag != other.fire_flag) {
      return false;
    }
    if (this->pos != other.pos) {
      return false;
    }
    if (this->empty != other.empty) {
      return false;
    }
    if (this->cnt != other.cnt) {
      return false;
    }
    if (this->ist_flag != other.ist_flag) {
      return false;
    }
    if (this->aim_flag != other.aim_flag) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->base_flag != other.base_flag) {
      return false;
    }
    return true;
  }
  bool operator!=(const GimbalCtrl_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct GimbalCtrl_

// alias to use template instance with default allocator
using GimbalCtrl =
  auto_aim_interfaces::msg::GimbalCtrl_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace auto_aim_interfaces

#endif  // AUTO_AIM_INTERFACES__MSG__DETAIL__GIMBAL_CTRL__STRUCT_HPP_
