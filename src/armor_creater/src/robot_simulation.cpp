#include "armor_creater/robot_simulation.hpp"

namespace rm_auto_aim
{

RobotSumulation::RobotSumulation(const CreaterParams & params) : params_(params)
{
  // 获取当前时间点
  auto currentTime = std::chrono::system_clock::now();
  std::chrono::duration<double> seconds = currentTime.time_since_epoch();
  first_time_ = seconds.count();

  // x.resize(9);
  x(0) = 3;   // x坐标赋值为1

}

Eigen::VectorXd RobotSumulation::move()
{
  // 获取当前时间点
  auto currentTime = std::chrono::system_clock::now();
  std::chrono::duration<double> seconds = currentTime.time_since_epoch();
  double dt = seconds.count() - time_ - first_time_;
  time_ = seconds.count() - first_time_;
  // std::cout << "dt : " << dt << std::endl;

  // params update
  x(8) = params_.robot_radius;

  // state update
  double y_omega = 2 * M_PI / params_.y_sin_cycle;
  double vel_omega = 2 * M_PI / params_.vel_sin_cycle;
  
  // position
  x(2) = params_.y_sin_range * sin(y_omega * time_);
  x(3) = params_.y_sin_range * cos(y_omega * time_) * y_omega;

  // armor yaw angle
  if (params_.mode == SIN)
  {
    x(6) = params_.vel_sin_range * sin(vel_omega * time_);
    x(7) = params_.vel_sin_range * cos(vel_omega * time_) * vel_omega;
  }else if (params_.mode == UNIFORM_VELOCITY)
  {
    // yaw_1 = yaw_0 + v_yaw * t
    x(6) += x(7) * dt;
    x(6) = (x(6) > 0.785 ) ? -x(6) : x(6);
    x(7) = params_.vel_sin_range; 
    // std::cout << "yaw : " << x(6) << std::endl;
  }
  return x;
}

Eigen::VectorXd RobotSumulation::measure()
{
  // Calculate predicted position of the current armor
  double xc = x(0), yc = x(2), zc = x(4);
  double yaw = x(6), r = x(8);

  // 已废弃， 改为按球面坐标系分别添加噪声
  // double xa = xc - r * cos(yaw) + generateRandomNumber(0, params_.pos_stddev);
  // double ya = yc - r * sin(yaw) + generateRandomNumber(0, params_.pos_stddev);
  // double za = zc + generateRandomNumber(0, params_.pos_stddev);

  double xa = xc - r * cos(yaw);
  double ya = yc - r * sin(yaw);
  double za = zc;
  double yawa = x(6) + generateRandomNumber(0, params_.yaw_stddev);

  // 转换为球面坐标系并添加高斯噪声
  double xyz[3] = {xa, ya, za};
  double pyd[3];
  xyz2pyd(xyz, pyd);
  pyd[0] += generateRandomNumber(0, params_.euler_stddev);
  pyd[1] += generateRandomNumber(0, params_.euler_stddev);
  pyd[2] += generateRandomNumber(0, params_.dis_stddev);
  pyd2xyz(pyd, xyz);

  // measure: xa, yz, za, yaw
  Eigen::VectorXd x_measure(4);
  x_measure << xyz[0], xyz[1], xyz[2], yawa;
  return x_measure;
}

}
