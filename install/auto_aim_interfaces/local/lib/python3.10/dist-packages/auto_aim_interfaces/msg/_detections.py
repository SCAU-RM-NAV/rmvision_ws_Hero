# generated from rosidl_generator_py/resource/_idl.py.em
# with input from auto_aim_interfaces:msg/Detections.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

# Member 'imu_to_camera'
import numpy  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_Detections(type):
    """Metaclass of message 'Detections'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('auto_aim_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'auto_aim_interfaces.msg.Detections')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__detections
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__detections
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__detections
            cls._TYPE_SUPPORT = module.type_support_msg__msg__detections
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__detections

            from auto_aim_interfaces.msg import Detection
            if Detection.__class__._TYPE_SUPPORT is None:
                Detection.__class__.__import_type_support__()

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class Detections(metaclass=Metaclass_Detections):
    """Message class 'Detections'."""

    __slots__ = [
        '_header',
        '_detections',
        '_inif_id',
        '_my_id',
        '_imu_to_camera',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'detections': 'sequence<auto_aim_interfaces/Detection>',
        'inif_id': 'int16',
        'my_id': 'int16',
        'imu_to_camera': 'double[9]',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.NamespacedType(['auto_aim_interfaces', 'msg'], 'Detection')),  # noqa: E501
        rosidl_parser.definition.BasicType('int16'),  # noqa: E501
        rosidl_parser.definition.BasicType('int16'),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('double'), 9),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.detections = kwargs.get('detections', [])
        self.inif_id = kwargs.get('inif_id', int())
        self.my_id = kwargs.get('my_id', int())
        if 'imu_to_camera' not in kwargs:
            self.imu_to_camera = numpy.zeros(9, dtype=numpy.float64)
        else:
            self.imu_to_camera = numpy.array(kwargs.get('imu_to_camera'), dtype=numpy.float64)
            assert self.imu_to_camera.shape == (9, )

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.detections != other.detections:
            return False
        if self.inif_id != other.inif_id:
            return False
        if self.my_id != other.my_id:
            return False
        if all(self.imu_to_camera != other.imu_to_camera):
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def detections(self):
        """Message field 'detections'."""
        return self._detections

    @detections.setter
    def detections(self, value):
        if __debug__:
            from auto_aim_interfaces.msg import Detection
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, Detection) for v in value) and
                 True), \
                "The 'detections' field must be a set or sequence and each value of type 'Detection'"
        self._detections = value

    @builtins.property
    def inif_id(self):
        """Message field 'inif_id'."""
        return self._inif_id

    @inif_id.setter
    def inif_id(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'inif_id' field must be of type 'int'"
            assert value >= -32768 and value < 32768, \
                "The 'inif_id' field must be an integer in [-32768, 32767]"
        self._inif_id = value

    @builtins.property
    def my_id(self):
        """Message field 'my_id'."""
        return self._my_id

    @my_id.setter
    def my_id(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'my_id' field must be of type 'int'"
            assert value >= -32768 and value < 32768, \
                "The 'my_id' field must be an integer in [-32768, 32767]"
        self._my_id = value

    @builtins.property
    def imu_to_camera(self):
        """Message field 'imu_to_camera'."""
        return self._imu_to_camera

    @imu_to_camera.setter
    def imu_to_camera(self, value):
        if isinstance(value, numpy.ndarray):
            assert value.dtype == numpy.float64, \
                "The 'imu_to_camera' numpy.ndarray() must have the dtype of 'numpy.float64'"
            assert value.size == 9, \
                "The 'imu_to_camera' numpy.ndarray() must have a size of 9"
            self._imu_to_camera = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 9 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -1.7976931348623157e+308 or val > 1.7976931348623157e+308) or math.isinf(val) for val in value)), \
                "The 'imu_to_camera' field must be a set or sequence with length 9 and each value of type 'float' and each double in [-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000]"
        self._imu_to_camera = numpy.array(value, dtype=numpy.float64)
