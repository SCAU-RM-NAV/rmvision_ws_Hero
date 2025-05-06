// Copyright 2022 Chen Jun

#ifndef ARMOR_PROCESSOR__KALMAN_FILTER_HPP_
#define ARMOR_PROCESSOR__KALMAN_FILTER_HPP_

#include <Eigen/Dense>
#include <functional>

#include <iostream>
#include <angles/angles.h>

#include <angles/angles.h>

namespace rm_auto_aim
{

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

class ExtendedKalmanFilter
{
public:
  ExtendedKalmanFilter() = default;

  using VecVecFunc = std::function<Eigen::VectorXd(const Eigen::VectorXd &)>;
  using VecMatFunc = std::function<Eigen::MatrixXd(const Eigen::VectorXd &)>;
  using VoidMatFunc = std::function<Eigen::MatrixXd()>;

  explicit ExtendedKalmanFilter(
    const VecVecFunc & f, const VecVecFunc & h, const VecMatFunc & j_f, const VecMatFunc & j_h,
    const VoidMatFunc & u_q, const VecMatFunc & u_r, const Eigen::MatrixXd & P0);

  // Set the initial state
  void setState(const Eigen::VectorXd & x0);

  // Compute a predicted state
  Eigen::MatrixXd predict();

  // Update the estimated state based on measurement
  Eigen::MatrixXd update(const Eigen::VectorXd & z);

  // Get Posteriori error  estimate covariance
  Eigen::MatrixXd getCovariance();

private:
  // Process nonlinear vector function
  VecVecFunc f;
  // Observation nonlinear vector function
  VecVecFunc h;
  // Jacobian of f()
  VecMatFunc jacobian_f;
  Eigen::MatrixXd F;
  // Jacobian of h()
  VecMatFunc jacobian_h;
  Eigen::MatrixXd H;
  // Process noise covariance matrix
  VoidMatFunc update_Q;
  Eigen::MatrixXd Q;
  // Measurement noise covariance matrix
  VecMatFunc update_R;
  Eigen::MatrixXd R;

  // Priori error estimate covariance matrix
  Eigen::MatrixXd P_pri;
  // Posteriori error estimate covariance matrix
  Eigen::MatrixXd P_post;

  // Kalman gain
  Eigen::MatrixXd K;

  // System dimensions
  int n;

  // N-size identity
  Eigen::MatrixXd I;

  // Priori state
  Eigen::VectorXd x_pri;
  // Posteriori state
  Eigen::VectorXd x_post;
};

}  // namespace rm_auto_aim

#endif  // ARMOR_PROCESSOR__KALMAN_FILTER_HPP_
