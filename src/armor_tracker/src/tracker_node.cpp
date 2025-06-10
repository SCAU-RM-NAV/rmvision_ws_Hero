// Copyright 2022 Chen Jun
#include "armor_tracker/tracker_node.hpp"


// STD
#include <memory>
#include <vector>

namespace rm_auto_aim
{
ArmorTrackerNode::ArmorTrackerNode(const rclcpp::NodeOptions & options)
: Node("armor_tracker", options)
{
  RCLCPP_INFO(this->get_logger(), "Starting TrackerNode!");

  // Maximum allowable armor distance in the XOY plane
  max_armor_distance_ = this->declare_parameter("max_armor_distance", 10.0);

  // Tracker
  double max_match_distance = this->declare_parameter("tracker.max_match_distance", 0.15);
  double max_match_yaw_diff = this->declare_parameter("tracker.max_match_yaw_diff", 1.0);
  double yaw_filter_alpha = this->declare_parameter("tracker.yaw_filter_alpha", 0.8);
  choose_armor_shoot_yaw_tole = this->declare_parameter("choose_armor_shoot_yaw_tole", 0.95);
  spin_tracker_ = std::make_unique<Tracker>(max_match_distance, max_match_yaw_diff, yaw_filter_alpha);
  spin_tracker_->tracking_thres = this->declare_parameter("tracker.tracking_thres", 5);
  RCLCPP_INFO(this->get_logger(), "ready init CommonTracker!");
  double common_match_distance = this->declare_parameter("tracker.common_match_distance", 0.30);
  com_tracker_ = std::make_unique<CommonTracker>(common_match_distance);
  RCLCPP_INFO(this->get_logger(), "init CommonTracker!");
  lost_time_thres_ = this->declare_parameter("tracker.lost_time_thres", 0.3);

  // Solve trajectory
  solve_trajectory_ = std::make_unique<SolveTrajectory>(this->now());

  auto bullet_type = this->declare_parameter("trajectory.bullet_type", 0);  //自身机器人类型 0-步兵 1-英雄
  solve_trajectory_->st.bullet_type = bullet_type==0 ? BULLET_17 : BULLET_42;
  solve_trajectory_->st.current_v = this->declare_parameter("trajectory.current_v", 30.0);
  solve_trajectory_->st.bias_time = this->declare_parameter("trajectory.bias_time", 200);
  solve_trajectory_->st.bias_time_com = this->declare_parameter("trajectory.bias_time_com", 200);
  solve_trajectory_->st.s_bias = this->declare_parameter("trajectory.s_bias", 0.2);
  solve_trajectory_->st.z_bias = this->declare_parameter("trajectory.z_bias", 0.0);
  solve_trajectory_->st.pitch_bias = this->declare_parameter("trajectory.pitch_bias", 0.0);
  solve_trajectory_->shoot_yaw = this->declare_parameter("shoot_yaw", 20.0);
  solve_trajectory_->shoot_yaw_ratio = this->declare_parameter("shoot_yaw_ratio", 20.0);

  shoot_yaw_tole_ = this->declare_parameter("shoot_yaw_tole", 2.0);


  // init spin EKF
  // xa = x_armor, xc = x_robot_center
  // state: xc, v_xc, yc, v_yc, za, v_za, yaw, v_yaw, r
  // measurement: pit, yaw, dis, yaw_armor
  // f - Process function
  auto f = [this](const Eigen::VectorXd & x) {
    Eigen::VectorXd x_new = x;
    x_new(0) += x(1) * dt_;
    x_new(2) += x(3) * dt_;
    x_new(4) += x(5) * dt_;
    x_new(6) += x(7) * dt_;
    return x_new;
  };
  // J_f - Jacobian of process function
  auto j_f = [this](const Eigen::VectorXd &) {
    Eigen::MatrixXd f(9, 9);
    // clang-format off
    f <<  1,   dt_, 0,   0,   0,   0,   0,   0,   0,
          0,   1,   0,   0,   0,   0,   0,   0,   0,
          0,   0,   1,   dt_, 0,   0,   0,   0,   0,
          0,   0,   0,   1,   0,   0,   0,   0,   0,
          0,   0,   0,   0,   1,   dt_, 0,   0,   0,
          0,   0,   0,   0,   0,   1,   0,   0,   0,
          0,   0,   0,   0,   0,   0,   1,   dt_, 0,
          0,   0,   0,   0,   0,   0,   0,   1,   0,
          0,   0,   0,   0,   0,   0,   0,   0,   1;
    // clang-format on
    return f;
  };
  // h - Observation function
  auto h = [&](const Eigen::VectorXd & x) {
    Eigen::VectorXd z(4);
    double xc = x(0), yc = x(2), yaw = x(6), r = x(8);

    // 转换为球面坐标系
    double xyz[3];
    xyz[0] = xc - r * cos(yaw);  // xa
    xyz[1] = yc - r * sin(yaw);  // ya
    xyz[2] = x(4);               // za

    double pyd[3];
    xyz2pyd(xyz, pyd);

    z(0) = pyd[0];    // pitch 
    z(1) = pyd[1];    // yaw
    z(2) = pyd[2];    // dis
    z(3) = x(6);      // yaw_armor
    return z;
  };
  // J_h - Jacobian of observation function
  auto j_h = [](const Eigen::VectorXd & x) {
    // 使用ceres库自动求导
    ceres::Jet<double, 9> x_jet[9];
    for (int i = 0; i < 9; i++) {
        x_jet[i].a = x[i];
        x_jet[i].v[i] = 1;
    }

    ceres::Jet<double, 9> y_jet[4];
    SpinMeasureFunc func; //整车状态估计观测方程
    func(x_jet, y_jet);

    // 使用Eigen的Matrix类型来表示矩阵
    Eigen::MatrixXd h(4, 9);
    for (int i = 0; i < 4; i++) {
        // Yp[i] = Yp_auto_jet[i].a;
        h.block(i, 0, 1, 9) = y_jet[i].v.transpose();
    }

    return h;
  };
  // update_Q - process noise covariance matrix
  spin_s2qxyz_ = declare_parameter("spin_ekf.sigma2_q_xyz", 20.0);
  spin_s2qyaw_ = declare_parameter("spin_ekf.sigma2_q_yaw", 100.0);
  spin_s2qr_ = declare_parameter("spin_ekf.sigma2_q_r", 800.0);
  auto u_q = [this]() {
    Eigen::MatrixXd q(9, 9);
    double t = dt_, x = spin_s2qxyz_, y = spin_s2qyaw_, r = spin_s2qr_;
    double q_x_x = pow(t, 4) / 4 * x, q_x_vx = pow(t, 3) / 2 * x, q_vx_vx = pow(t, 2) * x;
    double q_y_y = pow(t, 4) / 4 * y, q_y_vy = pow(t, 3) / 2 * y, q_vy_vy = pow(t, 2) * y;
    double q_r = pow(t, 4) / 4 * r;
    // clang-format off
    //    xc      v_xc    yc      v_yc    za      v_za    yaw     v_yaw   r
    q <<  q_x_x,  q_x_vx, 0,      0,      0,      0,      0,      0,      0,
          q_x_vx, q_vx_vx,0,      0,      0,      0,      0,      0,      0,
          0,      0,      q_x_x,  q_x_vx, 0,      0,      0,      0,      0,
          0,      0,      q_x_vx, q_vx_vx,0,      0,      0,      0,      0,
          0,      0,      0,      0,      q_x_x,  q_x_vx, 0,      0,      0,
          0,      0,      0,      0,      q_x_vx, q_vx_vx,0,      0,      0,
          0,      0,      0,      0,      0,      0,      q_y_y,  q_y_vy, 0,
          0,      0,      0,      0,      0,      0,      q_y_vy, q_vy_vy,0,
          0,      0,      0,      0,      0,      0,      0,      0,      q_r;
    // clang-format on
    return q;
  };
  // update_R - measurement noise covariance matrix
  spin_r_py = declare_parameter("spin_ekf.r_py", 0.05);  // 球坐标系 pit与yaw观测方差
  spin_r_dis_factor = declare_parameter("spin_ekf.r_dis_factor", 0.1); // 球坐标系距离观测方差系数，方差 = r_dis_factor * dis
  // r_xyz_factor = declare_parameter("spin_ekf.r_xyz_factor", 0.05);  // 已废除
  spin_r_yaw = declare_parameter("spin_ekf.r_yaw", 0.02);
  auto u_r = [this](const Eigen::VectorXd & z) {
    Eigen::DiagonalMatrix<double, 4> r;
    r.diagonal() << spin_r_py, spin_r_py, abs(spin_r_dis_factor * z[2]), spin_r_yaw;
    return r;
  };
  // P - error estimate covariance matrix
  Eigen::DiagonalMatrix<double, 9> p0;
  p0.setIdentity();
  spin_tracker_->ekf = ExtendedKalmanFilter{f, h, j_f, j_h, u_q, u_r, p0};


  // init common ekf
  // state: xc, v_xc, yc, v_yc, za, v_za
  // measurement: pit, yaw, dis, yaw_armor
  // f_com - Process function
  auto f_com = [this](const Eigen::VectorXd & x) {
    Eigen::VectorXd x_new = x;
    x_new(0) += x(1) * dt_;
    x_new(2) += x(3) * dt_;
    x_new(4) += x(5) * dt_;
    return x_new;
  };
  // J_f - Jacobian of process function
  auto j_f_com = [this](const Eigen::VectorXd &) {
    Eigen::MatrixXd f(6, 6);
    // clang-format off
    f <<  1,   dt_, 0,   0,   0,   0,
          0,   1,   0,   0,   0,   0,
          0,   0,   1,   dt_, 0,   0,
          0,   0,   0,   1,   0,   0,
          0,   0,   0,   0,   1,   dt_,
          0,   0,   0,   0,   0,   1;
    // clang-format on
    return f;
  };

  // h - Observation function
  auto h_com = [&](const Eigen::VectorXd & x) {
    Eigen::VectorXd z(3);

    // 转换为球面坐标系
    double xyz[3];
    xyz[0] = x(0);  // xa
    xyz[1] = x(2);  // ya
    xyz[2] = x(4);  // za

    double pyd[3];
    xyz2pyd(xyz, pyd);

    z(0) = pyd[0];    // pitch 
    z(1) = pyd[1];    // yaw
    z(2) = pyd[2];    // dis

    return z;
  };

  // J_h - Jacobian of observation function
  auto j_h_com = [](const Eigen::VectorXd & x) {
    // 使用ceres库自动求导
    ceres::Jet<double, 6> x_jet[6];
    for (int i = 0; i < 6; i++) {
        x_jet[i].a = x[i];
        x_jet[i].v[i] = 1;
    }

    ceres::Jet<double, 6> y_jet[3];
    CommonMeasureFunc func; //普通跟踪器观测方程
    func(x_jet, y_jet);

    // 使用Eigen的Matrix类型来表示矩阵
    Eigen::MatrixXd h(3, 6);
    for (int i = 0; i < 3; i++) {
        // Yp[i] = Yp_auto_jet[i].a;
        h.block(i, 0, 1, 6) = y_jet[i].v.transpose();
    }

    return h;
  };
  // update_Q - process noise covariance matrix
  com_s2qxyz_ = declare_parameter("com_ekf.sigma2_q_xyz", 20.0);
  auto u_q_com = [this]() {
    Eigen::MatrixXd q(6, 6);
    double t = dt_, x = spin_s2qxyz_;
    double q_x_x = pow(t, 4) / 4 * x, q_x_vx = pow(t, 3) / 2 * x, q_vx_vx = pow(t, 2) * x;
    // clang-format off
    //    xc      v_xc    yc      v_yc    za      v_za    
    q <<  q_x_x,  q_x_vx, 0,      0,      0,      0,      
          q_x_vx, q_vx_vx,0,      0,      0,      0,      
          0,      0,      q_x_x,  q_x_vx, 0,      0,      
          0,      0,      q_x_vx, q_vx_vx,0,      0,      
          0,      0,      0,      0,      q_x_x,  q_x_vx, 
          0,      0,      0,      0,      q_x_vx, q_vx_vx;
    // clang-format on
    return q;
  };

  // update_R - measurement noise covariance matrix
  com_r_py = declare_parameter("com_ekf.r_py", 0.05);  // 球坐标系 pit与yaw观测方差
  com_r_dis_factor = declare_parameter("com_ekf.r_dis_factor", 0.1); // 球坐标系距离观测方差系数，方差 = r_dis_factor * dis
  auto u_r_com = [this](const Eigen::VectorXd & z) {
    Eigen::DiagonalMatrix<double, 3> r;
    r.diagonal() << com_r_py, com_r_py, abs(com_r_dis_factor * z[2]);
    return r;
  };
  // P - error estimate covariance matrix
  Eigen::DiagonalMatrix<double, 6> p0_com;
  p0_com.setIdentity();
  com_tracker_->ekf = ExtendedKalmanFilter{f_com, h_com, j_f_com, j_h_com, u_q_com, u_r_com, p0_com};


  // Reset tracker service
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  reset_tracker_srv_ = this->create_service<std_srvs::srv::Trigger>(
    "/tracker/reset", [this](
                        const std_srvs::srv::Trigger::Request::SharedPtr,
                        std_srvs::srv::Trigger::Response::SharedPtr response) {
      spin_tracker_->tracker_state = Tracker::LOST;
      response->success = true;
      RCLCPP_INFO(this->get_logger(), "Tracker reset!");
      return;
    });

  // Subscriber with tf2 message_filter
  // tf2 relevant
  tf2_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
  // Create the timer interface before call to waitForTransform,
  // to avoid a tf2_ros::CreateTimerInterfaceException exception
  auto timer_interface = std::make_shared<tf2_ros::CreateTimerROS>(
    this->get_node_base_interface(), this->get_node_timers_interface());
  tf2_buffer_->setCreateTimerInterface(timer_interface);
  tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
  // subscriber and filter
  // armors_sub_ = this->create_subscription<auto_aim_interfaces::msg::Armors>(
  //     "/detector/armors", rclcpp::QoS(10).best_effort(), std::bind(&ArmorTrackerNode::armorsCallback, this, std::placeholders::_1));
  armors_sub_.subscribe(this, "/detector/armors", rmw_qos_profile_sensor_data);

  tracker_gimbal_fdb_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalFdb>(
	"gimbal_fdb", rclcpp::SensorDataQoS(),
	std::bind(&ArmorTrackerNode::gimbalFdbCallback, this, std::placeholders::_1));

  target_frame_ = this->declare_parameter("target_frame", "world");
  tf2_filter_ = std::make_shared<tf2_filter>(
    armors_sub_, *tf2_buffer_, target_frame_, 500, this->get_node_logging_interface(),
    this->get_node_clock_interface(), std::chrono::duration<int>(1));
  // Register a callback with tf2_ros::MessageFilter to be called when transforms are available
  tf2_filter_->registerCallback(&ArmorTrackerNode::armorsCallback, this);

  // Measurement publisher (for debug usage)
  info_pub_ = this->create_publisher<auto_aim_interfaces::msg::TrackerInfo>("/tracker/info", 10);

  // Publisher
  target_pub_ = this->create_publisher<auto_aim_interfaces::msg::Target>(
    "/tracker/target", rclcpp::SensorDataQoS());

  // Visualization Marker Publisher
  // See http://wiki.ros.org/rviz/DisplayTypes/Marker
  position_marker_.ns = "position";
  position_marker_.type = visualization_msgs::msg::Marker::SPHERE;
  position_marker_.scale.x = position_marker_.scale.y = position_marker_.scale.z = 0.1;
  position_marker_.color.a = 1.0;
  position_marker_.color.g = 1.0;
  linear_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
  linear_v_marker_.ns = "linear_v";
  linear_v_marker_.scale.x = 0.03;
  linear_v_marker_.scale.y = 0.05;
  linear_v_marker_.color.a = 1.0;
  linear_v_marker_.color.r = 1.0;
  linear_v_marker_.color.g = 1.0;
  armor_position_marker_.ns = "armor_position";
  armor_position_marker_.type = visualization_msgs::msg::Marker::SPHERE;
  armor_position_marker_.scale.x = armor_position_marker_.scale.y = armor_position_marker_.scale.z = 0.1;
  armor_position_marker_.color.a = 1.0;
  armor_position_marker_.color.b = 1.0;
  armor_position_marker_.color.r = 1.0;
  armor_linear_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
  armor_linear_v_marker_.ns = "armor_linear_v";
  armor_linear_v_marker_.scale.x = 0.03;
  armor_linear_v_marker_.scale.y = 0.05;
  armor_linear_v_marker_.color.a = 1.0;
  armor_linear_v_marker_.color.b = 1.0;
  armor_linear_v_marker_.color.r = 1.0;
  angular_v_marker_.type = visualization_msgs::msg::Marker::ARROW;
  angular_v_marker_.ns = "angular_v";
  angular_v_marker_.scale.x = 0.03;
  angular_v_marker_.scale.y = 0.05;
  angular_v_marker_.color.a = 1.0;
  angular_v_marker_.color.b = 1.0;
  angular_v_marker_.color.g = 1.0;
  armor_marker_.ns = "armors";
  armor_marker_.type = visualization_msgs::msg::Marker::CUBE;
  armor_marker_.scale.x = 0.03;
  armor_marker_.scale.z = 0.125;
  armor_marker_.color.a = 1.0;
  armor_marker_.color.r = 1.0;
  aim_marker_.ns = "aim";
  aim_marker_.type = visualization_msgs::msg::Marker::SPHERE;
  aim_marker_.scale.x = aim_marker_.scale.y = aim_marker_.scale.z = 0.1;
  aim_marker_.color.a = 1.0;
  aim_marker_.color.g = 1.0;
  marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("/tracker/marker", 10);

  // send_pitch_yaw_pub_ = this->create_publisher<auto_aim_interfaces::msg::PitchYaw>(
  //   "/tracker/send_pitch_yaw", rclcpp::SensorDataQoS());
  aim_marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("/tracker/aim_marker", 10);
  gimbal_control_pub_ = this->create_publisher<auto_aim_interfaces::msg::GimbalCtrl>(
    "/gimbal_ctrl", rclcpp::SensorDataQoS());

}

void ArmorTrackerNode::armorsCallback(const auto_aim_interfaces::msg::Armors::SharedPtr armors_msg)
{
  // calculate latency from read camera to now
  auto recv_time = this->now();
  auto latency = (recv_time - armors_msg->header.stamp).seconds() * 1000;
  RCLCPP_DEBUG_STREAM(this->get_logger(), "Received armors, latency: " << latency << "ms");

  // dynamic update solve trajectory params
  solve_trajectory_->st.bias_time = get_parameter("trajectory.bias_time").as_int();
  solve_trajectory_->st.bias_time_com = get_parameter("trajectory.bias_time_com").as_int();
  solve_trajectory_->st.s_bias = get_parameter("trajectory.s_bias").as_double();
  solve_trajectory_->st.z_bias = get_parameter("trajectory.z_bias").as_double();
  solve_trajectory_->st.s_bias = get_parameter("trajectory.s_bias").as_double();
  solve_trajectory_->shoot_yaw = get_parameter("shoot_yaw").as_double();
  solve_trajectory_->shoot_yaw_ratio = get_parameter("shoot_yaw_ratio").as_double();

  // Tranform armor position from image frame to world coordinate
  for (auto & armor : armors_msg->armors) {
    std::istringstream iss(armor.number);
    int number_int;
    iss >> number_int;
    geometry_msgs::msg::PoseStamped ps;
    ps.header = armors_msg->header;
    ps.pose = armor.pose;
    try {
      armor.pose = tf2_buffer_->transform(ps, target_frame_).pose;
    } catch (const tf2::ExtrapolationException & ex) {
      RCLCPP_ERROR(get_logger(), "Error while transforming %s", ex.what());
      return;
    }
    if (number_int == priority_number) {
      have_priroity_number = true;
    }
  }

  // Filter abnormal armors
  armors_msg->armors.erase(
    std::remove_if(
      armors_msg->armors.begin(), armors_msg->armors.end(),
      [this](const auto_aim_interfaces::msg::Armor & armor) {
        return abs(armor.pose.position.z) > 2.0 ||
               Eigen::Vector2d(armor.pose.position.x, armor.pose.position.y).norm() >
                 max_armor_distance_;
      }),
    armors_msg->armors.end());

  // Init message
  auto_aim_interfaces::msg::TrackerInfo info_msg;
  auto_aim_interfaces::msg::Target target_msg;
  auto_aim_interfaces::msg::GimbalCtrl gimbal_ctrl_msg;
  rclcpp::Time time = armors_msg->header.stamp;
  target_msg.header.stamp = time;
  target_msg.header.frame_id = target_frame_;

  if (have_priroity_number) {
    spin_tracker_->tracker_priority_number(armors_msg,priority_number);
  }

  // Update tracker
  if (spin_tracker_->tracker_state == Tracker::LOST) {
    spin_tracker_->init(armors_msg);
    // com_tracker_->init(spin_tracker_->tracked_armor);
    if (!armors_msg->armors.empty()) {
      com_tracker_->update(spin_tracker_->tracked_armor);  //初始化普通跟踪器
    }
    target_msg.tracking = false;
    gimbal_ctrl_msg.aim_flag = 0;
    gimbal_ctrl_msg.ist_flag = 0;
  } else {
    dt_ = (time - last_time_).seconds();
    spin_tracker_->lost_thres = static_cast<int>(lost_time_thres_ / dt_);
    spin_tracker_->update(armors_msg);
    com_tracker_->update(spin_tracker_->tracked_armor);

    // Publish Info
    // 已改为球面坐标系，xyz分别对应pit、yaw、dis
    info_msg.position_diff = spin_tracker_->info_position_diff;
    info_msg.yaw_diff = spin_tracker_->info_yaw_diff;
    info_msg.position.x = spin_tracker_->measurement(0);
    info_msg.position.y = spin_tracker_->measurement(1);
    info_msg.position.z = spin_tracker_->measurement(2);
    info_msg.yaw = spin_tracker_->measurement(3);

    // std::cout << "ready state_covariance";

    Eigen::MatrixXd state_covariance = spin_tracker_->ekf.getCovariance();
    int n = state_covariance.rows(); //获取对角阵的大小
    // float state_covariance_diag[n+1];

    for (int i = 0; i < n; ++i) {
      info_msg.sigma2_state.push_back(state_covariance(i,i));
      // info_msg.sigma2_state[i] = state_covariance(n,n); //将对角元素放入数组
      // state_covariance_diag[n] = state_covariance(n,n); //将对角元素放入数组
      // std::cout << state_covariance(i,i) << " ";
    }
    // info_msg.sigma2_state = state_covariance_diag;
    info_pub_->publish(info_msg);

    if (spin_tracker_->tracker_state == Tracker::DETECTING) {
      target_msg.tracking = false;

      gimbal_ctrl_msg.aim_flag = 0;
      gimbal_ctrl_msg.ist_flag = 0;
      gimbal_ctrl_msg.shoot_yaw_tole = 0;
    } else if (
      spin_tracker_->tracker_state == Tracker::TRACKING ||
      spin_tracker_->tracker_state == Tracker::TEMP_LOST) {
      target_msg.tracking = true;
      target_msg.armor_jump =  spin_tracker_->armor_jump;
      // Fill target message
      const auto & robo_state = spin_tracker_->target_state;
      const auto & armor_state = com_tracker_->target_state;
      target_msg.id = spin_tracker_->tracked_id;
      target_msg.armors_num = static_cast<int>(spin_tracker_->tracked_armors_num);
      target_msg.position.x = robo_state(0);
      target_msg.velocity.x = robo_state(1);
      target_msg.position.y = robo_state(2);
      target_msg.velocity.y = robo_state(3);
      target_msg.position.z = robo_state(4);
      target_msg.velocity.z = robo_state(5);
      target_msg.armor_position.x = armor_state(0);
      target_msg.armor_velocity.x = armor_state(1);
      target_msg.armor_position.y = armor_state(2);
      target_msg.armor_velocity.y = armor_state(3);
      target_msg.armor_position.z = armor_state(4);
      target_msg.armor_velocity.z = armor_state(5);
      target_msg.yaw = robo_state(6);
      target_msg.v_yaw = robo_state(7);
      target_msg.radius_1 = robo_state(8);
      target_msg.radius_2 = spin_tracker_->another_r;
      target_msg.dz = spin_tracker_->dz;

      gimbal_ctrl_msg.aim_flag = 1;
      gimbal_ctrl_msg.ist_flag = 1;

      // calculate latency from read camera to now
      auto now_time = this->now();
      auto calculate_delay = (now_time - target_msg.header.stamp).seconds() * 1000;
      RCLCPP_DEBUG_STREAM(this->get_logger(), "calculate delay : " << calculate_delay << "ms");
      solve_trajectory_->autoSolveTrajectory(target_msg, calculate_delay);
      // std::cout<<"solve_trajectory_.pitch,solve_trajectory_.yaw:  "<<solve_trajectory_.pitch<<","<<solve_trajectory_.yaw<<std::endl;

      float yaw = solve_trajectory_->yaw * 57.29578;
      gimbal_ctrl_msg.yaw = yaw > 0 ? yaw : yaw + 360;
      gimbal_ctrl_msg.pit = solve_trajectory_->pitch * 57.29578;
      // gimbal_control_pub_->publish(gimbal_ctrl_msg);
      auto p = target_msg.position;
      Eigen::Vector3d target_p(p.x,p.y,p.z);

      // std::cout<<"-------------"<<solve_trajectory_->choose_armor_shoot_yaw<<std::endl;
      

      gimbal_ctrl_msg.fire_flag = 1;
       //设置火控标志位，1为发弹，0不发弹
      if(solve_trajectory_->fire_flag==true){
        gimbal_ctrl_msg.fire_flag = 1;
      } else {
        gimbal_ctrl_msg.fire_flag = 0;
      }

         //gimbal_ctrl_msg.fire_flag = 0;
      if(!solve_trajectory_->is_spin){
        gimbal_ctrl_msg.shoot_yaw_tole = 2.0;
        // std::cout<<"---------------------------"<<gimbal_ctrl_msg.shoot_yaw_tole<<std::endl;
      }else{
        if (solve_trajectory_->control_diff_mode ==  1) //平移小陀螺y方向速度小，放宽角度，适合追击
        {
          gimbal_ctrl_msg.shoot_yaw_tole = 0.5;
        }else if(solve_trajectory_->control_diff_mode == 2) //近距离或者是英雄和平步，放宽击打角度
        {
          gimbal_ctrl_msg.shoot_yaw_tole = 1.0;
        }
        else if(solve_trajectory_->control_diff_mode == 3)//普通高速小陀螺，放宽发枪角度，追求速度
        {
          gimbal_ctrl_msg.shoot_yaw_tole = 0.7;
        }
        else if(solve_trajectory_->control_diff_mode == 4) //平移小陀螺y方向速度较大，角度较为严格
        {
          gimbal_ctrl_msg.shoot_yaw_tole = 1.0;
        }else{                                                  //普通低速小陀螺，严格发枪角度，追求命中率
          gimbal_ctrl_msg.shoot_yaw_tole = 1.0;
        }
      gimbal_ctrl_msg.shoot_yaw_tole = 0.3; //比赛用0.5
      // gimbal_ctrl_msg.shoot_yaw_tole = 0.0; //比赛用0.5
      }
      // std::cout<<"shoot_yaw_tole:-------"<<gimbal_ctrl_msg.shoot_yaw_tole<<std::endl;
      // }
      publishMarkers(target_msg);
    }
  }

  last_time_ = time;
  target_pub_->publish(target_msg);


  gimbal_control_pub_->publish(gimbal_ctrl_msg);

}

void ArmorTrackerNode::gimbalFdbCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg)
{
  priority_number = msg->priority_number;
}

void ArmorTrackerNode::publishMarkers(const auto_aim_interfaces::msg::Target & target_msg)
{
  position_marker_.header = target_msg.header;
  linear_v_marker_.header = target_msg.header;
  armor_position_marker_.header = target_msg.header;
  armor_linear_v_marker_.header = target_msg.header;
  angular_v_marker_.header = target_msg.header;
  armor_marker_.header = target_msg.header;
  aim_marker_.header.stamp = this->get_clock()->now();
  aim_marker_.header.frame_id=target_msg.header.frame_id;

  aim_marker_.action = visualization_msgs::msg::Marker::ADD;
  aim_marker_.pose.position.x = solve_trajectory_->aim_x;
  aim_marker_.pose.position.y = solve_trajectory_->aim_y;
  aim_marker_.pose.position.z = solve_trajectory_->aim_z;

  visualization_msgs::msg::MarkerArray marker_array;
  if (target_msg.tracking) {
    double yaw = target_msg.yaw, r1 = target_msg.radius_1, r2 = target_msg.radius_2;
    double xc = target_msg.position.x, yc = target_msg.position.y, za = target_msg.position.z;
    double vx = target_msg.velocity.x, vy = target_msg.velocity.y, vz = target_msg.velocity.z;
    double dz = target_msg.dz;

    double xar = target_msg.armor_position.x, yar = target_msg.armor_position.y, zar = target_msg.armor_position.z;
    double vxar =  target_msg.armor_velocity.x, vyar = target_msg.armor_velocity.y, vzar = target_msg.armor_velocity.z;
    // 整车状态坐标可视化数据
    position_marker_.action = visualization_msgs::msg::Marker::ADD;
    position_marker_.pose.position.x = xc;
    position_marker_.pose.position.y = yc;
    position_marker_.pose.position.z = za + dz / 2;
    // 整车状态速度可视化数据
    linear_v_marker_.action = visualization_msgs::msg::Marker::ADD;
    linear_v_marker_.points.clear();
    linear_v_marker_.points.emplace_back(position_marker_.pose.position);
    geometry_msgs::msg::Point arrow_end = position_marker_.pose.position;
    arrow_end.x += vx;
    arrow_end.y += vy;
    arrow_end.z += vz;
    linear_v_marker_.points.emplace_back(arrow_end);
    // 普通跟踪坐标可视化
    armor_position_marker_.action = visualization_msgs::msg::Marker::ADD;
    armor_position_marker_.pose.position.x = xar;
    armor_position_marker_.pose.position.y = yar;
    armor_position_marker_.pose.position.z = zar;
    // 普通跟踪速度可视化
    armor_linear_v_marker_.action = visualization_msgs::msg::Marker::ADD;
    armor_linear_v_marker_.points.clear();
    armor_linear_v_marker_.points.emplace_back(armor_position_marker_.pose.position);
    arrow_end = armor_position_marker_.pose.position;
    arrow_end.x += vxar;
    arrow_end.y += vyar;
    arrow_end.z += vzar;
    armor_linear_v_marker_.points.emplace_back(arrow_end);
    // 整车状态角速度可视化数据
    angular_v_marker_.action = visualization_msgs::msg::Marker::ADD;
    angular_v_marker_.points.clear();
    angular_v_marker_.points.emplace_back(position_marker_.pose.position);
    arrow_end = position_marker_.pose.position;
    arrow_end.z += target_msg.v_yaw / M_PI;
    angular_v_marker_.points.emplace_back(arrow_end);
    // 整车状态装甲板可视化数据
    armor_marker_.action = visualization_msgs::msg::Marker::ADD;
    armor_marker_.scale.y = spin_tracker_->tracked_armor.type == "small" ? 0.135 : 0.23;
    bool is_current_pair = true;
    size_t a_n = target_msg.armors_num;
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
      q.setRPY(0, target_msg.id == "7" ? -0.26 : 0.26, tmp_yaw);
      armor_marker_.pose.orientation = tf2::toMsg(q);
      marker_array.markers.emplace_back(armor_marker_);
    }
  } else {
    position_marker_.action = visualization_msgs::msg::Marker::DELETE;
    linear_v_marker_.action = visualization_msgs::msg::Marker::DELETE;
    angular_v_marker_.action = visualization_msgs::msg::Marker::DELETE;
    armor_position_marker_.action = visualization_msgs::msg::Marker::DELETE;
    armor_linear_v_marker_.action = visualization_msgs::msg::Marker::DELETE;

    armor_marker_.action = visualization_msgs::msg::Marker::DELETE;
    marker_array.markers.emplace_back(armor_marker_);
  }

  marker_array.markers.emplace_back(position_marker_);
  marker_array.markers.emplace_back(linear_v_marker_);
  marker_array.markers.emplace_back(armor_position_marker_);
  marker_array.markers.emplace_back(armor_linear_v_marker_);
  marker_array.markers.emplace_back(angular_v_marker_);
  marker_pub_->publish(marker_array);
  aim_marker_pub_->publish(aim_marker_);
}

}  // namespace rm_auto_aim

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(rm_auto_aim::ArmorTrackerNode)
