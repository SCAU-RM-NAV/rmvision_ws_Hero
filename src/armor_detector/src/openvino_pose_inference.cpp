#include "armor_detector/openvino_pose_inference.hpp"
#include "armor_detector/detector.hpp"
#include "armor_detector/armor.hpp"
#include <memory>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <random>
#include <future>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include "auto_aim_interfaces/msg/detections.hpp"
#include "auto_aim_interfaces/msg/detection.hpp"

#include "auto_aim_interfaces/msg/key_point_and_float.hpp"
#include "auto_aim_interfaces/msg/key_point2_f.hpp"
#include <filesystem>
// 设置线程亲和性
#include <pthread.h>
#include <sched.h>


void set_cpu_affinity(const std::vector<int>& cpus) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (int cpu : cpus) {
        CPU_SET(cpu, &cpuset);
    }
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

namespace rm_auto_aim
{
	// Constructor to initialize the model with default input shape
	Inference::Inference(const std::string &model_path, const float &model_confidence_threshold, const float &model_NMS_threshold)
	{
		model_input_shape_ = cv::Size(640, 640); // Set the default size for models with dynamic shapes to prevent errors.
		model_confidence_threshold_ = model_confidence_threshold;
		model_NMS_threshold_ = model_NMS_threshold;
		InitializeModel(model_path);
	}

	// Constructor to initialize the model with specified input shape
	Inference::Inference(const std::string &model_path, const cv::Size model_input_shape, const float &model_confidence_threshold, const float &model_NMS_threshold)
	{

		model_input_shape_ = model_input_shape;
		model_confidence_threshold_ = model_confidence_threshold;
		model_NMS_threshold_ = model_NMS_threshold;
		InitializeModel(model_path);
	}

		Inference::Inference(const std::string &model_path, const cv::Size model_input_shape, const float &model_confidence_threshold, const float &model_NMS_threshold, rclcpp::Publisher<auto_aim_interfaces::msg::Detections>::SharedPtr &detections_pub_)
	{
		detections_pub=detections_pub_;
		model_input_shape_ = model_input_shape;
		model_confidence_threshold_ = model_confidence_threshold;
		model_NMS_threshold_ = model_NMS_threshold;
		InitializeModel(model_path);
	}

	// Constructor to initialize the model with specified input shape
	Inference::Inference(const std::string &model_path, const cv::Size model_input_shape, const float &model_confidence_threshold, const float &model_NMS_threshold, std::string &driver_, int &num_requests_)
	{
		num_requests = num_requests_;
		driver = driver_;
		model_input_shape_ = model_input_shape;
		model_confidence_threshold_ = model_confidence_threshold;
		model_NMS_threshold_ = model_NMS_threshold;

		InitializeModel(model_path);
	}

	Inference::Inference(const std::string &model_path, const cv::Size model_input_shape, const float &model_confidence_threshold, const float &model_NMS_threshold, std::string &driver_, rclcpp::Publisher<auto_aim_interfaces::msg::Detections>::SharedPtr &detections_pub_,  std::shared_ptr<Detector> detector)
	{
		driver = driver_;
		detections_pub=detections_pub_;
		detector_ = detector; // 显式移动赋值
		model_input_shape_ = model_input_shape;
		model_confidence_threshold_ = model_confidence_threshold;
		model_NMS_threshold_ = model_NMS_threshold;

		InitializeModel(model_path);
	}

	void Inference::InitializeModel(const std::string &model_path)
	{
		ov::Core core;													// OpenVINO core object
		std::shared_ptr<ov::Model> model = core.read_model(model_path); // Read the model from file

		// If the model has dynamic shapes, reshape it to the specified input shape
		if (model->is_dynamic())
		{
			model->reshape({1, 3, static_cast<long int>(model_input_shape_.height), static_cast<long int>(model_input_shape_.width)});
		}

		// Preprocessing setup for the model
		ov::preprocess::PrePostProcessor ppp = ov::preprocess::PrePostProcessor(model);
		ppp.input().tensor().set_element_type(ov::element::u8).set_layout("NHWC").set_color_format(ov::preprocess::ColorFormat::BGR);
		ppp.input().preprocess().convert_element_type(ov::element::f32).convert_color(ov::preprocess::ColorFormat::RGB).scale({255, 255, 255});
		ppp.input().model().set_layout("NCHW");
		ppp.output().tensor().set_element_type(ov::element::f32);
		model = ppp.build(); // Build the preprocessed model

		// Compile the model for inference
		// 检查 GPU 属性

		// compiled_model_ = core.compile_model(model, "AUTO");
		compiled_model_ = core.compile_model(model, driver,
											 ov::hint::performance_mode(ov::hint::PerformanceMode::LATENCY) // THROUGHPUT LATENCY
																											   //,ov::hint::model_priority(ov::hint::Priority::MEDIUM)
		);
		// compiled_model_ = core.compile_model(model, "GPU", ov::hint::performance_mode(ov::hint::PerformanceMode::LATENCY));
		// Ainference_request_ = compiled_model_.create_infer_request();
		// Binference_request_ = compiled_model_.create_infer_request();
		//  初始化多个推理请求

		// 根据硬件调整数量 20-29hz 4-28.68hz 12-29hz
		std::cout << "num_requests==" << num_requests << std::endl;
		for (int i = 0; i < (num_requests); ++i)
		{
			//inference_requests_.push_back(compiled_model_.create_infer_request());
			flages.push_back(true);
			counts.push_back(0);
			headers.push_back(std_msgs::msg::Header());
			temp_headers.push_back(std_msgs::msg::Header());

			my_inputs.push_back(std::make_shared<cv::Mat>(cv::Mat()));

			imu_to_camera_s.push_back(Eigen::Matrix3d());
			temp_imu_to_camera_s.push_back(Eigen::Matrix3d());
			compiled_models.push_back(core.compile_model(model, driver,
			ov::hint::performance_mode(ov::hint::PerformanceMode::LATENCY) // THROUGHPUT LATENCY
			));
			inference_requests_.push_back(compiled_models[i].create_infer_request());
		}


		//inference_requests_.push_back(compiled_model.create_infer_request());
		

		short width, height;

		// Get input shape from the model
		const std::vector<ov::Output<ov::Node>> inputs = model->inputs();
		const ov::Shape input_shape = inputs[0].get_shape();
		height = input_shape[1];
		width = input_shape[2];
		model_input_shape_ = cv::Size2f(width, height);

		// Get output shape from the model
		const std::vector<ov::Output<ov::Node>> outputs = model->outputs();
		const ov::Shape output_shape = outputs[0].get_shape();
		height = output_shape[1];
		width = output_shape[2];
		model_output_shape_ = cv::Size(width, height);

		    // 


	}

	bool Inference::Pose_Run_async_Inference(cv::Mat &frame,std_msgs::msg::Header header,Eigen::Matrix3d imu_to_camera_)
	{
			//std::cout<<" Pose_Run_async_Inference"<<" ";
		//int id=i;
		std::lock_guard<std::mutex> lock(flage_mutex);
		for(int id =0;id< num_requests;id++)
		{
		if ((flages[id])  && (!frame.empty()) )
		{	

			flages[id] = false;
			// int id =run_id;
			imu_to_camera_s[id]=imu_to_camera_;
			headers[id] = header;
			//inputs[id]=frame;

			//run_id=(id+1)%num_requests;
			// run_id.store((id + 1) % num_requests);  // 原子更新

			auto frame_ptr = std::make_shared<cv::Mat>(frame);
			
			std::thread([frame_ptr,this,id]() {
			auto s = std::chrono::high_resolution_clock::now();
			set_cpu_affinity(cpu_cores); //🔓核心

			Preprocessing(*frame_ptr,id);
			this->inference_requests_[id].infer();

			auto hs = std::chrono::high_resolution_clock::now();
			Pose_PostProcessing(*frame_ptr,id);
			auto he = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> Hdiff = he - hs;

			huamianshu++;

			auto e = std::chrono::high_resolution_clock::now();
			// 计算时间差（直接转换为浮点毫秒）
			std::chrono::duration<double, std::milli> diff = e - s;
			// 输出结果（保留3位小数）
			std::cout << "总延迟 Time: " 
					<< std::fixed 
					<< std::setprecision(3) 
					<< diff.count() 
					<< " ms" 
					<< "其中后处理延迟_Time: " 
					<< std::fixed 
					<< std::setprecision(3) 
					<< Hdiff.count() 
					<< " ms" 
					<< std::endl;

			 }).detach();

			auto& header = headers[id];
			auto sec = header.stamp.sec;
			auto nanosec = header.stamp.nanosec;
			std::cout<<" 输入图片 时间辍:"<<sec<<"."<<nanosec<<std::endl;

			return true;
		}
		}
			return false;
		
	}

	// Method to preprocess the input frame
	void Inference::Preprocessing(const cv::Mat &frame,int id)
	{
		//std::cout<<" 预处理开始 ";
		cv::Mat resized_frame;
		resized_frame.create(model_input_shape_, frame.type());
		
/* 		std::cout<<" model_input_shape_"<<model_input_shape_<<std::endl;
		std::cout<<" frame.type()"<<frame.type()<<std::endl; */

		cv::resize(frame, resized_frame, model_input_shape_, 0, 0, cv::INTER_LINEAR); // Resize the frame to match the model input shape
		//cv::INTER_LINEAR cv::INTER_AREA cv::INTER_CUBIC INTER_NEAREST

		// Calculate scaling factor
		scale_factor_.x = static_cast<float>(frame.cols / model_input_shape_.width);
		scale_factor_.y = static_cast<float>(frame.rows / model_input_shape_.height);

		float *input_data = (float *)resized_frame.data;																						 // Get pointer to resized frame data
		const ov::Tensor input_tensor = ov::Tensor(compiled_models[id].input().get_element_type(), compiled_models[id].input().get_shape(), input_data); // Create input tensor

		inference_requests_[id].set_input_tensor(input_tensor);		 // Set input tensor for inference
		//std::cout<<" 预处理成功 ";															
	}

	// Method to postprocess the inference results
	void Inference::Pose_PostProcessing(cv::Mat &frame, int i)
	{
	auto hs = std::chrono::high_resolution_clock::now();

		const float *detections = inference_requests_[i].get_output_tensor().data<const float>(); // 赶紧用掉inference_request解锁
		temp_imu_to_camera_s[i]=imu_to_camera_s[i];
		temp_headers[i]=headers[i];
		//flages[i]=true;

		std::vector<int> class_list;
		std::vector<float> confidence_list;
		std::vector<cv::Rect> box_list;
		std::vector<Key_PointAndFloat> key_list;
		// Get the output tensor from the inference request

		
		const cv::Mat detection_outputs(model_output_shape_, CV_32F, (float *)detections); // Create OpenCV matrix from output tensor
		//std::cout<<" Pose_PostProcessing flages[i]=true前的id="<<i<<std::endl;


		int labels_size = 2;
		int labels__zhanwei = 4 + labels_size;
		int dianshu=int(detection_outputs.rows - labels__zhanwei)/3; //4
		for (int i = 0; i < detection_outputs.cols; ++i)
		{
			const cv::Mat classes_scores = detection_outputs.col(i).rowRange(4, labels__zhanwei); // 现在是两类

			cv::Point class_id;
			double score;
			double key_point_score=0.0;
			cv::minMaxLoc(classes_scores, nullptr, &score, nullptr, &class_id); // Find the class with the highest score

			// Check if the detection meets the confidence threshold
			if (score > 0.9)
			{
				//计算key点置信度
				double temp_score=0.0;
				for(int j=0;j<dianshu; j++)
				{
					temp_score += detection_outputs.at<float>(labels__zhanwei + 2 + 3 * j, i);
				}
				key_point_score = temp_score / dianshu;

				if(key_point_score > model_confidence_threshold_)
				{

				class_list.push_back(class_id.y);
				confidence_list.push_back(score);

				const float x = detection_outputs.at<float>(0, i);
				const float y = detection_outputs.at<float>(1, i);
				const float w = detection_outputs.at<float>(2, i);
				const float h = detection_outputs.at<float>(3, i);

				Key_PointAndFloat paf;
				for (int j = 0; j < dianshu; j++)
				{
					paf.key_point.push_back(cv::Point(detection_outputs.at<float>(labels__zhanwei  + 3 * j, i), detection_outputs.at<float>(labels__zhanwei + 1 + 3 * j, i)));
					paf.value.push_back(detection_outputs.at<float>(labels__zhanwei + 2 + 3 * j, i));
				}
				// std::cout<<paf.key_point[0]<<std::endl;
				key_list.push_back(paf);

				cv::Rect box;
				box.x = static_cast<int>(x);
				box.y = static_cast<int>(y);
				box.width = static_cast<int>(w);
				box.height = static_cast<int>(h);
				box_list.push_back(box);
				}
			}
		}
		// std::cout << "The  detection_outputs  "  << ":\n" << detection_outputs<< std::endl;
		//  Apply Non-Maximum Suppression (NMS) to filter overlapping bounding boxes
		std::vector<int> NMS_result;
		cv::dnn::NMSBoxes(box_list, confidence_list, model_confidence_threshold_, model_NMS_threshold_, NMS_result);

  // Collect final detections after NMS
  my_Detection result;
  std::vector<my_Detection> results;//要存队列

   
  for (std::vector<int>::size_type i = 0; i < NMS_result.size(); ++i) {
    const unsigned short id = NMS_result[i];
    result.class_id = class_list[id];
/*     result.confidence = confidence_list[id];
    result.box = GetBoundingBox(box_list[id]); */
    result.Key_Point = GetKeyPointsinBox(key_list[id]);
    results.emplace_back(result);
   // std::cout<<" 有目标!!!!!!!!!!!!!!";
  }
/*    if( NMS_result.size()==0)
   {
    // 获取时间戳的两个部分
   auto& header = temp_headers[i];
	auto sec = header.stamp.sec;
	auto nanosec = header.stamp.nanosec;
	std::cout<<" 无目标 后处理时间辍:"<<sec<<"."<<nanosec<<std::endl;
   }
   else
   {
	    // 获取时间戳的两个部分
   auto& header = temp_headers[i];
	auto sec = header.stamp.sec;
	auto nanosec = header.stamp.nanosec;
	std::cout<<" 有目标 后处理时间辍:"<<sec<<"."<<nanosec<<std::endl;
   } */


    // if( NMS_result.size()==0)
    // {
    //     std::cout<<" 无目标!!!!!!!!!!!!!!";
    //     // 通过时间戳命名保存图片
    //     auto& header = temp_headers[i];
    //     std::string timestamp = std::to_string(header.stamp.sec) + "_" + std::to_string(header.stamp.nanosec);
    //     std::string directory = "saved_images";
    //     if (!std::filesystem::exists(directory)) {
    //         std::filesystem::create_directories(directory);
    //     }
    //     std::string filename = directory + "/" + timestamp + ".jpg";
    //     cv::imwrite(filename, frame);
    //     std::cout << " 图片已保存为: " << filename;
    // } 


	//detector_->pose_detect(results,)
	my_inputs[i]=std::make_shared<cv::Mat>(frame);
	pose_publish_detections(results,i);
	//std::cout<<" 后处理成功 ";	

	auto he = std::chrono::high_resolution_clock::now();
	auto Hdiff= std::chrono::duration_cast<std::chrono::milliseconds>(he - hs);
	//std::cout<<" Hdiff"<<Hdiff.count()<<" ";
}

void Inference::pose_publish_detections(std::vector<my_Detection> &results_,int id) 
{
	
    // 创建ROS消息
    auto_aim_interfaces::msg::Detections detections_msg;

	for (const my_Detection& detection : results_)
	{
	auto_aim_interfaces::msg::Detection msg_detection;
	msg_detection.class_id=detection.class_id;
 	

	// 假设 detection 是一个包含 Key_Point 和字段的对象
	auto_aim_interfaces::msg::KeyPointAndFloat key_point_and_float;

	for (const auto& kp : detection.Key_Point.key_point) {
		auto_aim_interfaces::msg::KeyPoint2F key_point2_f;
		key_point2_f.x = kp.x;
		key_point2_f.y = kp.y;

		key_point_and_float.key_point.push_back(key_point2_f); // 修正：添加 key_point2_f
	}

	msg_detection.key_point = key_point_and_float;
	detections_msg.detections.push_back(msg_detection);
	}

	for (int i = 0; i < 3; i++) {
	for (int j = 0; j < 3; j++) {
		detections_msg.imu_to_camera[i * 3 + j] = temp_imu_to_camera_s[id](i, j);
	}
	}

    // 发布消息
	detections_msg.inif_id=id;
	detections_msg.my_id=inference_id;
	detections_msg.header=temp_headers[id];
	//std::cout<<" 发布消息的id="<<id<<std::endl;
	detections_pub->publish(detections_msg); 
	//flages[id]=true;
	//std::cout<<" 发布_over ";
}



	Key_PointAndFloat Inference::GetKeyPointsinBox(Key_PointAndFloat &Key)
	{

		for (size_t i = 0; i < Key.key_point.size(); i++)
		{
			Key.key_point[i].x = Key.key_point[i].x * scale_factor_.x;
			Key.key_point[i].y = Key.key_point[i].y * scale_factor_.y;
		}
		return Key;
	}

	// Method to get the bounding box in the correct scale
	cv::Rect Inference::GetBoundingBox(const cv::Rect &src) const
	{
		cv::Rect box = src;
		box.x = (box.x - box.width / 2) * scale_factor_.x;
		box.y = (box.y - box.height / 2) * scale_factor_.y;
		box.width *= scale_factor_.x;
		box.height *= scale_factor_.y;
		return box;
	}


}  // namespace rm_auto_aim