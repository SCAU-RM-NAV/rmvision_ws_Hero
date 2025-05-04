// Copyright 2022 Chen Jun

#include "armor_detector/pnp_solver.hpp"
#include <iostream>
#include <ceres/ceres.h>
#include <ceres/rotation.h>

#include <opencv2/calib3d.hpp>
#include <vector>

namespace rm_auto_aim
{
PnPSolver::PnPSolver(
  const std::array<double, 9> & camera_matrix, const std::vector<double> & dist_coeffs)
: camera_matrix_(cv::Mat(3, 3, CV_64F, const_cast<double *>(camera_matrix.data())).clone()),
  dist_coeffs_(cv::Mat(1, 5, CV_64F, const_cast<double *>(dist_coeffs.data())).clone())
{
  // Unit: m
  constexpr double small_half_y = SMALL_ARMOR_WIDTH / 2.0 ;
  constexpr double small_half_z = SMALL_ARMOR_HEIGHT / 2.0 ;
  constexpr double large_half_y = LARGE_ARMOR_WIDTH / 2.0 ;
  constexpr double large_half_z = LARGE_ARMOR_HEIGHT / 2.0 ;

  // Start from bottom left in clockwise order
  // Model coordinate: x forward, y left, z up
  small_armor_points_.emplace_back(cv::Point3f(0, small_half_y, -small_half_z));
  small_armor_points_.emplace_back(cv::Point3f(0, small_half_y, small_half_z));
  small_armor_points_.emplace_back(cv::Point3f(0, -small_half_y, small_half_z));
  small_armor_points_.emplace_back(cv::Point3f(0, -small_half_y, -small_half_z));

  large_armor_points_.emplace_back(cv::Point3f(0, large_half_y, -large_half_z));
  large_armor_points_.emplace_back(cv::Point3f(0, large_half_y, large_half_z));
  large_armor_points_.emplace_back(cv::Point3f(0, -large_half_y, large_half_z));
  large_armor_points_.emplace_back(cv::Point3f(0, -large_half_y, -large_half_z));


  //8个点
  //5   0   6
  //1       3
  //4   2   7
  // small_armor_points_.emplace_back(cv::Point3f(0, 0, small_half_z));
  // small_armor_points_.emplace_back(cv::Point3f(0, small_half_y, 0));
  // small_armor_points_.emplace_back(cv::Point3f(0, 0, -small_half_z));
  // small_armor_points_.emplace_back(cv::Point3f(0, -small_half_y, 0));

  // small_armor_points_.emplace_back(cv::Point3f(0, small_half_y, -small_half_z));
  // small_armor_points_.emplace_back(cv::Point3f(0, small_half_y, small_half_z));
  // small_armor_points_.emplace_back(cv::Point3f(0, -small_half_y, small_half_z));
  // small_armor_points_.emplace_back(cv::Point3f(0, -small_half_y, -small_half_z));

  // large_armor_points_.emplace_back(cv::Point3f(0, 0, large_half_z));
  // large_armor_points_.emplace_back(cv::Point3f(0, large_half_y, 0));
  // large_armor_points_.emplace_back(cv::Point3f(0, 0, -large_half_z));
  // large_armor_points_.emplace_back(cv::Point3f(0, -large_half_y, 0));

  // large_armor_points_.emplace_back(cv::Point3f(0, large_half_y, -large_half_z));
  // large_armor_points_.emplace_back(cv::Point3f(0, large_half_y, large_half_z));
  // large_armor_points_.emplace_back(cv::Point3f(0, -large_half_y, large_half_z));
  // large_armor_points_.emplace_back(cv::Point3f(0, -large_half_y, -large_half_z));
}

bool PnPSolver::solvePnP(const Armor & armor, cv::Mat & rvec, cv::Mat & tvec)
{
  std::vector<cv::Point2f> image_armor_points;

  // Fill in image points
  image_armor_points.emplace_back(armor.left_light.bottom);
  image_armor_points.emplace_back(armor.left_light.top);
  image_armor_points.emplace_back(armor.right_light.top);
  image_armor_points.emplace_back(armor.right_light.bottom);

  //8个点
  //5   0   6
  //1       3
  //4   2   7
  // cv::Point2f average_top_point=cv::Point2f((armor.left_light.top.x+armor.right_light.top.x)/2,(armor.left_light.top.y+armor.right_light.top.y)/2);
  // cv::Point2f average_left_point=cv::Point2f((armor.left_light.bottom.x+armor.left_light.top.x)/2,(armor.left_light.bottom.y+armor.left_light.top.y)/2);
  // cv::Point2f average_bottom_point=cv::Point2f((armor.left_light.bottom.x+armor.right_light.bottom.x)/2,(armor.left_light.bottom.y+armor.right_light.bottom.y)/2);
  // cv::Point2f average_right_point=cv::Point2f((armor.right_light.bottom.x+armor.right_light.top.x)/2,(armor.right_light.bottom.y+armor.right_light.top.y)/2);

  // image_armor_points.emplace_back(average_top_point);
  // image_armor_points.emplace_back(average_left_point);
  // image_armor_points.emplace_back(average_bottom_point);
  // image_armor_points.emplace_back(average_right_point);

  // image_armor_points.emplace_back(armor.left_light.bottom);
  // image_armor_points.emplace_back(armor.left_light.top);
  // image_armor_points.emplace_back(armor.right_light.top);
  // image_armor_points.emplace_back(armor.right_light.bottom);

  // Solve pnp
  auto object_points = armor.type == ArmorType::SMALL ? small_armor_points_ : large_armor_points_;
  
  cv::solvePnP(
    object_points, image_armor_points, camera_matrix_, dist_coeffs_, rvec, tvec, false,
    cv::SOLVEPNP_IPPE);
  FLAGS_minloglevel = google::GLOG_FATAL;
  cv::Mat R;
  // 假设 object_points 和 image_armor_points 是 std::vector<cv::Point3_<float>> 类型的对象
  std::vector<cv::Point3_<double>> object_points_double(object_points.begin(), object_points.end());
  std::vector<cv::Point_<double>> image_armor_points_double(image_armor_points.begin(), image_armor_points.end());

// 然后将转换后的参数传递给 PoseOptimization 函数


  // PoseOptimization(object_points_double, image_armor_points_double, rvec, tvec);
  return true;
}

float PnPSolver::calculateDistanceToCenter(const cv::Point2f & image_point)
{
  float cx = camera_matrix_.at<double>(0, 2);
  float cy = camera_matrix_.at<double>(1, 2);
  return cv::norm(image_point - cv::Point2f(cx, cy));
}

void PoseOptimization(const std::vector<cv::Point3d>& points_3d,
                      const std::vector<cv::Point2d>& points_2d,
                      cv::Mat &rvec, cv::Mat &t)
{
    // Attention: cv::Mat::type
    assert(rvec.type() == CV_64F);
    assert(t.type() == CV_64F);

    double camera_rvec[3];
    camera_rvec[0] = rvec.at<double>(0,0); 
    camera_rvec[1] = rvec.at<double>(1,0);
    camera_rvec[2] = rvec.at<double>(2,0);
    
    double camera_t[3];
    camera_t[0] = t.at<double>(0,0);
    camera_t[1] = t.at<double>(1,0);
    camera_t[2] = t.at<double>(2,0);

    ceres::Problem problem;
    ceres::LossFunction* lossfunction = NULL;
    for(uint i = 0; i < points_3d.size(); i++)//将3D点和对应的图像点加入到代码函数中
    {
        Eigen::Vector3d p3d(points_3d[i].x, points_3d[i].y, points_3d[i].z);
        Eigen::Vector2d p2d(points_2d[i].x, points_2d[i].y);
        
        ceres::CostFunction* costfunction = ReprojectionError::Create(p3d, p2d);
        problem.AddResidualBlock(costfunction, lossfunction, camera_rvec, camera_t);//这一步也是我们照葫芦画瓢的关键，输入正确的参数
    }

    //设置优化器参数及优化方法
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.max_num_iterations = 100;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
    options.minimizer_progress_to_stdout = false;

    //调用求解器进行优化
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    // std::cout << summary.BriefReport() << std::endl;

    // std::cout << "After Optimizing: " << std::endl;
    double quat[4];
    ceres::AngleAxisToQuaternion(camera_rvec, quat);
    Eigen::Quaterniond q(quat[0], quat[1], quat[2], quat[3]);
   //对优化后的rt进行赋值还原
    rvec.at<double>(0,0) = camera_rvec[0];
    rvec.at<double>(1,0) = camera_rvec[1];
    rvec.at<double>(2,0) = camera_rvec[2];
    t.at<double>(0,0) = camera_t[0];
    t.at<double>(1,0) = camera_t[1];
    t.at<double>(2,0) = camera_t[2];
}


}  // namespace rm_auto_aim
