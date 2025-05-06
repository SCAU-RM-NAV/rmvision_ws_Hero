import os
import sys
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
sys.path.append(os.path.join(get_package_share_directory('rm_vision_bringup'), 'launch'))


def generate_launch_description():

  
  from common import launch_params, robot_state_publisher, node_params, tracker_node
  from launch_ros.actions import Node
  from launch import LaunchDescription

  node_params = os.path.join(
    get_package_share_directory('armor_creater'), 'config', 'creater_params.yaml')

  creater_node = Node(
    package='armor_creater',
    executable='armor_creater_node',
    name='armor_creater',
    # emulate_tty=True,
    output='both',
    respawn=True,
    respawn_delay=2,
    parameters=[node_params],
    # arguments=['--ros-args', '--log-level']
  )

  # 设置静态 tf 变换的参数
  translation = [0.0, 0.0, 0.0]
  rotation = [0.0, 0.0, 0.0, 1.0]  # 四元数 [x, y, z, w]
  cov_matrix = [0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1]  # 协方差矩阵

  static_tf_args = {
      'translation': translation,
      'rotation': rotation,
      'translation_covariance': [0.0, 0.0, 0.0],
      'rotation_covariance': cov_matrix
  }

  # 发布静态 tf 变换的节点
  static_tf_publisher_node = Node(
      package='tf2_ros',
      executable='static_transform_publisher',
      arguments=[str(translation[0]), str(translation[1]), str(translation[2]), 
                  str(rotation[0]), str(rotation[1]), str(rotation[2]), str(rotation[3]), 
                  'world', 'gimbal_link'],

      output='screen',
      respawn=True,
      respawn_delay=2,
  )

  detector_node = Node(
    package='armor_detector',
    executable='armor_detector_node',
    emulate_tty=True,
    output='both',
    respawn=True,
    respawn_delay=2,
    parameters=[node_params],
    arguments=['--ros-args', '--log-level',
                'armor_detector:='+launch_params['detector_log_level']],
  )
  tracker_node = Node(
    package='armor_tracker',
    executable='armor_tracker_node',
    output='both',
    emulate_tty=True,
    respawn=True,
    respawn_delay=2,
    parameters=[node_params], 
    ros_arguments=['--log-level', 'armor_tracker:='+launch_params['tracker_log_level']],
)

  return LaunchDescription([
    creater_node,
    static_tf_publisher_node
  ])
