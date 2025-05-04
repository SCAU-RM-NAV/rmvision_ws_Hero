// Copyright 2022 Chen Jun
// Licensed under the MIT License.

#ifndef ARMOR_DETECTOR__PNP_SOLVER_HPP_
#define ARMOR_DETECTOR__PNP_SOLVER_HPP_

#include <geometry_msgs/msg/point.hpp>
#include <opencv2/core.hpp>

// STD
#include <array>
#include <vector>

#include <math.h>

#include <rclcpp/rclcpp.hpp>
#include "armor_detector/armor.hpp"
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include <ceres/autodiff_cost_function.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <glog/logging.h>



namespace rm_auto_aim
{
    void PoseOptimization(const std::vector<cv::Point3d>& points_3d,
                      const std::vector<cv::Point2d>& points_2d,
                      cv::Mat &rvec, cv::Mat &t);
class PnPSolver
{
public:
  PnPSolver(
    const std::array<double, 9> & camera_matrix,
    const std::vector<double> & distortion_coefficients);

  // Get 3d position
  bool solvePnP(const Armor & armor, cv::Mat & rvec, cv::Mat & tvec);

  // Calculate the distance between armor center and image center
  float calculateDistanceToCenter(const cv::Point2f & image_point);

  cv::Mat camera_matrix_;
private:

  cv::Mat dist_coeffs_;



  // Four vertices of armor in 3d
  std::vector<cv::Point3f> small_armor_points_;
  std::vector<cv::Point3f> large_armor_points_;
};  // namespace rm_auto_aim
class ReprojectionError
{
public:
    ReprojectionError(Eigen::Vector3d point_, Eigen::Vector2d observed_)
        : point(point_), observed(observed_)
    {
    }
    
    template<typename T>
    bool operator()(const T* const camera_r, const T* const camera_t, T* residuals) const
{
        T pt1[3];  //3D点坐标系
        pt1[0] = T(point.x());
        pt1[1] = T(point.y());
        pt1[2] = T(point.z());

        T pt2[3];//对3D点进行坐标转换，换算到相机坐标系下
        ceres::AngleAxisRotatePoint(camera_r, pt1, pt2);

        pt2[0] = pt2[0] + camera_t[0];
        pt2[1] = pt2[1] + camera_t[1];
        pt2[2] = pt2[2] + camera_t[2];
        //归一化并将其转换到图像坐标系下
        const T xp = T(K[0] * (pt2[0] / pt2[2]) + K[2]);
        const T yp = T(K[1] * (pt2[1] / pt2[2]) + K[3]);
        const T u = T(observed.x());
        const T v = T(observed.y());
        //投影点与图像点的误差
        residuals[0] = u - xp;
        residuals[1] = v - yp;
        
        return true;
    }  
    static ceres::CostFunction* Create(Eigen::Vector3d points, Eigen::Vector2d observed)
{
        return (new ceres::AutoDiffCostFunction<ReprojectionError, 2, 3, 3>(
                        new ReprojectionError(points, observed)));
    }
private:
    Eigen::Vector3d point;
    Eigen::Vector2d observed;
    double K[4] = {3084.12524, 3089.19043, 671.95302, 506.27035}; // fx,fy,cx,cy
};
}
#endif  // ARMOR_DETECTOR__PNP_SOLVER_HPP_
