# generated from rosidl_generator_py/resource/_idl.py.em
# with input from auto_aim_interfaces:msg/GimbalCtrl.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_GimbalCtrl(type):
    """Metaclass of message 'GimbalCtrl'."""

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
                'auto_aim_interfaces.msg.GimbalCtrl')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__gimbal_ctrl
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__gimbal_ctrl
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__gimbal_ctrl
            cls._TYPE_SUPPORT = module.type_support_msg__msg__gimbal_ctrl
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__gimbal_ctrl

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class GimbalCtrl(metaclass=Metaclass_GimbalCtrl):
    """Message class 'GimbalCtrl'."""

    __slots__ = [
        '_yaw',
        '_pit',
        '_shoot_yaw_tole',
        '_fire_flag',
        '_pos',
        '_empty',
        '_cnt',
        '_ist_flag',
        '_aim_flag',
        '_x',
        '_base_flag',
    ]

    _fields_and_field_types = {
        'yaw': 'double',
        'pit': 'double',
        'shoot_yaw_tole': 'double',
        'fire_flag': 'double',
        'pos': 'double',
        'empty': 'uint8',
        'cnt': 'uint8',
        'ist_flag': 'uint8',
        'aim_flag': 'uint8',
        'x': 'int32',
        'base_flag': 'uint8',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.yaw = kwargs.get('yaw', float())
        self.pit = kwargs.get('pit', float())
        self.shoot_yaw_tole = kwargs.get('shoot_yaw_tole', float())
        self.fire_flag = kwargs.get('fire_flag', float())
        self.pos = kwargs.get('pos', float())
        self.empty = kwargs.get('empty', int())
        self.cnt = kwargs.get('cnt', int())
        self.ist_flag = kwargs.get('ist_flag', int())
        self.aim_flag = kwargs.get('aim_flag', int())
        self.x = kwargs.get('x', int())
        self.base_flag = kwargs.get('base_flag', int())

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
        if self.yaw != other.yaw:
            return False
        if self.pit != other.pit:
            return False
        if self.shoot_yaw_tole != other.shoot_yaw_tole:
            return False
        if self.fire_flag != other.fire_flag:
            return False
        if self.pos != other.pos:
            return False
        if self.empty != other.empty:
            return False
        if self.cnt != other.cnt:
            return False
        if self.ist_flag != other.ist_flag:
            return False
        if self.aim_flag != other.aim_flag:
            return False
        if self.x != other.x:
            return False
        if self.base_flag != other.base_flag:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def yaw(self):
        """Message field 'yaw'."""
        return self._yaw

    @yaw.setter
    def yaw(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'yaw' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'yaw' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._yaw = value

    @builtins.property
    def pit(self):
        """Message field 'pit'."""
        return self._pit

    @pit.setter
    def pit(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'pit' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'pit' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._pit = value

    @builtins.property
    def shoot_yaw_tole(self):
        """Message field 'shoot_yaw_tole'."""
        return self._shoot_yaw_tole

    @shoot_yaw_tole.setter
    def shoot_yaw_tole(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'shoot_yaw_tole' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'shoot_yaw_tole' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._shoot_yaw_tole = value

    @builtins.property
    def fire_flag(self):
        """Message field 'fire_flag'."""
        return self._fire_flag

    @fire_flag.setter
    def fire_flag(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'fire_flag' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'fire_flag' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._fire_flag = value

    @builtins.property
    def pos(self):
        """Message field 'pos'."""
        return self._pos

    @pos.setter
    def pos(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'pos' field must be of type 'float'"
            assert not (value < -1.7976931348623157e+308 or value > 1.7976931348623157e+308) or math.isinf(value), \
                "The 'pos' field must be a double in [-1.7976931348623157e+308, 1.7976931348623157e+308]"
        self._pos = value

    @builtins.property
    def empty(self):
        """Message field 'empty'."""
        return self._empty

    @empty.setter
    def empty(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'empty' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'empty' field must be an unsigned integer in [0, 255]"
        self._empty = value

    @builtins.property
    def cnt(self):
        """Message field 'cnt'."""
        return self._cnt

    @cnt.setter
    def cnt(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'cnt' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'cnt' field must be an unsigned integer in [0, 255]"
        self._cnt = value

    @builtins.property
    def ist_flag(self):
        """Message field 'ist_flag'."""
        return self._ist_flag

    @ist_flag.setter
    def ist_flag(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'ist_flag' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'ist_flag' field must be an unsigned integer in [0, 255]"
        self._ist_flag = value

    @builtins.property
    def aim_flag(self):
        """Message field 'aim_flag'."""
        return self._aim_flag

    @aim_flag.setter
    def aim_flag(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'aim_flag' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'aim_flag' field must be an unsigned integer in [0, 255]"
        self._aim_flag = value

    @builtins.property
    def x(self):
        """Message field 'x'."""
        return self._x

    @x.setter
    def x(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'x' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'x' field must be an integer in [-2147483648, 2147483647]"
        self._x = value

    @builtins.property
    def base_flag(self):
        """Message field 'base_flag'."""
        return self._base_flag

    @base_flag.setter
    def base_flag(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'base_flag' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'base_flag' field must be an unsigned integer in [0, 255]"
        self._base_flag = value
