# generated from rosidl_generator_py/resource/_idl.py.em
# with input from auto_aim_interfaces:msg/GimbalFdb.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_GimbalFdb(type):
    """Metaclass of message 'GimbalFdb'."""

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
                'auto_aim_interfaces.msg.GimbalFdb')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__gimbal_fdb
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__gimbal_fdb
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__gimbal_fdb
            cls._TYPE_SUPPORT = module.type_support_msg__msg__gimbal_fdb
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__gimbal_fdb

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


class GimbalFdb(metaclass=Metaclass_GimbalFdb):
    """Message class 'GimbalFdb'."""

    __slots__ = [
        '_header',
        '_imu_yaw',
        '_imu_yaw_spd',
        '_imu_pit',
        '_imu_pit_spd',
        '_camp',
        '_priority_number',
        '_aiming_mode',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'imu_yaw': 'double',
        'imu_yaw_spd': 'double',
        'imu_pit': 'double',
        'imu_pit_spd': 'double',
        'camp': 'int64',
        'priority_number': 'int64',
        'aiming_mode': 'int64',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        self.imu_yaw = kwargs.get('imu_yaw', float())
        self.imu_yaw_spd = kwargs.get('imu_yaw_spd', float())
        self.imu_pit = kwargs.get('imu_pit', float())
        self.imu_pit_spd = kwargs.get('imu_pit_spd', float())
        self.camp = kwargs.get('camp', int())
        self.priority_number = kwargs.get('priority_number', int())
        self.aiming_mode = kwargs.get('aiming_mode', int())

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
        if self.imu_yaw != other.imu_yaw:
            return False
        if self.imu_yaw_spd != other.imu_yaw_spd:
            return False
        if self.imu_pit != other.imu_pit:
            return False
        if self.imu_pit_spd != other.imu_pit_spd:
            return False
        if self.camp != other.camp:
            return False
        if self.priority_number != other.priority_number:
            return False
        if self.aiming_mode != other.aiming_mode:
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
    def imu_yaw(self):
        """Message field 'imu_yaw'."""
        return self._imu_yaw

    @imu_yaw.setter
    def imu_yaw(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'imu_yaw' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'imu_yaw' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._imu_yaw = value

    @builtins.property
    def imu_yaw_spd(self):
        """Message field 'imu_yaw_spd'."""
        return self._imu_yaw_spd

    @imu_yaw_spd.setter
    def imu_yaw_spd(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'imu_yaw_spd' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'imu_yaw_spd' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._imu_yaw_spd = value

    @builtins.property
    def imu_pit(self):
        """Message field 'imu_pit'."""
        return self._imu_pit

    @imu_pit.setter
    def imu_pit(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'imu_pit' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'imu_pit' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._imu_pit = value

    @builtins.property
    def imu_pit_spd(self):
        """Message field 'imu_pit_spd'."""
        return self._imu_pit_spd

    @imu_pit_spd.setter
    def imu_pit_spd(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'imu_pit_spd' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'imu_pit_spd' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._imu_pit_spd = value

    @builtins.property
    def camp(self):
        """Message field 'camp'."""
        return self._camp

    @camp.setter
    def camp(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'camp' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'camp' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._camp = value

    @builtins.property
    def priority_number(self):
        """Message field 'priority_number'."""
        return self._priority_number

    @priority_number.setter
    def priority_number(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'priority_number' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'priority_number' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._priority_number = value

    @builtins.property
    def aiming_mode(self):
        """Message field 'aiming_mode'."""
        return self._aiming_mode

    @aiming_mode.setter
    def aiming_mode(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'aiming_mode' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'aiming_mode' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._aiming_mode = value
