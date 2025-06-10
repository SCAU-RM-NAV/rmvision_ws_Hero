#include "MvCameraControl.h"
// ROS
#include <camera_info_manager/camera_info_manager.hpp>
#include <image_transport/image_transport.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/utilities.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
// C++ system
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <chrono>

#include "auto_aim_interfaces/msg/gimbal_fdb.hpp" 

namespace hik_camera
{
class HikCameraNodeA : public rclcpp::Node
{
public:
  explicit HikCameraNodeA(const rclcpp::NodeOptions & options) : Node("hik_camera_a", options)
  {
    RCLCPP_INFO(this->get_logger(), "Starting HikCameraNode_A!");

    MV_CC_DEVICE_INFO_LIST device_list;
    // enum device
    nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &device_list);
    RCLCPP_INFO(this->get_logger(), "Found camera count = %d", device_list.nDeviceNum);

    while (device_list.nDeviceNum == 0 && rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "No camera found!");
      RCLCPP_INFO(this->get_logger(), "Enum state: [%x]", nRet);
      std::this_thread::sleep_for(std::chrono::seconds(1));
      nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &device_list);
    }



    MV_CC_CreateHandle(&camera_handle_, device_list.pDeviceInfo[0]);
    RCLCPP_INFO(this->get_logger(), "Selected device index 0");
    MV_CC_OpenDevice(camera_handle_);

    // Get camera infomation
    MV_CC_GetImageInfo(camera_handle_, &img_info_);
    image_msg_.data.reserve(img_info_.nHeightMax * img_info_.nWidthMax * 3);

    // Init convert param
    convert_param_.nWidth = img_info_.nWidthValue;
    convert_param_.nHeight = img_info_.nHeightValue;
    convert_param_.enDstPixelType = PixelType_Gvsp_RGB8_Packed;

    bool use_sensor_data_qos = this->declare_parameter("use_sensor_data_qos", false);
    auto qos = use_sensor_data_qos ? rmw_qos_profile_sensor_data : rmw_qos_profile_default;
    camera_pub_A = image_transport::create_camera_publisher(this, "image_raw_a", qos);\
    camera_pub_green = image_transport::create_camera_publisher(this, "image_raw_green", qos);

    declareParameters();

    MV_CC_StartGrabbing(camera_handle_);

    // Load camera info
    camera_name_ = this->declare_parameter("camera_name", "hik_camera_a");
    camera_info_manager_ =
      std::make_unique<camera_info_manager::CameraInfoManager>(this, camera_name_);
    auto camera_info_url_A =
      this->declare_parameter("camera_info_url_A", "package://hik_camera/config/camera_info_A.yaml");
    if (camera_info_manager_->validateURL(camera_info_url_A)) {
      camera_info_manager_->loadCameraInfo(camera_info_url_A);
      camera_info_msg_ = camera_info_manager_->getCameraInfo();
    } else {
      RCLCPP_WARN(this->get_logger(), "Invalid camera info URL: %s", camera_info_url_A.c_str());
    }

    params_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&HikCameraNodeA::parametersCallback, this, std::placeholders::_1));

    cam_gimbal_fdb_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalFdb>(
    "/gimbal_fdb", rclcpp::SensorDataQoS(),
    std::bind(&HikCameraNodeA::camGimbalFdbCallback, this, std::placeholders::_1));


    capture_thread_ = std::thread{[this]() -> void {
      MV_FRAME_OUT out_frame;

      RCLCPP_INFO(this->get_logger(), "Publishing cam_A!");

      image_msg_.header.frame_id = "camera_optical_frame";
      image_msg_.encoding = "rgb8";

    
      while (rclcpp::ok()) {
        nRet = MV_CC_GetImageBuffer(camera_handle_, &out_frame, 1000);
        if (MV_OK == nRet) {
          convert_param_.pDstBuffer = image_msg_.data.data();
          convert_param_.nDstBufferSize = image_msg_.data.size();
          convert_param_.pSrcData = out_frame.pBufAddr;
          convert_param_.nSrcDataLen = out_frame.stFrameInfo.nFrameLen;
          convert_param_.enSrcPixelType = out_frame.stFrameInfo.enPixelType;

          MV_CC_ConvertPixelType(camera_handle_, &convert_param_);

          camera_info_msg_.header.stamp = image_msg_.header.stamp = this->now();
          image_msg_.height = out_frame.stFrameInfo.nHeight;
          image_msg_.width = out_frame.stFrameInfo.nWidth;
          image_msg_.step = out_frame.stFrameInfo.nWidth * 3;
          image_msg_.data.resize(image_msg_.width * image_msg_.height * 3);
          
        // for (int i = 0; i < 10; ++i) {
        //   RCLCPP_INFO(this->get_logger(), "Data %d: 0x%X", i, image_msg_.data[i]);
        // }


          if(aiming_mode == 0)
          {
            //std::cout<<"image_msg_.height"<<image_msg_.height<<std::endl;
            camera_pub_A.publish(image_msg_, camera_info_msg_);
            std::cout<<"image_raw_a在发布"<<std::endl;
          }
          if(aiming_mode == 1)
          {
            camera_pub_green.publish(image_msg_, camera_info_msg_);
            std::cout<<"image_raw_green在发布"<<std::endl;
          }
          MV_CC_FreeImageBuffer(camera_handle_, &out_frame);
          fail_conut_ = 0;
        } else {
          RCLCPP_WARN(this->get_logger(), "Get buffer failed! nRet: [%x]", nRet);
          MV_CC_StopGrabbing(camera_handle_);
          MV_CC_StartGrabbing(camera_handle_);
          fail_conut_++;
        }

        if (fail_conut_ > 15) {
          RCLCPP_FATAL(this->get_logger(), "Camera failed!");
          camera_reconnect();
        }
      }
    }};
  }

  ~HikCameraNodeA() override
  {
    if (capture_thread_.joinable()) {
      capture_thread_.join();
    }
    if (camera_handle_) {
      MV_CC_StopGrabbing(camera_handle_);
      MV_CC_CloseDevice(camera_handle_);
      MV_CC_DestroyHandle(&camera_handle_);
    }
    RCLCPP_INFO(this->get_logger(), " destroyed!");
  }

private:
// void resetCameraParameters()
// {
//   // 重置触发模式为连续采集
//   MV_CC_SetEnumValue(camera_handle_, "TriggerMode", 0);
//   // 重置自动白平衡
//   MV_CC_SetEnumValue(camera_handle_, "BalanceWhiteAuto", 1);
//   // 其他关键参数重置...
// }

  void declareParameters()
  {
    rcl_interfaces::msg::ParameterDescriptor param_desc;
    
    this->declare_parameter("exposure_time_red", 3000, param_desc);
    this->declare_parameter("exposure_time_blue", 3000, param_desc);
    //this->declare_parameter("detect_color", 2, param_desc);


    MVCC_FLOATVALUE f_value;
    param_desc.integer_range.resize(1);
    param_desc.integer_range[0].step = 1;
    param_desc.description = "0-RED, 1-BLUE, 2-AUTO";
    param_desc.integer_range[0].from_value = 0;
    param_desc.integer_range[0].to_value = 2;
    detect_color = this->declare_parameter("detect_color", 2, param_desc);
    RCLCPP_INFO(this->get_logger(), "detect_color: %d", detect_color);
    // Exposure time
    param_desc.description = "Exposure time in microseconds";
    MV_CC_GetFloatValue(camera_handle_, "ExposureTime", &f_value);
    param_desc.integer_range[0].from_value = f_value.fMin;
    param_desc.integer_range[0].to_value = f_value.fMax;
    exposure_time = this->declare_parameter("exposure_time", 5000, param_desc);
    MV_CC_SetFloatValue(camera_handle_, "ExposureTime", exposure_time);
    RCLCPP_INFO(this->get_logger(), "Exposure time: %d", exposure_time);

    // Gain
    param_desc.description = "Gain";
    MV_CC_GetFloatValue(camera_handle_, "Gain", &f_value);
    param_desc.integer_range[0].from_value = f_value.fMin;
    param_desc.integer_range[0].to_value = f_value.fMax;
    double gain = this->declare_parameter("gain", f_value.fCurValue, param_desc);
    MV_CC_SetFloatValue(camera_handle_, "Gain", gain);
    RCLCPP_INFO(this->get_logger(), "Gain: %f", gain);


    // //查看帧率
    // MVCC_FLOATVALUE frame_rate_info = {0};
    // nRet = MV_CC_GetFrameRate(camera_handle_, &frame_rate_info);
    // if (nRet == MV_OK) {
    //     std::cout << "Current Frame Rate: " << frame_rate_info.fCurValue << " FPS" << std::endl;
    //     std::cout << "Max Frame Rate: " << frame_rate_info.fMax << " FPS" << std::endl;
    //     std::cout << "Min Frame Rate: " << frame_rate_info.fMin << " FPS" << std::endl;
    // } else {
    //     std::cerr << "Failed to get frame rate! Error code: " << nRet << std::endl;
    // }


    // //查看相机宽高
    // MVCC_INTVALUE_EX nWidthValue = {0},nHeightValue = {0};	
    // nRet = MV_CC_GetIntValueEx(camera_handle_, "Width", &nWidthValue);
    // if (MV_OK != nRet)
    // {
    // 	printf("Get Width fail! nRet [0x%x]\n", nRet);
    // } else {
    //   std::cout << "Width: " << nWidthValue.nCurValue << std::endl;
    // }
	  // nRet = MV_CC_GetIntValueEx(camera_handle_, "Height", &nHeightValue);
    // if (MV_OK != nRet)
    // {
    //   printf("Get Height fail! nRet [0x%x]\n", nRet);
    // }else {
    //   std::cout << "Height: " << nHeightValue.nCurValue << std::endl;
    // }


    // //开关自动白平衡  0-关 1-开（无法在rqt实时调整）
	  // nRet = MV_CC_SetEnumValue(camera_handle_, "BalanceWhiteAuto", 1);
	  // if (MV_OK != nRet)
	  // {
		//   printf("Set BalanceWhiteAuto  fail! nRet [0x%x]\n", nRet);
	  // }
	  // //手动白平衡设置
	  // int BalanceRatio_Value[3]={1100,1024,1800};//R、G、B
	  // for (int i = 0; i < 3; i++)
	  // {
		//   nRet = MV_CC_SetEnumValue(camera_handle_, "BalanceRatioSelector", i);
		//   nRet = MV_CC_SetIntValue(camera_handle_, "BalanceRatio",BalanceRatio_Value[i]);
	  // }


    // XY翻转
    //nRet = MV_CC_SetBoolValue(camera_handle_, "ReverseX", 1);
	  // if (MV_OK != nRet)
	  // {
		//   printf("Set ReverseX fail! nRet [0x%x]\n", nRet);
	  // }

	  //nRet = MV_CC_SetBoolValue(camera_handle_, "ReverseY", 1);
	  // if (MV_OK != nRet)
	  // {
		//   printf("Set ReverseY fail! nRet [0x%x]\n", nRet);
	  // }

  }

  rcl_interfaces::msg::SetParametersResult parametersCallback(
    const std::vector<rclcpp::Parameter> & parameters)
  {
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    for (const auto & param : parameters) {
      if (param.get_name() == "exposure_time") {
        int status = MV_CC_SetFloatValue(camera_handle_, "ExposureTime", param.as_int());
        if (MV_OK != status) {
          result.successful = false;
          result.reason = "Failed to set exposure time, status = " + std::to_string(status);
        }
      } else if (param.get_name() == "gain") {
        int status = MV_CC_SetFloatValue(camera_handle_, "Gain", param.as_double());
        if (MV_OK != status) {
          result.successful = false;
          result.reason = "Failed to set gain, status = " + std::to_string(status);
        }
      } else {
        result.successful = false;
        result.reason = "Unknown parameter: " + param.get_name();
      }
    }
    return result;
  }

  void camGimbalFdbCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg)
  {
    if (msg != nullptr && rclcpp::ok()) 
    {      
      aiming_mode = msg->aiming_mode;
      if(aiming_mode == 0)
      {
          int enemy_color = msg->camp == 0 ? 1 : 0;

          if(enemy_color == 0)
          {
            int exp_time_red = get_parameter("exposure_time_red").as_int();

            if(exp_time_red != exposure_time) 
            {
              this->set_parameters({rclcpp::Parameter("exposure_time", exp_time_red)});
              exposure_time = exp_time_red;
              RCLCPP_INFO(this->get_logger(), "set_parameters r: = %d", exp_time_red);
            }
          }
          else
          {
            int exp_time_blue = get_parameter("exposure_time_blue").as_int();

            if(exp_time_blue != exposure_time) 
            {
              this->set_parameters({rclcpp::Parameter("exposure_time", exp_time_blue)});
              exposure_time = exp_time_blue;
              RCLCPP_INFO(this->get_logger(), "set_parameters b: = %d", exp_time_blue);
            }
          }    
      }
      
      if(aiming_mode == 1)
      {
        int exp_time_green = get_parameter("exposure_time_green").as_int();
        if(exp_time_green != exposure_time) 
        {
          this->set_parameters({rclcpp::Parameter("exposure_time", exp_time_green)});
          exposure_time = exp_time_green;
          RCLCPP_INFO(this->get_logger(), "set_parameters g: = %d", exp_time_green);
        }
      }
      
      
    }
    else RCLCPP_INFO(this->get_logger(), "CamGimbalFdb msg empty!!!");
  }
  

  void camera_reconnect()
  {
    // 1. 彻底释放旧资源
    MV_CC_StopGrabbing(camera_handle_);
    MV_CC_CloseDevice(camera_handle_);
    MV_CC_DestroyHandle(camera_handle_);
    camera_handle_ = nullptr;  // 必须置空！

    // 2. 重连逻辑（带详细日志）
    bool reconnect_success = false;
    for (int retry = 0; retry < 20; ++retry) {
        RCLCPP_INFO(this->get_logger(), "第 %d 次重试枚举设备...", retry + 1);

        // 2.1 每次创建新的 device_list
        MV_CC_DEVICE_INFO_LIST new_device_list = {0};
        int enum_ret = MV_CC_EnumDevices(MV_USB_DEVICE, &new_device_list);
        
        // 打印枚举结果
        RCLCPP_INFO(
            this->get_logger(), 
            "枚举结果: 错误码=0x%x, 设备数=%d", 
            enum_ret, 
            new_device_list.nDeviceNum
        );

        // 2.2 检查枚举是否成功
        if (enum_ret != MV_OK) {
            RCLCPP_WARN(this->get_logger(), "枚举失败，继续重试...");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        // 2.3 检查设备数量
        if (new_device_list.nDeviceNum == 0) {
            RCLCPP_WARN(this->get_logger(), "未找到设备，等待系统刷新...");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        // 2.4 尝试创建和打开设备
        for (unsigned int i = 0; i < new_device_list.nDeviceNum; ++i) {
            int create_ret = MV_CC_CreateHandle(&camera_handle_, new_device_list.pDeviceInfo[i]);
            if (create_ret != MV_OK) {
                RCLCPP_WARN(this->get_logger(), "创建句柄失败, 错误码=0x%x", create_ret);
                continue;
            }

            int open_ret = MV_CC_OpenDevice(camera_handle_);
            if (open_ret == MV_OK) {
                RCLCPP_INFO(this->get_logger(), "成功打开设备!");
                reconnect_success = true;
                break;
            } else {
                RCLCPP_WARN(this->get_logger(), "打开设备失败, 错误码=0x%x", open_ret);
                MV_CC_DestroyHandle(camera_handle_);
                camera_handle_ = nullptr;
            }
        }

        if (reconnect_success) break;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 3. 最终处理
    if (!reconnect_success) {
        RCLCPP_FATAL(this->get_logger(), "无法恢复连接，停止节点");
        rclcpp::shutdown();
        // break;
    }

    // 4. 重启采集
    MV_CC_StartGrabbing(camera_handle_);
    fail_conut_ = 0;
  }

  sensor_msgs::msg::Image image_msg_;

  image_transport::CameraPublisher camera_pub_A;
  image_transport::CameraPublisher camera_pub_green;

  int nRet = MV_OK;
  void * camera_handle_;
  MV_IMAGE_BASIC_INFO img_info_;
  int aiming_mode = 0;
  int exposure_time;
  int enemy_color = -1;
  int detect_color;

  MV_CC_PIXEL_CONVERT_PARAM convert_param_;

  std::string camera_name_;
  std::unique_ptr<camera_info_manager::CameraInfoManager> camera_info_manager_;
  sensor_msgs::msg::CameraInfo camera_info_msg_;

  int fail_conut_ = 0;
  std::thread capture_thread_;

  OnSetParametersCallbackHandle::SharedPtr params_callback_handle_;

    // GimbalFdb
  rclcpp::Subscription<auto_aim_interfaces::msg::GimbalFdb>::SharedPtr cam_gimbal_fdb_sub_;
};
}  // namespace hik_camera

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(hik_camera::HikCameraNodeA)
