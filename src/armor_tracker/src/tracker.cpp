// Copyright 2022 Chen Jun

#include "armor_tracker/tracker.hpp"

#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/convert.h>

#include <rclcpp/logger.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

// STD
#include <cfloat>
#include <memory>
#include <string>

namespace rm_auto_aim
{
Tracker::Tracker(double max_match_distance, double max_match_yaw_diff, double yaw_filter_alpha)
: tracker_state(LOST),
  tracked_id(std::string("")),
  measurement(Eigen::VectorXd::Zero(4)),
  target_state(Eigen::VectorXd::Zero(9)),
  max_match_distance_(max_match_distance),
  max_match_yaw_diff_(max_match_yaw_diff),
  yaw_filter_alpha_(yaw_filter_alpha)
{
}

void Tracker::init(const Armors::SharedPtr & armors_msg)
{
  if (armors_msg->armors.empty()) {
    RCLCPP_WARN(rclcpp::get_logger("armor_tracker"), "armors empty");
    return;
  }

  // Simply choose the armor that is closest to image center
  double min_distance = DBL_MAX;
  tracked_armor = armors_msg->armors[0];

  //优先英雄策略
  Armor hero;
  bool have_hero = false;
  bool close_range_flight = 0;
  bool close_range_flight_hero = 0;
  for (const auto & armor : armors_msg->armors) {
  if (Eigen::Vector2d(armor.pose.position.x, armor.pose.position.y).norm() < min_distance) {
      min_distance = Eigen::Vector2d(armor.pose.position.x, armor.pose.position.y).norm();
      tracked_armor = armor;
	  }
  if (Eigen::Vector2d(armor.pose.position.x, armor.pose.position.y).norm() < 3.2) close_range_flight = true;
  if (Eigen::Vector2d(armor.pose.position.x, armor.pose.position.y).norm() < 3.2 && armor.number == "1") close_range_flight_hero = true;

  if(armor.number=="1"){
    have_hero=true;
    hero=armor;
  }
  }

  if((have_hero && !close_range_flight) || (have_hero && close_range_flight_hero)){
    tracked_armor=hero;
    // RCLCPP_INFO(rclcpp::get_logger("armor_tracker"), "HEROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
  }
  initEKF(tracked_armor);
  RCLCPP_DEBUG(rclcpp::get_logger("armor_tracker"), "Init EKF!");

  tracked_id = tracked_armor.number;
  tracker_state = DETECTING;

  updateArmorsNum(tracked_armor);
}

void Tracker::tracker_priority_number(const Armors::SharedPtr & armors_msg,int number) 
{
  Armor priority_number = tracked_armor;
  // priority_number = armors_msg->armors[0];
  for (const auto & armor : armors_msg->armors) {
    std::istringstream iss(armor.number);
    int number_int;
    iss >> number_int;  // 从字符串流中提取整数
    if (number_int == number) {
      tracked_armor = armor;
    }
  }
  if (priority_number.number != tracked_armor.number) {
    initEKF(tracked_armor);
    RCLCPP_DEBUG(rclcpp::get_logger("armor_tracker"), "成功初�?�化小陀螺ekf�?");

    // 更新追踪�?标id为当前�?�追�?装甲板的id
    tracked_id = tracked_armor.number;
    // 将状态机切换到�?�测中模式
    tracker_state = DETECTING;

    // 获取小车装甲板数�?
    updateArmorsNum(tracked_armor);
  }
}

void Tracker::update(const Armors::SharedPtr & armors_msg)
{
  // KF predict
  Eigen::VectorXd ekf_prediction = ekf.predict();
  RCLCPP_DEBUG(rclcpp::get_logger("armor_tracker"), "EKF predict");
  bool matched = false;
  armor_jump = false;
  // Use KF prediction as default target state if no matched armor is found
  target_state = ekf_prediction;

  if (!armors_msg->armors.empty()) {
    // Find the closest armor with the same id
    Armor same_id_armor;
    int same_id_armors_count = 0;
    auto predicted_position = getArmorPositionFromState(ekf_prediction);
    double min_position_diff = DBL_MAX;
    double yaw_diff = DBL_MAX;
    for (const auto & armor : armors_msg->armors) {
      // Only consider armors with the same id
      if (armor.number == tracked_id) {
        if(static_cast<int>(tracked_armors_num) == 2 && armor.type == "small")
        {
          continue;
        } 
        
        same_id_armor = armor;
        same_id_armors_count++;
        // Calculate the difference between the predicted position and the current armor position
        auto p = armor.pose.position;
        Eigen::Vector3d position_vec(p.x, p.y, p.z);
        double position_diff = (predicted_position - position_vec).norm();
        if (position_diff < min_position_diff) {
          // Find the closest armor
          min_position_diff = position_diff;
          yaw_diff = abs(orientationToYaw(armor.pose.orientation) - ekf_prediction(6));
          tracked_armor = armor;
        }
      }
      //有英雄 且英雄是近距离 直接打英雄
      if(armor.number == "1" && armor.type == "large" && tracked_id != "1" && Eigen::Vector2d(armor.pose.position.x, armor.pose.position.y).norm()<3.0){
        find_hero_count++;
        if(find_hero_count > 10){
          init(armors_msg);
          find_hero_count = 0;
        } 
      }
    }

    // Store tracker info
    info_position_diff = min_position_diff;
    info_yaw_diff = yaw_diff;

    // Check if the distance and yaw difference of closest armor are within the threshold
    if (min_position_diff < max_match_distance_ && yaw_diff < max_match_yaw_diff_) {
      // Matched armor found
      matched = true;
      auto p = tracked_armor.pose.position;
      // Update EKF
      double measured_yaw = orientationToYaw(tracked_armor.pose.orientation);
      //低通滤波
      measured_yaw = yaw_filter_alpha_ * measured_yaw + (1 - yaw_filter_alpha_) * last_measure_yaw_;
      last_measure_yaw_ = measured_yaw;

      // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "measured_yaw: " << measured_yaw << " target_state(6): " << target_state(6));
      // RCLCPP_INFO_STREAM(rclcpp::get_logger("armor_tracker"), "measured_yaw: " << measured_yaw << " ekf_prediction(6): " << ekf_prediction(6));
      double measure_pyd[3], xyz[3] = {p.x, p.y, p.z};
      xyz2pyd(xyz, measure_pyd);   // 将xyz坐标转换为球面坐标系

      measurement = Eigen::Vector4d(measure_pyd[0], measure_pyd[1], measure_pyd[2], measured_yaw);  // pit, yaw, dis, yaw_armor
      target_state = ekf.update(measurement);

      // 如果是前哨站 
      if(tracked_armor.number=="7"){
        target_state(8) = 0.2765;
      }
      jump_count_ = 0;
      RCLCPP_DEBUG(rclcpp::get_logger("armor_tracker"), "EKF update");

      // auto start = std::chrono::high_resolution_clock::now();
      // int iter_num = 0;
      // while (iter_num < 10)
      // {
      //   // ekf.predict();
      //   target_state = ekf.update(measurement);
      //   double pyd[3], xyz[3] = {target_state(0), target_state(2), target_state(4)};
      //   xyz2pyd(xyz, pyd);   // 将xyz坐标转换为球面坐标系
      //   double residual_pit =  measure_pyd[0] - pyd[0];
      //   double residual_yaw =  measure_pyd[1] - pyd[1];
      //   double residual_dis =  measure_pyd[2] - pyd[2];
      //   double residual_yaw_a =  measured_yaw - target_state[8];

      //   RCLCPP_INFO_STREAM(rclcpp::get_logger("armor_tracker"), "======= residual " 
      //     << " iter_num : " << iter_num << "================"
      //     << " residual_pit : " << residual_pit
      //     << " residual_yaw : " << residual_yaw
      //     << " residual_dis : " << residual_dis
      //     << " residual_yaw_a : " << residual_yaw_a << std::endl
      //   ); 
      //   iter_num++;
      // }

      // // 记录结束时间
      // auto end = std::chrono::high_resolution_clock::now();

      // // 计算时间差并输出结果
      // std::chrono::duration<double> duration = end - start;
      // std::cout << "Function execution time: " << duration.count() << " seconds" << std::endl;
      

    } else if (same_id_armors_count == 1 && yaw_diff > max_match_yaw_diff_) {
      // Matched armor not found, but there is only one armor with the same id
      // and yaw has jumped, take this case as the target is spinning and armor jumped
      handleArmorJump(same_id_armor);
      armor_jump = true;
      jump_count_++;
      if(jump_count_>35){
        //连续跳多次 就重置
        init(armors_msg);
        matched = true;
      }
    } else {
      jump_count_ = 0;
      // No matched armor found
      RCLCPP_WARN(rclcpp::get_logger("armor_tracker"), "No matched armor found!");
    }
  }

  // Prevent radius from spreading
  if (target_state(8) < 0.12) {
    target_state(8) = 0.12;
    ekf.setState(target_state);
  } else if (target_state(8) > 0.4) {
    target_state(8) = 0.4;
    ekf.setState(target_state);
  }

  // Tracking state machine
  if (tracker_state == DETECTING) {
    if (matched) {
      detect_count_++;
      if (detect_count_ > tracking_thres) {
        detect_count_ = 0;
        tracker_state = TRACKING;
      }
    } else {
      detect_count_ = 0;
      tracker_state = LOST;
    }
  } else if (tracker_state == TRACKING) {
    if (!matched) {
      tracker_state = TEMP_LOST;
      lost_count_++;
    }
  } else if (tracker_state == TEMP_LOST) {
    if (!matched) {
      lost_count_++;
      if (lost_count_ > 200) {
        lost_count_ = 0;
        tracker_state = LOST;
      }
    } else {
      tracker_state = TRACKING;
      lost_count_ = 0;
    }
  }
}

void Tracker::initEKF(const Armor & a)
{
  target_state = Eigen::VectorXd::Zero(9);
  double xa = a.pose.position.x;
  double ya = a.pose.position.y;
  double za = a.pose.position.z;
  last_yaw_ = 0;
  last_measure_yaw_ = 0;
  double yaw = orientationToYaw(a.pose.orientation);

  double r = 0.26;
    // // 如果是前哨站 
  if(a.number=="7"){
    r = 0.2765;
  }

  double xc = xa + r * cos(yaw);
  double yc = ya + r * sin(yaw);
  dz = 0, another_r = r;
  target_state << xc, 0, yc, 0, za, 0, yaw, 0, r;

  double pyd[3], xyz[3] = {xc, yc, za};
  xyz2pyd(xyz, pyd);   // 将xyz坐标转换为球面坐标系
  // std::cout << "=====================initEKF======================" << std::endl
  //           << " xc : " << xc
  //           << " yc : " << yc
  //           << " za : " << za
  //           << " pyd0 : " <<  pyd[0] 
  //           << " pyd1 :  " <<  pyd[1]
  //           << " pyd2 :  " <<  pyd[2] << std::endl;
  ekf.setState(target_state);
}

void Tracker::updateArmorsNum(const Armor & armor)
{
  last_tracked_armors_num = tracked_armors_num;
  tracked_armors_num = ArmorsNum::NORMAL_4;

  if (armor.type == "large" && (tracked_id == "3" || tracked_id == "4" || tracked_id == "5")) {
    tracked_armors_num = ArmorsNum::BALANCE_2;
  } else if (tracked_id == "7") {
    tracked_armors_num = ArmorsNum::OUTPOST_3;
  } else {
    tracked_armors_num = ArmorsNum::NORMAL_4;
    if(static_cast<int>(tracked_armors_num)==2){
      if(armor.type == "large"){
        tracked_armors_num = ArmorsNum::BALANCE_2;
      }
    }
  }
}

void Tracker::handleArmorJump(const Armor & current_armor)
{
  double min_yaw_diff = DBL_MAX;
  double min_yaw = DBL_MAX;
  auto temp_yaw = target_state(6);
  double yaw = angles::normalize_angle(orientationToYaw(current_armor.pose.orientation));
  for(int i = 1 ; i < static_cast<int>(tracked_armors_num); i++)
  {
    if(static_cast<int>(tracked_armors_num)==2){
      temp_yaw = angles::normalize_angle(temp_yaw + i * M_PI);
    }else if(static_cast<int>(tracked_armors_num)==3){
      temp_yaw = angles::normalize_angle(temp_yaw + i * 2.0 * M_PI / 3.0);
    }else{
      temp_yaw = angles::normalize_angle(temp_yaw + i * M_PI / 2);
    }

    if (abs(angles::normalize_angle(yaw - temp_yaw)) < min_yaw_diff){
      min_yaw_diff = abs(yaw - temp_yaw);
      min_yaw = temp_yaw;
    }
    RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "temp_yaw: " << temp_yaw << 
                        " i: " << i << " yaw: " << yaw << " min_yaw: " << min_yaw);
  }

  //需测试四轮切装甲板的yaw赋值
  if(static_cast<int>(tracked_armors_num)==2){
    // target state(6)yaw;
    target_state(6)= angles::normalize_angle((yaw + min_yaw)/2); 
  }
  else{
    target_state(6) = min_yaw;
  }
  // target_state(6) = angles::normalize_angle((yaw + min_yaw)/2);
  
  last_measure_yaw_ = min_yaw;
  updateArmorsNum(current_armor);
  // Only 4 armors has 2 radius and height
  if (tracked_armors_num == ArmorsNum::NORMAL_4) {
    dz = target_state(4) - current_armor.pose.position.z;
    target_state(4) = current_armor.pose.position.z;
    std::swap(target_state(8), another_r);
  }
  RCLCPP_WARN(rclcpp::get_logger("armor_tracker"), "Armor jump!");

  // If position difference is larger than max_match_distance_,
  // take this case as the ekf diverged, reset the state
  auto p = current_armor.pose.position;
  Eigen::Vector3d current_p(p.x, p.y, p.z);
  Eigen::Vector3d infer_p = getArmorPositionFromState(target_state);
  if ((current_p - infer_p).norm() > max_match_distance_) {
    double r = target_state(8);
    target_state(0) = p.x + r * cos(yaw);  // xc
    target_state(1) = 0;                   // vxc
    target_state(2) = p.y + r * sin(yaw);  // yc
    target_state(3) = 0;                   // vyc
    target_state(4) = p.z;                 // za
    target_state(5) = 0;                   // vza
    RCLCPP_ERROR(rclcpp::get_logger("armor_tracker"), "Reset State!");
  }
  double pyd[3], xyz[3] = {target_state(0), target_state(2), target_state(4)};
  xyz2pyd(xyz, pyd);   // 将xyz坐标转换为球面坐标系

  if(current_armor.number=="7"){//如果是前哨站 不用估计值 而用定值
      target_state(8) = 0.2765;
  }
  ekf.setState(target_state);
}

double Tracker::orientationToYaw(const geometry_msgs::msg::Quaternion & q)
{
  // Get armor yaw
  tf2::Quaternion tf_q;
  tf2::fromMsg(q, tf_q);
  double roll, pitch, yaw;
  tf2::Matrix3x3(tf_q).getRPY(roll, pitch, yaw);
  // Make yaw change continuous (-pi~pi to -inf~inf)
  yaw = target_state(6) + angles::shortest_angular_distance(target_state(6), yaw);  last_yaw_ = yaw;
  return yaw;
}

Eigen::Vector3d Tracker::getArmorPositionFromState(const Eigen::VectorXd & x)
{
  // Calculate predicted position of the current armor
  double xc = x(0), yc = x(2), za = x(4);
  double yaw = x(6), r = x(8);
  double xa = xc - r * cos(yaw);
  double ya = yc - r * sin(yaw);
  return Eigen::Vector3d(xa, ya, za);
}

}  // namespace rm_auto_aim
