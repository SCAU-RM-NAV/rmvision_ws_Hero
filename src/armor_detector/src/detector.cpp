// Copyright (c) 2022 ChenJun
// Licensed under the MIT License.

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

// STD
#include <algorithm>
#include <cmath>
#include <vector>
//文件和时间
#include <filesystem>
#include <chrono> 

#include "armor_detector/detector.hpp"
#include "auto_aim_interfaces/msg/debug_armor.hpp"
#include "auto_aim_interfaces/msg/debug_light.hpp"

namespace rm_auto_aim
{
Detector::Detector(
  const int & bin_thres, const int & color, const LightParams & l, const ArmorParams & a)
: binary_thres(bin_thres), detect_color(color), l(l), a(a)
{
}
  void Detector::get_images_train(const std::vector<Armor> & armors_, const cv::Mat & input)
{
      // 将 RGB 图像转换为 BGR 图像
    cv::Mat bgrImage;
    cv::cvtColor(input, bgrImage, cv::COLOR_RGB2BGR);

  if(armors_.size())
  {
  std::ostringstream path;
path << "/home/auto/Desktop/train/";
  // 获取当前时间
  std::time_t now = std::time(nullptr);
  std::tm * ltm = std::localtime(&now);

  // 格式化时间字符串，用作文件名
  std::ostringstream fileName;
  fileName << path.str() << "labels/" << "example_"
           << std::put_time(ltm, "%Y-%m-%d_%H-%M-%S")  // 格式化为 YYYY-MM-DD_HH-MM-SS
           << ".txt";
  std::ostringstream fileName_;
  fileName_ << path.str() << "images/" << "example_"
            << std::put_time(ltm, "%Y-%m-%d_%H-%M-%S")  // 格式化为 YYYY-MM-DD_HH-MM-SS
            << ".jpg";
  
  std::cout << "File path for labels: " << fileName.str() << std::endl;
std::cout << "File path for images: " << fileName_.str() << std::endl;

  // 创建文件并写入内容
  std::ofstream outFile(fileName.str());

  // 检查文件是否成功打开
  if (!outFile) {
    std::cerr << "无法创建文件！" << std::endl;
    return ;  // 返回错误代码
  }
  cv::imwrite(fileName_.str(), bgrImage);

  // 保存图片和labels
  for (auto & armor : armors_) {
    // 写入内容
    //outFile << "Hello, World!" << std::endl;

    int class_index = armor.left_light.color;
    double x = armor.center.x;
    double y = armor.center.y;
    double zuo;
    double you;
    double shang;
    double xia;
    if (armor.left_light.top.x > armor.left_light.bottom.x) {
      zuo = armor.left_light.bottom.x;
    } else {
      zuo = armor.left_light.top.x;
    }

    //xia
    if (armor.right_light.bottom.y > armor.left_light.bottom.y) {
      xia = armor.right_light.bottom.y;
    } else {
      xia = armor.left_light.bottom.y;
    }

    //you
    if (armor.right_light.top.x > armor.right_light.bottom.x) {
      you = armor.right_light.top.x;
    } else {
      you = armor.right_light.bottom.x;
    }

    //shang
    if (armor.right_light.top.y > armor.left_light.top.y) {
      shang = armor.left_light.top.y;
    } else {
      shang = armor.right_light.top.y;
    }

    //double images_w=input.cols;
    outFile << class_index << " " << x / (double)input.cols << " " << y / (double)input.rows
              << " " << (you - zuo)/ (double)input.cols  << " " << (xia - shang)/ (double)input.rows << " "  //0在左上 逆时针
              << armor.left_light.top.x/ (double)input.cols << " " << armor.left_light.top.y / (double)input.rows<< " " << "2" << " "
              << armor.left_light.bottom.x / (double)input.cols<< " " << armor.left_light.bottom.y / (double)input.rows<< " " << "2"<< " " 
              << armor.right_light.bottom.x / (double)input.cols<< " " << armor.right_light.bottom.y / (double)input.rows<< " " << "2"<< " " 
              << armor.right_light.top.x/ (double)input.cols << " " << armor.right_light.top.y/ (double)input.rows << " " << "2"<< std::endl;
  }
  // 关闭文件
  outFile.close();
  }
}
/**
 * @brief 检测装甲板
 * @param input 待检测图像
 * @return 所有检测到的装甲板构成的容器
 */

std::vector<Armor> Detector::detect(const cv::Mat & input)
{
  // 预处理
  binary_img = preprocessImage(input);
  // 寻找装甲板灯条
  lights_ = findLights(input, binary_img);
  // 灯条匹配寻找装甲板
  armors_ = matchLights(lights_);

  if (!armors_.empty()) {
    // 提取数字二值化图像
    classifier->extractNumbers(input, armors_);
    // 识别数字，并对装甲板进一步过滤
    classifier->classify(armors_);
  }

  // 装甲板灯条角点修正
  for (auto &armor : armors_){
    if (corner_corrector != nullptr ) {
      corner_corrector->correctCorners(armor, gray_img_);
      }
    }

    //拍数据及
  huamianshu++;
  if(huamianshu>100){
  get_images_train(armors_,input);
  huamianshu=0;
  }

  return armors_;
}

std::vector<Armor> Detector::pose_detect(std::vector<my_Detection> & result_ ,const cv::Mat & input)
{

  armors_ = pose_find_armors(result_);


  if (!armors_.empty()) {
    classifier->extractNumbers(input, armors_);
    classifier->classify(armors_);
  }
  //std::cout<<" pose_detect return armors 大小 ="<<armors_.size()<<std::endl;
  return armors_;
}

//openvino_pose 寻找装甲板
std::vector<Armor> Detector::pose_find_armors(std::vector<my_Detection> & pose_result)
{
  std::vector<Armor> armors;
for(size_t i=0;i<pose_result.size();i++)
{
  Armor armor;
  if(static_cast<int>(pose_result[i].class_id) != detect_color)
  {
    continue;
    std::cout<<" 没有符合颜色的装甲板"<<std::endl;
  }
  armor.left_light.color = armor.right_light.color = static_cast<int>(pose_result[i].class_id);


  armor.left_light.top = pose_result[i].Key_Point.key_point[0];
  armor.left_light.bottom = pose_result[i].Key_Point.key_point[1]; 


  armor.left_light.width = abs(armor.left_light.top.x - armor.left_light.bottom.x);
  armor.left_light.length = cv::norm(armor.left_light.top - armor.left_light.bottom);

  armor.left_light.tilt_angle = std::atan2(
    std::abs(armor.left_light.top.x - armor.left_light.bottom.x),
    std::abs(armor.left_light.top.y - armor.left_light.bottom.y));

  armor.left_light.tilt_angle = armor.left_light.tilt_angle / CV_PI * 180;

  armor.right_light.top = pose_result[i].Key_Point.key_point[3];
  armor.right_light.bottom = pose_result[i].Key_Point.key_point[2];
  armor.right_light.width = abs(armor.right_light.top.x - armor.right_light.bottom.x);
  armor.right_light.length = cv::norm(armor.right_light.top - armor.right_light.bottom);
  armor.right_light.tilt_angle = std::atan2(
    std::abs(armor.right_light.top.x - armor.right_light.bottom.x),
    std::abs(armor.right_light.top.y - armor.right_light.bottom.y));
  armor.right_light.tilt_angle = armor.right_light.tilt_angle / CV_PI * 180;

  armor.left_light.center= (armor.left_light.top + armor.left_light.bottom) / 2;
  armor.right_light.center= (armor.right_light.top + armor.right_light.bottom) / 2;



  auto type = isArmor(armor.left_light, armor.right_light); //降好多帧 要改
  if (type != ArmorType::INVALID) {
    armor.type = type;
    armors.emplace_back(armor);
  }
      //armor.type=ArmorType::SMALL;

}
  //std::cout<<" pose_find_armors return armors 大小 ="<<armors.size()<<std::endl;
  return armors;
}

/**
 * @brief 预处理图像
 * @param rgb_img 待检测图像
 * @return 预处理后的二值化图像
 */
cv::Mat Detector::preprocessImage(const cv::Mat & rgb_img)
{
  // 转为灰度图
  cv::cvtColor(rgb_img, gray_img_, cv::COLOR_RGB2GRAY);

  // 二值化
  cv::Mat binary_img;
  cv::threshold(gray_img_, binary_img, binary_thres, 255, cv::THRESH_BINARY);
 
  return binary_img;
}

/**
 * @brief 寻找灯条
 * @param rgb_img 原图像
 * @param binary_img 二值化图像
 * @return 装甲板灯条构成的容器
 */
std::vector<Light> Detector::findLights(const cv::Mat & rbg_img, const cv::Mat & binary_img)
{
  using std::vector;
  // 在二值化图上寻找轮廓
  vector<vector<cv::Point>> contours;
  vector<cv::Vec4i> hierarchy;
  cv::findContours(binary_img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 初始化灯条容器
  vector<Light> lights;

  // 清空调试显示的灯条数据
  this->debug_lights.data.clear();

  // 遍历轮廓
  for (const auto & contour : contours) {
    // 排除点数少于5的轮廓，因为不可用于拟合
    if (contour.size() < 5) continue;

    // 分别对轮廓进行不可旋转矩阵和可以旋转矩形的最小外接矩形拟合
    auto b_rect = cv::boundingRect(contour);
    auto r_rect = cv::minAreaRect(contour);

    // 按照b_rect对的尺寸创建单通道全0掩膜
    cv::Mat mask = cv::Mat::zeros(b_rect.size(), CV_8UC1);

    // 计算原图轮廓点相对于b_rect的坐标（将其变换到b_rect中），装入mask_contour容器中
    std::vector<cv::Point> mask_contour;
    for (const auto & p : contour) {
      mask_contour.emplace_back(p - cv::Point(b_rect.x, b_rect.y));
    }

    // 根据上面计算出的轮廓相对坐标，在mask中绘制轮廓构成的多边形（多边形内部也会填充，值为255）
    cv::fillPoly(mask, {mask_contour}, 255);

    // 寻找mask中的轮廓构成点阵（这里不同于mask_contour，mask_contour只是轮廓外面一圈的点，points是包括轮廓内的点）
    std::vector<cv::Point> points;
    cv::findNonZero(mask, points);

    // 用上面点阵的点数（对应轮廓面积）除以旋转矩阵面积，计算比例进行比较
    bool is_fill_rotated_rect =
      points.size() / (r_rect.size.width * r_rect.size.height) > l.min_fill_ratio;

    // 用旋转矩阵构造灯条对象
    auto light = Light(r_rect);

    if (isLight(light) && is_fill_rotated_rect) {
      auto rect = light.boundingRect();
      if (0 <= rect.x && 0 <= rect.width && rect.x + rect.width <= rbg_img.cols && 0 <= rect.y &&
          0 <= rect.height && rect.y + rect.height <= rbg_img.rows) {
        // 颜色判定
        int sum_r = 0, sum_b = 0;
        auto roi = rbg_img(rect);
        for (int i = 0; i < roi.rows; i++) {
          for (int j = 0; j < roi.cols; j++) {
            if (cv::pointPolygonTest(contour, cv::Point2f(j + rect.x, i + rect.y), false) >= 0) {
              sum_r += roi.at<cv::Vec3b>(i, j)[0];
              sum_b += roi.at<cv::Vec3b>(i, j)[2];
            }
          }
        }
        light.color = sum_r > sum_b ? RED : BLUE;
        lights.emplace_back(light);
      }
    }
  }

  return lights;
}


/**
 * @brief 判断是否为符合条件的灯条
 * @param light 待检测灯条
 * @return 是否符合条件
 */
bool Detector::isLight(const Light & light)
{
  // 宽与长比例约束
  float ratio = light.width / light.length;
  bool ratio_ok = l.min_ratio < ratio && ratio < l.max_ratio;

  // 角度约束
  bool angle_ok = std::abs(light.tilt_angle) < l.max_angle;

  // std::cout<<"l.min_ratio:"<<l.min_ratio<<std::endl;
  // std::cout<<"l.max_ratio:"<<l.max_ratio<<std::endl;
  // std::cout<<"ratio_ok:"<<ratio_ok<<std::endl;
  // std::cout<<"angle_ok:"<<angle_ok<<std::endl;

  bool is_light = ratio_ok && angle_ok;

  // 将上述待判断灯条的数据和判定结果写入debug中
  auto_aim_interfaces::msg::DebugLight light_data;
  light_data.center_x = light.center.x;
  light_data.ratio = ratio;
  light_data.angle = light.tilt_angle;
  light_data.is_light = is_light;
  this->debug_lights.data.emplace_back(light_data);

  return is_light;
}

/**
 * @brief 灯条匹配寻找装甲板
 * @param lights 待匹配装甲板灯条
 * @return 有效装甲板构成的容器
 */
std::vector<Armor> Detector::matchLights(const std::vector<Light> & lights)
{
  std::vector<Armor> armors;
  // 清空调试显示的装甲板数据
  this->debug_armors.data.clear();

  // 遍历所有组合
  for (auto light_1 = lights.begin(); light_1 != lights.end(); light_1++) {
    for (auto light_2 = light_1 + 1; light_2 != lights.end(); light_2++) {
      // 排除颜色不匹配的灯条
      if (light_1->color != detect_color || light_2->color != detect_color) continue;

      // 排除灯条之间还有灯条的
      if (containLight(*light_1, *light_2, lights)) continue;

      // 尝试将灯条匹配成装甲板
      auto type = isArmor(*light_1, *light_2);

      // 将有效装甲板构成装甲板对象放入容器
      if (type != ArmorType::INVALID) {
        auto armor = Armor(*light_1, *light_2);
        armor.type = type;
        armor.armor_ratio = armor_radio_;
        armors.emplace_back(armor);
      }
    }
  }

  return armors;
}

/**
 * @brief 检测灯条直接是否还有灯条
 * @param light_1,light_2 待检测灯条
 * @param lights 所有待匹配灯条构成的容器
 * @return 是否包含其它灯条
 */
bool Detector::containLight(
  const Light & light_1, const Light & light_2, const std::vector<Light> & lights)
{
  // 使用两个灯条上下两个点总共四个点构成矩形
  auto points = std::vector<cv::Point2f>{light_1.top, light_1.bottom, light_2.top, light_2.bottom};
  auto bounding_rect = cv::boundingRect(points);

  // 遍历所有灯条
  for (const auto & test_light : lights) {
    // 排除待检测灯条
    if (test_light.center == light_1.center || test_light.center == light_2.center) continue;

    // 使用三点检测是否包含
    if (bounding_rect.contains(test_light.top) || bounding_rect.contains(test_light.bottom) ||
      bounding_rect.contains(test_light.center)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief 检测灯条是否能匹配成装甲板
 * @param light_1,light_2 待匹配灯条
 * @return 装甲板类型（大装甲板，小装甲板，无效装甲板）
 */
ArmorType Detector::isArmor(const Light & light_1, const Light & light_2)
{
  // Ratio of the length of 2 lights (short side / long side)
  // 灯条长度length比，短的比长的
  float light_length_ratio = light_1.length < light_2.length ? light_1.length / light_2.length
                                                             : light_2.length / light_1.length;
  bool light_ratio_ok = light_length_ratio > a.min_light_ratio;

  // Distance between the center of 2 lights (unit : light length)
  // 灯条中点间距和灯条平均长度的比（这样的比例才不会受到距离影响）是否满足大/小装甲板应该有的比例区间
  float avg_light_length = (light_1.length + light_2.length) / 2;
  float armor_ratio = cv::norm(light_1.center - light_2.center) / avg_light_length;
  bool armor_ratio_ok = (a.min_small_armor_ratio <= armor_ratio && armor_ratio < a.max_small_armor_ratio) ||
                            (a.min_large_armor_ratio <= armor_ratio && armor_ratio < a.max_large_armor_ratio);
 
  // 检测两灯条中点连线相对于水平线的角度是否满足要求
  cv::Point2f diff = light_1.center - light_2.center;
  float angle = std::abs(std::atan(diff.y / diff.x)) / CV_PI * 180;
  bool angle_ok = angle < a.max_angle;

  // 两个灯条的倾斜角度差是否满足要求
  bool light_angle_ok = std::abs(light_1.tilt_angle - light_2.tilt_angle) < 7.5;

  // bool is_armor = light_ratio_ok && armor_ratio_ok && angle_ok && light_angle_ok;
  bool is_armor = armor_ratio_ok && angle_ok && light_angle_ok;
  std::cout<<" light_ratio_ok="<<light_ratio_ok<<std::endl;
  std::cout<<" armor_ratio_ok="<<armor_ratio_ok<<std::endl;
  std::cout<<" angle_ok="<<angle_ok<<std::endl;
  std::cout<<" light_angle_ok="<<light_angle_ok<<std::endl;
  // 判断装甲板类型
  ArmorType type;
  if (is_armor) {
    type = armor_ratio > a.min_large_armor_ratio ? ArmorType::LARGE : ArmorType::SMALL;
    armor_radio_ = armor_ratio;
  } else {
    type = ArmorType::INVALID;
  }

  // 将上述装甲板信息和判断结果写入调试容器中
  auto_aim_interfaces::msg::DebugArmor armor_data;
  armor_data.type = ARMOR_TYPE_STR[static_cast<int>(type)];
  armor_data.center_x = (light_1.center.x + light_2.center.x) / 2;
  armor_data.light_ratio = light_length_ratio;
  armor_data.armor_ratio = armor_ratio;
  //std::cout<<" armor_ratio="<<armor_ratio<<std::endl;
  armor_data.angle = angle;
  // 把并非无效的装甲板排在前面
  if(type == ArmorType::INVALID)
  {
      this->debug_armors.data.emplace_back(armor_data);
      std::cout<<" INVALID 装甲板没过type "<<std::endl;
  }
  else
  {
      this->debug_armors.data.emplace(this->debug_armors.data.begin(), armor_data);
  }

  return type;
}

/**
 * @brief 返回所有装甲板数字二值化图像
 * @return 装甲板数字二值化图像
 */
cv::Mat Detector::getAllNumbersImage()
{
  if (armors_.empty()) {
    return cv::Mat(cv::Size(20, 28), CV_8UC1);
  } 
  else {
    std::vector<cv::Mat> number_imgs;
    number_imgs.reserve(armors_.size());
    for (auto & armor : armors_) {
      number_imgs.emplace_back(armor.number_img);
    }
    cv::Mat all_num_img;
    cv::vconcat(number_imgs, all_num_img);
    return all_num_img;
  }
}

/**
 * @brief 返回识别结果图像
 * @param img 要绘制的图像
 */
void Detector::drawResults(cv::Mat & img)
{
  // 绘制所有灯条
  for (const auto & light : lights_) {
    cv::circle(img, light.top, 5, cv::Scalar(255, 255, 255), 1);
    cv::circle(img, light.bottom, 5, cv::Scalar(255, 255, 255), 1);
    auto line_color = light.color == RED ? cv::Scalar(255, 255, 0) : cv::Scalar(255, 0, 255);
    cv::line(img, light.top, light.bottom, line_color, 1);
  }

  // 绘制所有装甲板
  for (const auto & armor : armors_) {
    cv::line(img, armor.left_light.top, armor.right_light.bottom, cv::Scalar(0, 255, 0), 2);
    cv::line(img, armor.left_light.bottom, armor.right_light.top, cv::Scalar(0, 255, 0), 2);
    cv::circle(img, cv::Point2f((armor.left_light.top.x+armor.right_light.top.x)/2,(armor.left_light.top.y+armor.right_light.top.y)/2), 5, cv::Scalar(0, 255, 0), 1);
    
  }

  // 绘制所有装甲板的数字识别结果
  for (const auto & armor : armors_) {
    cv::putText(
      img, armor.classfication_result, armor.left_light.top, cv::FONT_HERSHEY_SIMPLEX, 0.8,
      cv::Scalar(0, 255, 255), 2);
  }
}

}  // namespace rm_auto_aim


