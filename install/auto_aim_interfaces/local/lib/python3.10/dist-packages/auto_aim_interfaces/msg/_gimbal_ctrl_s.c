// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from auto_aim_interfaces:msg/GimbalCtrl.idl
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
#include "auto_aim_interfaces/msg/detail/gimbal_ctrl__struct.h"
#include "auto_aim_interfaces/msg/detail/gimbal_ctrl__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool auto_aim_interfaces__msg__gimbal_ctrl__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[48];
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
    assert(strncmp("auto_aim_interfaces.msg._gimbal_ctrl.GimbalCtrl", full_classname_dest, 47) == 0);
  }
  auto_aim_interfaces__msg__GimbalCtrl * ros_message = _ros_message;
  {  // yaw
    PyObject * field = PyObject_GetAttrString(_pymsg, "yaw");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->yaw = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // pit
    PyObject * field = PyObject_GetAttrString(_pymsg, "pit");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->pit = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // shoot_yaw_tole
    PyObject * field = PyObject_GetAttrString(_pymsg, "shoot_yaw_tole");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->shoot_yaw_tole = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // fire_flag
    PyObject * field = PyObject_GetAttrString(_pymsg, "fire_flag");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->fire_flag = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // pos
    PyObject * field = PyObject_GetAttrString(_pymsg, "pos");
    if (!field) {
      return false;
    }
    assert(PyFloat_Check(field));
    ros_message->pos = PyFloat_AS_DOUBLE(field);
    Py_DECREF(field);
  }
  {  // empty
    PyObject * field = PyObject_GetAttrString(_pymsg, "empty");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->empty = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // cnt
    PyObject * field = PyObject_GetAttrString(_pymsg, "cnt");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->cnt = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // ist_flag
    PyObject * field = PyObject_GetAttrString(_pymsg, "ist_flag");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->ist_flag = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // aim_flag
    PyObject * field = PyObject_GetAttrString(_pymsg, "aim_flag");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->aim_flag = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // x
    PyObject * field = PyObject_GetAttrString(_pymsg, "x");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->x = (int32_t)PyLong_AsLong(field);
    Py_DECREF(field);
  }
  {  // base_flag
    PyObject * field = PyObject_GetAttrString(_pymsg, "base_flag");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->base_flag = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * auto_aim_interfaces__msg__gimbal_ctrl__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of GimbalCtrl */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("auto_aim_interfaces.msg._gimbal_ctrl");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "GimbalCtrl");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  auto_aim_interfaces__msg__GimbalCtrl * ros_message = (auto_aim_interfaces__msg__GimbalCtrl *)raw_ros_message;
  {  // yaw
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->yaw);
    {
      int rc = PyObject_SetAttrString(_pymessage, "yaw", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // pit
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->pit);
    {
      int rc = PyObject_SetAttrString(_pymessage, "pit", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // shoot_yaw_tole
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->shoot_yaw_tole);
    {
      int rc = PyObject_SetAttrString(_pymessage, "shoot_yaw_tole", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // fire_flag
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->fire_flag);
    {
      int rc = PyObject_SetAttrString(_pymessage, "fire_flag", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // pos
    PyObject * field = NULL;
    field = PyFloat_FromDouble(ros_message->pos);
    {
      int rc = PyObject_SetAttrString(_pymessage, "pos", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // empty
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->empty);
    {
      int rc = PyObject_SetAttrString(_pymessage, "empty", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // cnt
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->cnt);
    {
      int rc = PyObject_SetAttrString(_pymessage, "cnt", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ist_flag
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->ist_flag);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ist_flag", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // aim_flag
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->aim_flag);
    {
      int rc = PyObject_SetAttrString(_pymessage, "aim_flag", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // x
    PyObject * field = NULL;
    field = PyLong_FromLong(ros_message->x);
    {
      int rc = PyObject_SetAttrString(_pymessage, "x", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // base_flag
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->base_flag);
    {
      int rc = PyObject_SetAttrString(_pymessage, "base_flag", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
