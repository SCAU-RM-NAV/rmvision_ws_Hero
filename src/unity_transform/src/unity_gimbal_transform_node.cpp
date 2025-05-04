#include "rclcpp/rclcpp.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/convert.h>
#include "tf2_ros/transform_broadcaster.h"
#include "sensor_msgs/msg/joint_state.hpp"
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

// STD
#include <memory>
#include <string>
#include <vector>

class GimbalTransformNode : public rclcpp::Node
{
public:
  GimbalTransformNode()
    : Node("gimbal_transform_node")
  {
    joint_state_sub_ = create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", 10, std::bind(&GimbalTransformNode::jointStateCallback, this, std::placeholders::_1));

    tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
  }

private:
  void jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg)
  {
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = msg->header.stamp;
    transform.header.frame_id = "world";
    transform.child_frame_id = "gimbal_link";

    tf2::Quaternion q;
    q.setRPY(0.0,msg->position[1],msg->position[0] );
    transform.transform.rotation = tf2::toMsg(q);

    tf_broadcaster_->sendTransform(transform);
  }

  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_sub_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GimbalTransformNode>());
  rclcpp::shutdown();
  return 0;
}