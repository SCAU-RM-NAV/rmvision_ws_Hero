// Copyright 2022 Chen Jun

#include "armor_tracker/extended_kalman_filter.hpp"

namespace rm_auto_aim
{
ExtendedKalmanFilter::ExtendedKalmanFilter(
  const VecVecFunc & f, const VecVecFunc & h, const VecMatFunc & j_f, const VecMatFunc & j_h,
  const VoidMatFunc & u_q, const VecMatFunc & u_r, const Eigen::MatrixXd & P0)
: f(f),
  h(h),
  jacobian_f(j_f),
  jacobian_h(j_h),
  update_Q(u_q),
  update_R(u_r),
  P_post(P0),
  n(P0.rows()),
  I(Eigen::MatrixXd::Identity(n, n)),
  x_pri(n),
  x_post(n)
{
}

void ExtendedKalmanFilter::setState(const Eigen::VectorXd & x0) { 
  x_post = x0;
  double pyd[3], xyz[3] = {x_post[0], x_post[2], x_post[4]};
  xyz2pyd(xyz, pyd);   // 将xyz坐标转换为球面坐标系

             }

Eigen::MatrixXd ExtendedKalmanFilter::predict()
{
  F = jacobian_f(x_post), Q = update_Q();

  x_pri = f(x_post);
  P_pri = F * P_post * F.transpose() + Q;

  // handle the case when there will be no measurement before the next predict
  x_post = x_pri;
  P_post = P_pri;

  double pyd[3], xyz[3] = {x_post[0], x_post[2], x_post[4]};
  xyz2pyd(xyz, pyd);   // 将xyz坐标转换为球面坐标系

  return x_pri;
}


Eigen::MatrixXd ExtendedKalmanFilter::update(const Eigen::VectorXd & z)
{
  H = jacobian_h(x_pri), R = update_R(z);

  K = P_pri * H.transpose() * (H * P_pri * H.transpose() + R).inverse();
  Eigen::VectorXd residual = z - h(x_pri);
  residual(1) = angles::normalize_angle(residual(1));
  x_post = x_pri + K * residual;

  P_post = (I - K * H) * P_pri;

  double pyd[3], xyz[3] = {x_post[0], x_post[2], x_post[4]};
  xyz2pyd(xyz, pyd);   // 将xyz坐标转换为球面坐标系
  return x_post;
}

Eigen::MatrixXd ExtendedKalmanFilter::getCovariance() {return P_post;}

}  // namespace rm_auto_aim
