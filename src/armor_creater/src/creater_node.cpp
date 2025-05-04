#include "armor_creater/creater_node.hpp"

namespace rm_auto_aim
{

ArmorCreaterNode::ArmorCreaterNode(const rclcpp::NodeOptions & options)
: Node("armor_creater", options)
{
  RCLCPP_INFO(this->get_logger(), "Starting CreaterNode!");

  // initial params
  RobotSumulation::CreaterParams params;
  int mode_num = this->declare_parameter("mode", 0);
  params.mode = static_cast<RobotSumulation::Mode>(mode_num);
  params.robot_radius = this->declare_parameter("robot_radius", 0.3);
  params.vel_sin_range = this->declare_parameter("vel_sin_range", 1.0);
  params.vel_sin_cycle = this->declare_parameter("vel_sin_cycle", 1.0);
  params.vel_sin_offset = this->declare_parameter("vel_sin_offset", 0.0);

  params.pos_stddev = this->declare_parameter("pos_stddev", 0.3);
  params.yaw_stddev = this->declare_parameter("yaw_stddev", 0.3);
  params.euler_stddev = this->declare_parameter("euler_stddev", 0.03);
  params.dis_stddev = this->declare_parameter("dis_stddev", 0.3);

  params.y_sin_range = this->declare_parameter("y_sin_range", 2.0);
  params.y_sin_cycle = this->declare_parameter("y_sin_cycle", 2.0);

  robot_sumulation_ = std::make_unique<RobotSumulation>(params);

  freq_ = this->declare_parameter("freq", 100.0);
  freq_ = get_parameter("freq").as_double();

  //initial timer
  timer_ = this->create_wall_timer(
    std::chrono::milliseconds(static_cast<int>(1000.0 / freq_)), 
    std::bind(&ArmorCreaterNode::timerCallback, this));

  // Armors Publisher
  armors_pub_ = this->create_publisher<auto_aim_interfaces::msg::Armors>(
    "/detector/armors", rclcpp::SensorDataQoS());

  // Publisher
  robot_state_pub_ = this->create_publisher<auto_aim_interfaces::msg::Target>(
    "/creater/robot_state", rclcpp::SensorDataQoS());

  // subscription
  target_sub_ = this->create_subscription<auto_aim_interfaces::msg::Target>(
      "/tracker/target", rclcpp::SensorDataQoS(), 
      std::bind(&ArmorCreaterNode::targetCallback, this, std::placeholders::_1));

  // Visualization Marker Publisher
  // See http://wiki.ros.org/rviz/DisplayTypes/Marker
  position_marker_.ns = "position";
  position_marker_.type = visualization_msgs::msg::Marker::SPHERE;
  position_marker_.scale.x = position_marker_.scale.y = position_marker_.scale.z = 0.1;
  position_marker_.color.a = 0.5;
  position_marker_.color.g = 1.0;
  linear_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
  linear_v_marker_.ns = "linear_v";
  linear_v_marker_.scale.x = 0.03;
  linear_v_marker_.scale.y = 0.05;
  linear_v_marker_.color.a = 0.5;
  linear_v_marker_.color.r = 1.0;
  linear_v_marker_.color.g = 1.0;
  angular_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
  angular_v_marker_.ns = "angular_v";
  angular_v_marker_.scale.x = 0.03;
  angular_v_marker_.scale.y = 0.05;
  angular_v_marker_.color.a = 0.5;
  angular_v_marker_.color.b = 1.0;
  angular_v_marker_.color.g = 1.0;
  armor_marker_.ns = "armors";
  armor_marker_.type = visualization_msgs::msg::Marker::CUBE;
  armor_marker_.scale.x = 0.03;
  armor_marker_.scale.z = 0.125;
  armor_marker_.color.a = 0.5;
  armor_marker_.color.r = 1.0;
  real_marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("/creater/state_marker", 10);

  measure_marker_.ns = "measure_armor";
  measure_marker_.action = visualization_msgs::msg::Marker::ADD;
  measure_marker_.type = visualization_msgs::msg::Marker::CUBE;
  measure_marker_.scale.x = 0.05;
  measure_marker_.scale.z = 0.125;
  measure_marker_.color.a = 1.0;
  measure_marker_.color.g = 0.5;
  measure_marker_.color.b = 1.0;
  measure_marker_.lifetime = rclcpp::Duration::from_seconds(0.1);
  measure_marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("/creater/measure_marker", 10);

  if (!creater_sender.init("127.0.0.1", 5001)) {
      std::cerr << "Error: Failed to initialize UDP sender." << std::endl;
  }
}

/**
 * 订阅tracker的数据，统计分析状态估计器效果
*/
void ArmorCreaterNode::targetCallback(const auto_aim_interfaces::msg::Target::SharedPtr target_msg)
{

  tracker_deque_.push_back(target_msg);
  // RCLCPP_INFO_STREAM(this->get_logger(), "id : " << target_msg->id);

  // 取出1s前的数据进行处理
  if (get_time_sec(target_msg->header.stamp) - get_time_sec(tracker_deque_.front()->header.stamp) > 1)
  {
    tracker_deque_.pop_front();

    // RCLCPP_INFO_STREAM(this->get_logger(), "target_msg last time : " 
    //                 << std::fixed << std::setprecision(4)   // 控制输出保留的小数位
    //                 << get_time_sec(target_msg->header.stamp) << std::endl
    //                 << "tracker_deque_ front time : "
    //                 << std::fixed << std::setprecision(4)
    //                 << get_time_sec(tracker_deque_.front()->header.stamp));

    // RCLCPP_INFO_STREAM(this->get_logger(),"size : " <<  tracker_deque_.size());
  }

  for (auto it = creater_deque_.rbegin(); it != creater_deque_.rend(); ++it)
  {  
    //使用反向迭代器从后往前遍历
    // RCLCPP_INFO_STREAM(this->get_logger(), "(*it)->header.stamp : " 
    //                 << std::fixed << std::setprecision(4)   // 控制输出保留的小数位
    //                 << get_time_sec((*it)->header.stamp) << std::endl
    //                 << "tracker_deque_ front time : "
    //                 << std::fixed << std::setprecision(4)
    //                 << get_time_sec(tracker_deque_.front()->header.stamp));

    if(get_time_sec((*it)->header.stamp) == get_time_sec(tracker_deque_.front()->header.stamp))
      { //使用反向迭代器从后往前遍历

        // RCLCPP_INFO_STREAM(this->get_logger(), "(*it)->header.stamp : " 
        //                 << std::fixed << std::setprecision(4)   // 控制输出保留的小数位
        //                 << get_time_sec((*it)->header.stamp) << std::endl
        //                 << "tracker_deque_ front time : "
        //                 << std::fixed << std::setprecision(4)
        //                 << get_time_sec(tracker_deque_.front()->header.stamp));

        creater_deque_.erase(creater_deque_.begin(), std::next(it).base());
        std::vector<double> ms_error;
        ms_error = meanSquareError(creater_deque_,tracker_deque_);
        std::cout<<"meanSquareError position  : "<<ms_error[0]<<std::endl;
        std::cout<<"meanSquareError yaw  : "<<ms_error[1]<<std::endl;
        
        creater_vofa_data.fdata[0] = ms_error[0];
        creater_vofa_data.fdata[1] = ms_error[1];
        creater_sender.send_data(creater_vofa_data);  // 发送数据

        break;
    }
  }

  
}

/**
 * 数据生成器定时器回调
*/
void ArmorCreaterNode::timerCallback()
{
  // RCLCPP_INFO(this->get_logger(), "Robot move!");
  updateParams();

  // robot move
  Eigen::VectorXd robot_state = robot_sumulation_->move();

  // publish real robot state
  auto_aim_interfaces::msg::Target state_msg;
  state_msg.header.stamp = this->get_clock()->now();
  state_msg.header.frame_id = "gimbal_link";
  // state_msg.id = tracker_->tracked_id;
  state_msg.armors_num = 4;
  state_msg.position.x = robot_state(0);
  state_msg.velocity.x = robot_state(1);
  state_msg.position.y = robot_state(2);
  state_msg.velocity.y = robot_state(3);
  state_msg.position.z = robot_state(4);
  state_msg.velocity.z = robot_state(5);
  state_msg.yaw = robot_state(6);
  state_msg.v_yaw = robot_state(7);
  state_msg.radius_1 = robot_state(8);
  state_msg.radius_2 = robot_state(8);
  state_msg.dz = 0;

  robot_state_pub_->publish(state_msg);
  publishStateMarkers(robot_state);
  // auto_aim_interfaces::msg::Target::SharedPtr msg(new auto_aim_interfaces::msg::Target(*state_msg));
  std::shared_ptr<auto_aim_interfaces::msg::Target> state_msg_ptr = 
      std::make_shared<auto_aim_interfaces::msg::Target>(state_msg);
  creater_deque_.emplace_back(state_msg_ptr);

  // simulate measure robot state
  // measure: xa, yz, za, yaw
  Eigen::VectorXd measure_state = robot_sumulation_->measure();

  auto_aim_interfaces::msg::Armor armor_msg;
  // Fill basic info
  armor_msg.type = "small";
  armor_msg.number = "3";
  // Fill pose
  armor_msg.pose.position.x = measure_state(0);
  armor_msg.pose.position.y = measure_state(1);
  armor_msg.pose.position.z = measure_state(2);
  tf2::Quaternion quaternion;
  quaternion.setRPY(0, 0, measure_state(3));
  armor_msg.pose.orientation = tf2::toMsg(quaternion);
  armor_msg.distance_to_image_center = 0; // tracker用于选择装甲板，只有一个装甲板直接设置为0

  auto_aim_interfaces::msg::Armors armors_msg;
  // armors_msg.header.stamp = this->get_clock()->now();
  armors_msg.header.stamp = state_msg_ptr->header.stamp;
  armors_msg.header.frame_id = "gimbal_link";
  armors_msg.armors.emplace_back(armor_msg);
  // Publishing detected armors
  armors_pub_->publish(armors_msg);

  publishMeasureMarkers(armor_msg);
}

/**
 * 更新参数
*/
void ArmorCreaterNode::updateParams()
{

  int mode_num = get_parameter("mode").as_int();
  robot_sumulation_->params_.mode = static_cast<RobotSumulation::Mode>(mode_num);
  robot_sumulation_->params_.robot_radius = get_parameter("robot_radius").as_double();
  robot_sumulation_->params_.robot_radius = get_parameter("robot_radius").as_double();
  robot_sumulation_->params_.vel_sin_range = get_parameter("vel_sin_range").as_double();
  robot_sumulation_->params_.vel_sin_cycle = get_parameter("vel_sin_cycle").as_double();
  robot_sumulation_->params_.vel_sin_offset = get_parameter("vel_sin_offset").as_double();
  robot_sumulation_->params_.pos_stddev = get_parameter("pos_stddev").as_double();
  robot_sumulation_->params_.yaw_stddev = get_parameter("yaw_stddev").as_double();
  robot_sumulation_->params_.y_sin_range = get_parameter("y_sin_range").as_double();
  robot_sumulation_->params_.y_sin_cycle = get_parameter("y_sin_cycle").as_double();
}

void ArmorCreaterNode::publishMeasureMarkers(auto_aim_interfaces::msg::Armor armor)
{
  // 创建一个std_msgs/Header消息
  std_msgs::msg::Header header;
  header.stamp = this->get_clock()->now(); // 设置时间戳为当前时间
  header.frame_id = "gimbal_link"; // 设置frame_id

  measure_marker_.header = header;
  measure_marker_.id = 0;
  measure_marker_.scale.y = 0.135;
  measure_marker_.pose =armor.pose;

  visualization_msgs::msg::MarkerArray marker_array;
  marker_array.markers.emplace_back(measure_marker_);

  measure_marker_pub_->publish(marker_array);
}

void ArmorCreaterNode::publishStateMarkers(Eigen::VectorXd & state)
{
  // 创建一个std_msgs/Header消息
  std_msgs::msg::Header header;
  header.stamp = this->get_clock()->now(); // 设置时间戳为当前时间
  header.frame_id = "gimbal_link"; // 设置frame_id

  position_marker_.header = header;
  linear_v_marker_.header = header;
  angular_v_marker_.header = header;
  armor_marker_.header = header;

  visualization_msgs::msg::MarkerArray marker_array;

  // state: xc, v_xc, yc, v_yc, za, v_za, yaw, v_yaw, r
  double yaw = state(6), r1 = state(8), r2 = state(8);
  double xc = state(0), yc = state(2), za = state(4);
  double vx = state(1), vy = state(3), vz = state(5);
  double dz = 0;

  position_marker_.action = visualization_msgs::msg::Marker::ADD;
  position_marker_.pose.position.x = xc;
  position_marker_.pose.position.y = yc;
  position_marker_.pose.position.z = za + dz / 2;

  linear_v_marker_.action = visualization_msgs::msg::Marker::ADD;
  linear_v_marker_.points.clear();
  linear_v_marker_.points.emplace_back(position_marker_.pose.position);
  geometry_msgs::msg::Point arrow_end = position_marker_.pose.position;
  arrow_end.x += vx;
  arrow_end.y += vy;
  arrow_end.z += vz;
  linear_v_marker_.points.emplace_back(arrow_end);

  angular_v_marker_.action = visualization_msgs::msg::Marker::ADD;
  angular_v_marker_.points.clear();
  angular_v_marker_.points.emplace_back(position_marker_.pose.position);
  arrow_end = position_marker_.pose.position;
  arrow_end.z += state(7) / M_PI;
  angular_v_marker_.points.emplace_back(arrow_end);

  armor_marker_.action = visualization_msgs::msg::Marker::ADD;
  armor_marker_.scale.y = 0.135;  // small armor
  bool is_current_pair = true;
  size_t a_n = 4; // none for num
  geometry_msgs::msg::Point p_a;
  double r = 0;
  for (size_t i = 0; i < a_n; i++) {
    double tmp_yaw = yaw + i * (2 * M_PI / a_n);
    // Only 4 armors has 2 radius and height
    if (a_n == 4) {
      r = is_current_pair ? r1 : r2;
      p_a.z = za + (is_current_pair ? 0 : dz);
      is_current_pair = !is_current_pair;
    } else {
      r = r1;
      p_a.z = za;
    }
    p_a.x = xc - r * cos(tmp_yaw);
    p_a.y = yc - r * sin(tmp_yaw);

    armor_marker_.id = i;
    armor_marker_.pose.position = p_a;
    tf2::Quaternion q;
    q.setRPY(0, 0.26, tmp_yaw);
    armor_marker_.pose.orientation = tf2::toMsg(q);
    marker_array.markers.emplace_back(armor_marker_);
  }

  marker_array.markers.emplace_back(position_marker_);
  marker_array.markers.emplace_back(linear_v_marker_);
  marker_array.markers.emplace_back(angular_v_marker_);
  real_marker_pub_->publish(marker_array);
}

// 计算deque中元素的均方误差
std::vector<double> ArmorCreaterNode::meanSquareError(const std::deque<auto_aim_interfaces::msg::Target::SharedPtr>& actual, const std::deque<auto_aim_interfaces::msg::Target::SharedPtr>& predicted) 
{
  size_t minSize = std::min(actual.size(), predicted.size());

  std::vector<double> mse(2, 0.0);
  double positionMSE = 0.0;
  double yawMSE = 0.0;

  for (size_t i = 0; i < minSize; i++) 
  {
    double diffX = actual[i]->position.x - predicted[i]->position.x;
    double diffY = actual[i]->position.y - predicted[i]->position.y;
    double diffZ = actual[i]->position.z - predicted[i]->position.z;
    double diffPosSquared = diffX * diffX + diffY * diffY + diffZ * diffZ;
    positionMSE += diffPosSquared;

    double diffYaw = actual[i]->yaw - predicted[i]->yaw;
    double diffYawSquared = diffYaw * diffYaw;
    yawMSE += diffYawSquared;
  
  }

  mse[0] =positionMSE / static_cast<double>(minSize);
  mse[1] =yawMSE / static_cast<double>(minSize);
  return mse;
}


} // namespace rm_auto_aim

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rm_auto_aim::ArmorCreaterNode)
