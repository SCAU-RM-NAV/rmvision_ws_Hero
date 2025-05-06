// #include <rclcpp/rclcpp.hpp>
// #include <tf2/LinearMath/Quaternion.h>
// #include "tf2_ros/transform_broadcaster.h" 
// #include "tf2_ros/transform_listener.h"
// #include <tf2_ros/buffer.h>
// #include "geometry_msgs/msg/pose.hpp"
// #include "geometry_msgs/msg/transform_stamped.hpp"
#include <rclcpp/rclcpp.hpp>
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include "nav_msgs/msg/odometry.hpp"
#include "tf2_ros/transform_broadcaster.h" 
// #include "tf/transform_datatypes.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2/time.h"
#include <tf2_ros/buffer.h>
#include "geometry_msgs/msg/twist.hpp"
#include <mutex>
#include "auto_aim_interfaces/msg/fdb.hpp"


#define GRAVITY 9.78
#define PI 3.1415
typedef unsigned char uint8_t;


namespace taurus
{
class Navi_cal : public rclcpp::Node{
  public:
    Navi_cal(const rclcpp::NodeOptions &options);
    ~Navi_cal();
  private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::TimerBase::SharedPtr navier_;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> transform_listener;
    geometry_msgs::msg::Pose robot_pose_;
    double target_x_ = 18.228789;
    double target_y_ = 1.697352;
    double target_z_ = 0.8855;
    double robot_yaw_;
    double robot_roll_;
    double robot_pitch_;
    auto_aim_interfaces::msg::Fdb fdb_msg_;
    rclcpp::Publisher<auto_aim_interfaces::msg::Fdb>::SharedPtr fdb_pub_;
    int R_K_iter = 60;
    double k = 0.000556; //空气阻力系数
  // st.k = 0.01903;                //25°C,1atm,小弹丸
    // st.k = 0.000556;                //25°C,1atm,大弹丸
    // st.k = 0.000530;                //25°C,1atm,发光大弹丸
  //   st.bullet_type =  BULLET_17;
  //   st.current_v = 30;
  float s_bias = 0.26;         //枪口前推的距离
  float z_bias = 0.15;         //雷达到枪口水平面的垂直距离

    //fun
    void timerCallback();
    float pitchTrajectoryCompensation(float s, float z, float v);

    

};   
} // namespace tartus

