// Copyright 2022 Chen Jun
// Licensed under the MIT License.

#ifndef ARMOR_DETECTOR__DETECTOR_HPP_
#define ARMOR_DETECTOR__DETECTOR_HPP_

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>

// STD
#include <cmath>
#include <string>
#include <vector>

#include "armor_detector/armor.hpp"
#include "armor_detector/number_classifier.hpp"
#include "armor_detector/light_corner_corrector.hpp"
#include "auto_aim_interfaces/msg/debug_armors.hpp"
#include "auto_aim_interfaces/msg/debug_lights.hpp"
#include <rclcpp/rclcpp.hpp>
#include "armor_detector/openvino_pose_inference.hpp"

namespace rm_auto_aim
{
  
class Detector
{
public:
  struct LightParams
  {
    // width / height
    double min_ratio;
    double max_ratio;
    // vertical angle
    double max_angle;
    // area condition
    double min_fill_ratio;
  };

  struct ArmorParams
  {
    double min_light_ratio;
    // armor length pair width 
    double min_small_armor_ratio;
    double max_small_armor_ratio;
    double min_large_armor_ratio;
    double max_large_armor_ratio;
    // horizontal angle
    double max_angle;
  };

  Detector(const int & bin_thres, const int & color, const LightParams & l, const ArmorParams & a);

  std::vector<Armor> detect(const cv::Mat & input);

  std::vector<Armor> pose_find_armors(std::vector<my_Detection> & pose_result);
 std::vector<Armor> pose_detect(std::vector<my_Detection> & result_ ,const cv::Mat & input);

  cv::Mat preprocessImage(const cv::Mat & input);
  std::vector<Light> findLights(const cv::Mat & rbg_img, const cv::Mat & binary_img);
  std::vector<Armor> matchLights(const std::vector<Light> & lights);

  // For debug usage
  cv::Mat getAllNumbersImage();
  void drawResults(cv::Mat & img);

  int binary_thres;
  int detect_color;
  LightParams l;
  ArmorParams a;
  float armor_radio_;


  std::unique_ptr<NumberClassifier> classifier;
  std::unique_ptr<LightCornerCorrector> corner_corrector;
  std::unique_ptr<Inference> pose_inference;
  std::unique_ptr<Inference> Apose_inference;
  std::unique_ptr<Inference> Bpose_inference;
  std::unique_ptr<Inference> Cpose_inference;
  // Debug msgs
  cv::Mat binary_img;
  auto_aim_interfaces::msg::DebugLights debug_lights;
  auto_aim_interfaces::msg::DebugArmors debug_armors;

  //pose
    std::vector<std::unique_ptr<Inference>> all_inferences;
    int xiancheng=3;

private:
  bool isLight(const Light & possible_light);
  bool containLight(
    const Light & light_1, const Light & light_2, const std::vector<Light> & lights);
  ArmorType isArmor(const Light & light_1, const Light & light_2);

  cv::Mat gray_img_;
  
  std::vector<Light> lights_;
  std::vector<Armor> armors_;
};

}  // namespace rm_auto_aim

#endif  // ARMOR_DETECTOR__DETECTOR_HPP_
