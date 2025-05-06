#ifndef BASE_HPP_
#define BASE_HPP_

#include <opencv2/opencv.hpp>

// 单盏绿色圆灯的结构体
struct Base
{
    Base() = default;
    explicit Base(const cv::Point2f &center,const float &radius)
      : center(center), radius(radius) {}
      
    cv::Point2f center;  // 圆心坐标
    float radius;         // 圆半径
    float area;
    double light_ratio;
    cv::Rect bounding_rect;
    
    cv::Point left_top;
    cv::Point right_top;
    cv::Point left_bottom;
    cv::Point right_bottom;
    std::vector<cv::Point> contour;
};

//BoundingBox left_top,left_buttom_,right_top,right_buttom;
#endif