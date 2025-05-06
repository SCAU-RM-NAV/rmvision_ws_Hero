// Copyright 2022 Chen Jun
// Licensed under the MIT License.

#ifndef ARMOR_DETECTOR__ARMOR_HPP_
#define ARMOR_DETECTOR__ARMOR_HPP_

#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

// STL
#include <algorithm>
#include <string>
#include <sophus/so3.hpp>
#include <Eigen/Dense>

namespace rm_auto_aim
{
const int RED = 0;
const int BLUE = 1;
const int AUTO = 2;

  // Unit: m
  static constexpr float SMALL_ARMOR_WIDTH = 135 / 1000.0; // 130
  static constexpr float SMALL_ARMOR_HEIGHT = 55 / 1000.0; // 52.96
  static constexpr float LARGE_ARMOR_WIDTH = 225 / 1000.0;
  static constexpr float LARGE_ARMOR_HEIGHT = 55 / 1000.0;

  // 15 degree in rad
constexpr double FIFTTEN_DEGREE_RAD = 15 * CV_PI / 180;

enum class ArmorType { SMALL, LARGE, INVALID };
const std::string ARMOR_TYPE_STR[3] = {"small", "large", "invalid"};

struct CameraInternalK
{
  Eigen::Matrix3d toMatrix() const noexcept
  {
    Eigen::Matrix3d K;
    K << fx, 0, cx, 0, fy, cy, 0, 0, 1;
    return K;
  }
  double fx, fy, cx, cy;
};

struct Light : public cv::RotatedRect
{
  Light() = default;
  explicit Light(cv::RotatedRect box) : cv::RotatedRect(box)
  {
    cv::Point2f p[4];
    box.points(p);
    std::sort(p, p + 4, [](const cv::Point2f & a, const cv::Point2f & b) { return a.y < b.y; });
    top = (p[0] + p[1]) / 2;
    bottom = (p[2] + p[3]) / 2;

    length = cv::norm(top - bottom);
    width = cv::norm(p[0] - p[1]);

    tilt_angle = std::atan2(std::abs(top.x - bottom.x), std::abs(top.y - bottom.y));
    // tilt_angle = tilt_angle / CV_PI * 180;
    if((top.x - bottom.x) > 0){
      tilt_angle = tilt_angle / CV_PI * 180;
    }else{
      tilt_angle = -tilt_angle / CV_PI * 180;
    }
  }

  int color;
  cv::Point2f top, bottom;
  double length;
  double width;
  float tilt_angle;
};

struct Armor
{
  static constexpr const int N_LANDMARKS = 4;
  static constexpr const int N_LANDMARKS_2 = N_LANDMARKS * 2;
  Armor() = default;
  Armor(const Light & l1, const Light & l2)
  {
    if (l1.center.x < l2.center.x) {
      left_light = l1, right_light = l2;
    } else {
      left_light = l2, right_light = l1;
    }
    center = (left_light.center + right_light.center) / 2;
  }

    template <typename PointType>
  static inline std::vector<PointType> buildObjectPoints(
    const double & w, const double & h) noexcept
  {
    if constexpr (N_LANDMARKS == 4) {
      return {PointType(0, w / 2, -h / 2), PointType(0, w / 2, h / 2), PointType(0, -w / 2, h / 2),
              PointType(0, -w / 2, -h / 2)};
    } else {
      return {PointType(0, 0, -h / 2), PointType(0, w / 2, 0),  PointType(0,0 , -h / 2),PointType(0, -w / 2, 0),
              PointType(0, w / 2, -h / 2), PointType(0, w / 2, h /2), PointType(0, -w / 2, h / 2),PointType(0, -w / 2, -h / 2)};
    }
  }
  std::vector<cv::Point2f> landmarks() const
  {
    if constexpr (N_LANDMARKS == 4) {
      return {left_light.bottom, left_light.top, right_light.top, right_light.bottom};
    } else {
      return {
        // ((left_light.top.x + right_light.top.x) / 2, (left_light.top.y + right_light.top.y) / 2),
        // left_light.center,
        // ((left_light.bottom.x + right_light.bottom.x) / 2, (left_light.bottom.y + right_light.bottom.y) / 2),
        //  right_light.center, left_light.bottom, left_light.top, right_light.top,right_light.bottom
         left_light.bottom, left_light.top, right_light.top, right_light.bottom
         };
    }
  }

    // Armor pose part
  cv::Mat rmat;
  cv::Mat tvec;
  double roll;
  Eigen::Matrix3d imu2camera;

  // Light pairs part
  Light left_light, right_light;
  cv::Point2f center;
  ArmorType type;

  // Number part
  cv::Mat number_img;
  std::string number;
  float confidence;
  std::string classfication_result;
};

}  // namespace rm_auto_aim

#endif  // ARMOR_DETECTOR__ARMOR_HPP_
