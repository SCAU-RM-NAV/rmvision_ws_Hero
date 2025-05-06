#ifndef BASE_DETECTOR__DETECTOR_HPP_
#define BASE_DETECTOR__DETECTOR_HPP_

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>

// STD
#include <cmath>
#include <string>
#include <vector>

#include "base_detector/base_light.hpp"
#include <rclcpp/rclcpp.hpp>
#include "auto_aim_interfaces/msg/debug_bases.hpp"

namespace rm_auto_aim
{
class Detector
{
public:
  struct LightParams
  {
    // cv::Rect bounding_rect;
    double min_area = 300;
    double max_area = 8000;
    float min_radius = 10.0;
    float max_radius = 40.0;   
    double min_ratio = 0.75;
    double max_ratio = 1.25;
  };

  Detector(const int & bin_thres, const LightParams & l);

  std::vector<Base> detect(const cv::Mat & input);
  std::vector<Base> candidate;
  cv::Mat preprocessImage(const cv::Mat & input);
  std::vector<Base> findLights(const cv::Mat & rgb_img, const cv::Mat & binary_img);


  void drawResults(cv::Mat & img);//debug用
  int binary_thres = 70;
  LightParams l;
  // Debug msgs
  cv::Mat binary_img;
  //msg也写在这里

  private:
  bool isBase(const Base & possible_base);
  cv::Mat gray_img_;
  cv::Mat hsv_img, green_mask;


  std::vector<Base> bases_;
  };

  }// namespace rm_auto_aim

  #endif  // BASE_DETECTOR__DETECTOR_HPP_