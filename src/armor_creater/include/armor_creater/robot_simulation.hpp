#ifndef ARMOR_ROBOT_SIMULATION_NODE_HPP_
#define ARMOR_ROBOT_SIMULATION_NODE_HPP_

#include <rclcpp/rclcpp.hpp>

#include <Eigen/Dense>
#include <cmath>
#include <random>

namespace rm_auto_aim
{

class RobotSumulation
{
public:

  using VecVecFunc = std::function<Eigen::VectorXd(const Eigen::VectorXd &)>;

  enum Mode {
    SIN,
    UNIFORM_VELOCITY,
  } creater_mode;

  struct CreaterParams
  {
    Mode mode;

    double robot_radius;
    double vel_sin_range;   // 幅值
    double vel_sin_cycle;   // 周期
    double vel_sin_offset;  // 偏置

    double pos_stddev;  // 位置测量标准差
    double yaw_stddev;  // 姿态测量标准差

    double euler_stddev; // 机体球坐标系下角度标准差
    double dis_stddev; // 机体球坐标系下距离标准差

    // use for UNIFORM_VELOCITY mode
    double vel_x;
    double vel_y;
    double vel_z;

    // use for SIN mode
    // f(x) = sin_a * sin(sin_b * x)
    double y_sin_range;   // 幅值
    double y_sin_cycle;   // 周期
  };

  CreaterParams params_;
  
  RobotSumulation(const CreaterParams & params);

  /**
   * @brief 机器人运动状态更新
  */
  Eigen::VectorXd move();

  /**
   * @brief 测量机器人状态
  */
  Eigen::VectorXd measure();

  /**
   * @brief 生成服从正态分布的随机数
  */
  double generateRandomNumber(double mean, double stddev) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(mean, stddev);
    return distribution(gen);
  }

  template<class T>
  void xyz2pyd(T xyz[3], T pyd[3]) {
      /*
      * 工具函数：将 xyz 转化为 pitch、yaw、distance
      */
      pyd[0] = atan2(xyz[2], sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]));  // pitch
      pyd[1] = atan2(xyz[1], xyz[0]);  // yaw
      pyd[2] = sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2]);  // distance
  }

  template<class T>
  void pyd2xyz(T pyd[3], T xyz[3]) {
      /*
      * 工具函数：将 pitch、yaw、distance 转化为 xyz
      */
      xyz[0] = pyd[2] * cos(pyd[0]) * cos(pyd[1]);  // x
      xyz[1] = pyd[2] * cos(pyd[0]) * sin(pyd[1]);  // y
      xyz[2] = pyd[2] * sin(pyd[0]);  // z
  }


private:

  double time_;
  double first_time_;

  // Process nonlinear vector function
  // VecVecFunc f;
  // state: xc, v_xc, yc, v_yc, za, v_za, yaw, v_yaw, r
  Eigen::VectorXd x = Eigen::VectorXd::Zero(9);
};

}

#endif // ARMOR_ROBOT_SIMULATION_NODE_HPP_