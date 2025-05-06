#include "navi_cal/navi_cal.hpp"
namespace taurus
{
    Navi_cal::Navi_cal(const rclcpp::NodeOptions &options) : Node("Navi_cal",options)
    {   
        // RCLCPP_INFO(this->get_logger(),"Navi_cal");
        auto period = std::chrono::milliseconds(40);
        timer_ = this->create_wall_timer(period,std::bind(&Navi_cal::timerCallback,this));
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        transform_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        fdb_pub_ = this->create_publisher<auto_aim_interfaces::msg::Fdb>("/fdb", 10);
    }
    Navi_cal::~Navi_cal()
    {

    }
    /*
    @param s:m 距离
    @param z:m 高度差
    @param v:m/s
    @return angle_pitch:rad
    */
    float Navi_cal::pitchTrajectoryCompensation(float s, float z, float v)
    {
    //TODO:根据陀螺仪安装位置调整距离求解方式
        //降维，坐标系Y轴以垂直向上为正方向
        // auto dist_vertical = z + st.z_bias; //实际垂直距离 = z + 安装偏置
        auto dist_vertical = z + z_bias; //实际垂直距离 = z + 安装偏置
        auto vertical_tmp = dist_vertical;
        auto dist_horizonal = s + s_bias; //实际水平距离 = s - 安装偏置
        // auto dist_vertical = xyz[2];
        // auto dist_horizonal = sqrt(xyz.squaredNorm() - dist_vertical * dist_vertical);
        // t = (dist_horizonal/(v * cos(atan(dist_vertical / dist_horizonal)))); //初步计算飞行时间
        auto pitch = atan(dist_vertical / dist_horizonal) * 180 / PI; //初步计算飞行仰角
        auto pitch_new = pitch;
        //开始使用龙格库塔法求解弹道补偿
        for (int i = 0; i < 10; i++)
        {
            //TODO:可以考虑将迭代起点改为世界坐标系下的枪口位置
            //初始化
            auto x = 0.0;
            auto y = 0.0;
            auto p = tan(pitch_new / 180 * PI);
            auto u = v / sqrt(1 + pow(p,2));
            auto delta_x = dist_horizonal / R_K_iter;
            for (int j = 0; j < R_K_iter; j++)
            {
                auto k1_u = -k * u * sqrt(1 + pow(p, 2));
                auto k1_p = -GRAVITY / pow(u, 2);
                auto k1_u_sum = u + k1_u * (delta_x / 2);
                auto k1_p_sum = p + k1_p * (delta_x / 2);

                auto k2_u = -k * k1_u_sum * sqrt(1 + pow(k1_p_sum, 2));
                auto k2_p = -GRAVITY / pow(k1_u_sum, 2);
                auto k2_u_sum = u + k2_u * (delta_x / 2);
                auto k2_p_sum = p + k2_p * (delta_x / 2);

                auto k3_u = -k * k2_u_sum * sqrt(1 + pow(k2_p_sum, 2));
                auto k3_p = -GRAVITY / pow(k2_u_sum, 2);
                auto k3_u_sum = u + k3_u * (delta_x / 2);
                auto k3_p_sum = p + k3_p * (delta_x / 2);

                auto k4_u = -k * k3_u_sum * sqrt(1 + pow(k3_p_sum, 2));
                auto k4_p = -GRAVITY / pow(k3_u_sum, 2);
                
                u += (delta_x / 6) * (k1_u + 2 * k2_u + 2 * k3_u + k4_u);
                p += (delta_x / 6) * (k1_p + 2 * k2_p + 2 * k3_p + k4_p);
                
                x+=delta_x;
                y+=p * delta_x;
            }
            //评估迭代结果,若小于迭代精度需求则停止迭代
            auto error = dist_vertical - y;
            if (abs(error) <= 0.0005)
            {
                break;
            }
            else 
            {
                vertical_tmp+=error;
                // xyz_tmp[1] -= error;
                pitch_new = atan(vertical_tmp / dist_horizonal) * 180 / PI;
            }
        }

        return pitch_new  * PI /180;
    }
    void Navi_cal::timerCallback()
    {
        RCLCPP_INFO(this->get_logger(),"timerCallback");
        try{
            geometry_msgs::msg::TransformStamped transform = tf_buffer_->lookupTransform("map","lidar_link",tf2::TimePointZero);
            robot_pose_.position.x = transform.transform.translation.x;
            robot_pose_.position.y = transform.transform.translation.y;
            robot_pose_.position.z = transform.transform.translation.z;
            tf2::Quaternion q(
                transform.transform.rotation.x,
                transform.transform.rotation.y,
                transform.transform.rotation.z,
                transform.transform.rotation.w
            );
            tf2::Matrix3x3 m(q);
            m.getRPY(robot_roll_,robot_pitch_,robot_yaw_);
            RCLCPP_INFO(this->get_logger(),"Current yaw: %f, Current x: %f, Current y: %f,Current z: %f,",robot_yaw_,robot_pose_.position.x,robot_pose_.position.y,robot_pose_.position.z);
        }catch(tf2::TransformException  &ex){
            RCLCPP_ERROR(this->get_logger(),"Failure %s",ex.what());
        }
        // fdb_msg_.yaw = 1;
        fdb_msg_.pitch = 2;
        double map_yaw_ = atan2((target_y_ - robot_pose_.position.y),(target_x_ - robot_pose_.position.x)) * 180 /3.14159;
        RCLCPP_INFO(this->get_logger(),"map_yaw_ :%f,",map_yaw_);
        robot_yaw_ = robot_yaw_ * 180 /3.14159;
        RCLCPP_INFO(this->get_logger(),"robot_yaw_ :%f,",robot_yaw_);
        // fdb_msg_.yaw = robot_yaw_ + ;
        double result_yaw_ = map_yaw_ - robot_yaw_;
        RCLCPP_INFO(this->get_logger(),"result_yaw_ :%f,",result_yaw_);
        fdb_msg_.yaw = result_yaw_;
        double target_dis_ = sqrt((target_y_ - robot_pose_.position.y) * (target_y_ - robot_pose_.position.y)+(target_x_ - robot_pose_.position.x) * (target_x_ - robot_pose_.position.x));
        double result_pit_ = pitchTrajectoryCompensation(target_dis_,(target_z_ - robot_pose_.position.z - 0.15),15.8)* 180 /3.14159;
        fdb_msg_.pitch = result_pit_;
        RCLCPP_INFO(this->get_logger(),"result_pit_ :%f,",result_pit_);
        fdb_pub_->publish(fdb_msg_);
    }
}// namespace taurus2weee3
#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(taurus::Navi_cal);