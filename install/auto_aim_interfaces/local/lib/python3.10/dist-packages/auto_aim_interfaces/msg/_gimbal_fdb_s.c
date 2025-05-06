// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from auto_aim_interfaces:msg/GimbalFdb.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "auto_aim_interfaces/msg/detail/gimbal_fdb__struct.h"
#include "auto_aim_interfaces/msg/detail/gimbal_fdb__functions.h"

ROSIDL_GENERATOR_C_IMPORT
bool std_msgs__msg__header__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * std_msgs__msg__header__convert_to_py(void * raw_ros_message);

ROSIDL_GENERATOR_C_EXPORT
bool auto_aim_interfaces__msg__gimbal_fdb__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[46];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("auto_aim_interfaces.msg._gimbal_fdb.GimbalFdb", full_classname_dest, 45) == 0);
  }
  auto_aim_interfaces__msg__GimbalFdb * ros_message = _ros_message;
  {  // header
    PyObject * field = PyObject_GetAttrString(_pymsg, "header");
    if (!field) {
      return false;
    }
    if (!std_msgs__msg__header__convert_from_py(field, &ros_message->header)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }
  {  // imu_yaw
    PyObject * field = PyObject_GetAttrString(_pymsg, "imu_yaw");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->imu_yaw = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // imu_yaw_spd
    PyObject * field = PyObject_GetAttrString(_pymsg, "imu_yaw_spd");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->imu_yaw_spd = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // imu_pit
    PyObject * field = PyObject_GetAttrString(_pymsg, "imu_pit");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->imu_pit = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // imu_pit_spd
    PyObject * field = PyObject_GetAttrString(_pymsg, "imu_pit_spd");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->imu_pit_spd = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // camp
    PyObject * field = PyObject_GetAttrString(_pymsg, "camp");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->camp = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }
  {  // priority_number
    PyObject * field = PyObject_GetAttrString(_pymsg, "priority_number");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->priority_number = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }
  {  // aiming_mode
    PyObject * field = PyObject_GetAttrString(_pymsg, "aiming_mode");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->aiming_mode = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * auto_aim_interfaces__msg__gimbal_fdb__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of GimbalFdb */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("auto_aim_interfaces.msg._gimbal_fdb");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "GimbalFdb");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  auto_aim_interfaces__msg__GimbalFdb * ros_message = (auto_aim_interfaces__msg__GimbalFdb *)raw_ros_message;
  {  // header
    PyObject * field = NULL;
    field = std_msgs__msg__header__convert_to_py(&ros_message->header);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "header", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // imu_yaw
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->imu_yaw);
    {
      int rc = PyObject_SetAttrString(_pymessage, "imu_yaw", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // imu_yaw_spd
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->imu_yaw_spd);
    {
      int rc = PyObject_SetAttrString(_pymessage, "imu_yaw_spd", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // imu_pit
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->imu_pit);
    {
      int rc = PyObject_SetAttrString(_pymessage, "imu_pit", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // imu_pit_spd
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->imu_pit_spd);
    {
      int rc = PyObject_SetAttrString(_pymessage, "imu_pit_spd", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // camp
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->camp);
    {
      int rc = PyObject_SetAttrString(_pymessage, "camp", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // priority_number
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->priority_number);
    {
      int rc = PyObject_SetAttrString(_pymessage, "priority_number", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // aiming_mode
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->aiming_mode);
    {
      int rc = PyObject_SetAttrString(_pymessage, "aiming_mode", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
