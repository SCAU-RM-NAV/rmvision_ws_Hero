#ifndef YOLO_INFERENCE_H_
#define YOLO_INFERENCE_H_
#include <opencv2/imgproc.hpp>
#include <openvino/openvino.hpp>
#include <string>
#include <vector>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include "auto_aim_interfaces/msg/detections.hpp"
#include <Eigen/Core>
#include <future>


namespace rm_auto_aim
{
class Detector;  // 前向声明
struct my_Detection;  // 确保前向声明

// 定义结构体
struct Key_PointAndFloat
{
  // 存储坐标的成员，使用 std::pair 存储两个整数表示坐标
  std::vector<cv::Point2f> key_point;
  // 存储浮点数的成员
  std::vector<float> value;
};

struct my_Detection
{
  short class_id = 0;
  float confidence;
  cv::Rect box;
  Key_PointAndFloat Key_Point;
};

class Inference
{
public:
  std::vector<int> cpu_cores = {0, 1, 2, 3};
  int inference_id;
  cv::Mat mat;

   std::atomic<int> run_id{0};  // 使用原子变量

  std::shared_ptr<Detector> detector_;
  //Detector detect;
  std::vector<std_msgs::msg::Header> headers;
  std::vector<std_msgs::msg::Header> temp_headers;
  std::vector<my_Detection> re_;
  //std::vector<cv::Mat> inputs;
  // 存储时直接保存共享指针
  std::vector<std::shared_ptr<cv::Mat>> my_inputs;

  std::vector<Eigen::Matrix3d>imu_to_camera_s;
  std::vector<Eigen::Matrix3d>temp_imu_to_camera_s;
  
  std::vector<int> counts;
  double Pose_Run_time = 0;
  double Pose_Run_img = 0;
  double huamianshu = 0;
  //	int flage=1;
  bool RUN = false;
  int num_requests = 2;  //mo ren

  std::string driver = "GPU";  //"MULTI:GPU.1,GPU.0" "BATCH:GPU(1)"

  rclcpp::Publisher<auto_aim_interfaces::msg::Detections>::SharedPtr detections_pub;
  void pose_publish_detections(std::vector<my_Detection> &results_,int id);

  Inference() {}
  // Constructor to initialize the model with default input shape
  Inference(
    const std::string & model_path, const float & model_confidence_threshold,
    const float & model_NMS_threshold);
  // Constructor to initialize the model with specified input shape
  Inference(
    const std::string & model_path, const cv::Size model_input_shape,
    const float & model_confidence_threshold, const float & model_NMS_threshold);

  Inference(
    const std::string & model_path, const cv::Size model_input_shape,
    const float & model_confidence_threshold, const float & model_NMS_threshold,
    rclcpp::Publisher<auto_aim_interfaces::msg::Detections>::SharedPtr &detections_pub_);
  Inference(
    const std::string & model_path, const cv::Size model_input_shape,
    const float & model_confidence_threshold, const float & model_NMS_threshold,
    std::string & driver, int & num_requests_);
  Inference(const std::string &model_path, const cv::Size model_input_shape,
    const float &model_confidence_threshold, const float &model_NMS_threshold,std::string &driver_, 
    rclcpp::Publisher<auto_aim_interfaces::msg::Detections>::SharedPtr &detections_pub_,
    std::shared_ptr<Detector> detector);

  void RunInference(cv::Mat & frame);
  void Pose_RunInference(cv::Mat & frame);
  bool Pose_Run_async_Inference(cv::Mat &frame,std_msgs::msg::Header header,Eigen::Matrix3d imu_to_camera_);
  std::vector<bool> flages;
private:
  void InitializeModel(const std::string & model_path);

  void Preprocessing(const cv::Mat &frame,int id);  //yu
  //void Pose_PostProcessing(cv::Mat &frame);
  void Pose_PostProcessing(cv::Mat & frame,int i);  //hou

  cv::Rect GetBoundingBox(const cv::Rect & src) const;
  Key_PointAndFloat GetKeyPointsinBox(Key_PointAndFloat & Key);
  void DrawDetectedObject(cv::Mat & frame, const my_Detection & detections) const;
  void Pose_DrawDetectedObject(cv::Mat & frame, const my_Detection & detections) const;
  cv::Point2f scale_factor_;      // Scaling factor for the input frame
  cv::Size2f model_input_shape_;  // Input shape of the model
  cv::Size model_output_shape_;   // Output shape of the model

  ov::InferRequest Ainference_request_;  // OpenVINO inference request
  ov::InferRequest Binference_request_;  // OpenVINO inference request
  ov::InferRequest Cinference_request_;  // OpenVINO inference request
  ov::InferRequest Dinference_request_;  // OpenVINO inference request

  std::vector<ov::InferRequest> inference_requests_;


  ov::CompiledModel compiled_model_;  // OpenVINO compiled model
  std::vector<ov::CompiledModel >compiled_models;
  //ov::InferRequest infer_request;

  float model_confidence_threshold_;  // Confidence threshold for detections
  float model_NMS_threshold_;         // Non-Maximum Suppression threshold

  std::vector<std::string> classes_{
    "RG",
    "BG",
  };

private:
  mutable std::mutex flage_mutex;  // 保护 flage 的互斥量
};

}  // namespace rm_auto_aim

#endif  // YOLO_INFERENCE_H_
