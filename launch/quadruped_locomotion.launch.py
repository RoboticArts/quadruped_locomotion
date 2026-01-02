from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch.actions import GroupAction
from launch_ros.actions import Node
from launch import LaunchDescription

def generate_launch_description():
    
    quadruped_locomotion_path = PathJoinSubstitution([
        FindPackageShare('quadruped_locomotion'),
        'config',
        'quadruped_drivers.yaml'
    ])

    quadruped_locomotion = Node(
        package="quadruped_locomotion",
        executable="quadruped_locomotion",
        arguments=['--driver-config', quadruped_locomotion_path],
        output="screen",
    )

    group = GroupAction([
        quadruped_locomotion
    ])

    return LaunchDescription([group])