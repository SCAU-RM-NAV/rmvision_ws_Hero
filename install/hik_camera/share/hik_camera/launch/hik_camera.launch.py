import os

from ament_index_python.packages import get_package_share_directory, get_package_prefix
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    params_file = os.path.join(
        get_package_share_directory('hik_camera'), 'config', 'camera_params_0.yaml')
    
    camera_info_url = 'package://hik_camera/config/camera_info.yaml'

    return LaunchDescription([
        DeclareLaunchArgument(name='params_file',
                              default_value=params_file),
        DeclareLaunchArgument(name='camera_info_url',
                              default_value=camera_info_url),
        DeclareLaunchArgument(name='use_sensor_data_qos',
                              default_value='false'),

        Node(
            package='hik_camera',
            executable='hik_camera_Node',
            output='screen',
            emulate_tty=True,
            # prefix=['taskset', ' -c', ' 0'],
            parameters=[LaunchConfiguration('params_file'), {
                'camera_info_url': LaunchConfiguration('camera_info_url'),
                'use_sensor_data_qos': LaunchConfiguration('use_sensor_data_qos'),
            }],
        )
    ])
