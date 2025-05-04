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

#include "base_detector/base_light.hpp"
#include "base_detector/base.hpp"

namespace rm_auto_aim
{
Detector::Detector(
  const int & bin_thres, 
  const LightParams & l)
    : binary_thres(bin_thres), 
      l(l)
{
}

std::vector<Base> Detector::detect(const cv::Mat & input)
{
  binary_img = preprocessImage(input);
  if(binary_img.empty())
  std::cout<<"1"<<std::endl;
  bases_ = findLights(input, binary_img);
  //std::cout<<"2"<<std::endl;
  return bases_;
}

cv::Mat Detector::preprocessImage(const cv::Mat & rgb_img)
{
      // 1. 提取绿色通道（RGB图像的G通道）
  cv::Mat green_channel;
  cv::extractChannel(rgb_img, green_channel, 1); // 1表示G通道（0=B,1=G,2=R）
  //cv::cvtColor(rgb_img, gray_img_, cv::COLOR_RGB2GRAY);
  cv::Mat binary_img;
  cv::threshold(green_channel, binary_img, binary_thres, 255, cv::THRESH_BINARY);
    //cv::imwrite("1.jpg",binary_img);
    //cv::imwrite("2.jpg",rgb_img);

  return binary_img;

}

std::vector<Base> Detector::findLights(const cv::Mat & rgb_img, const cv::Mat & binary_img)
{

    // Step 1: 预处理二值图（降噪）
    cv::Mat processed_binary;
    cv::morphologyEx(binary_img, processed_binary, cv::MORPH_OPEN, 
                    cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));

    // Step 2: 提取轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(processed_binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::cout<<" con:"<<contours.size()<<std::endl;

    // Step 3: 遍历所有轮廓并筛选
    std::vector<Base> detected_bases;
    for (const auto& contour : contours) 
    {
        // 3.1 封装轮廓为 Base 对象
        Base candidate;
        candidate.contour = contour;
        candidate.bounding_rect = cv::boundingRect(contour);
        cv::minEnclosingCircle(contour, candidate.center, candidate.radius);

        // 3.2 调用 isLight 验证
        if (isBase(candidate)) 
        {
            detected_bases.push_back(candidate);
        }
        
    }

    return detected_bases;

}

bool Detector::isBase(const Base & base)
{
    std::cout<<"面积"<<cv::contourArea(base.contour)<<"  "<<"半径"<<base.radius<<std::endl;

    //     // 1. 半径过滤
    if (base.radius < l.min_radius || base.radius > l.max_radius) 
        return false;

    // // 2. 面积过滤
    const double area = cv::contourArea(base.contour);
    if (area < l.min_area || area > l.max_area) 
        return false;

    // 3. 圆形度计算
    // const double perimeter = cv::arcLength(base.contour, true);
    // if (perimeter <= 0) return false;
    // const double circularity = 4 * CV_PI * area / (perimeter * perimeter);
    // if (circularity < l.min_circularity) 
    //     return false;

    // //4. 宽高比验证
    // const cv::Rect& rect = base.bounding_rect;
    // const float aspect_ratio = static_cast<float>(rect.width) / rect.height;
    // if (aspect_ratio < l.min_ratio || aspect_ratio > l.max_ratio) 
    //     return false;
    
    return true;
}

void Detector::drawResults(cv::Mat & img)
{
for (const auto & base : bases_) 
{
    // 绘制圆形
    cv::circle(img, base.center, static_cast<int>(base.radius), 
              cv::Scalar(0, 255, 0), 2);
              
    // 绘制中心点
    cv::drawMarker(img, base.center, cv::Scalar(0, 0, 255),
                  cv::MARKER_CROSS, 20, 2);
                  
    // 显示半径
    std::stringstream ss;
    ss << "R:" << std::fixed << std::setprecision(1) << base.radius;
    cv::putText(img, ss.str(), base.center + cv::Point2f(0, 30),
    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 255), 2);
}

}


}  //namespace rm_auto_aim