#include "armor_tracker/solve_trajectory.hpp"

namespace rm_auto_aim
{

SolveTrajectory::SolveTrajectory(rclcpp::Time time): Node("solve_trajectory_node") 
{
    gimbal_fdb_sub_ = this->create_subscription<auto_aim_interfaces::msg::GimbalFdb>(
  "gimbal_fdb", rclcpp::SensorDataQoS(),std::bind(&SolveTrajectory::gimbalCallback, this, std::placeholders::_1));


  //初始化参数
  st.k = 0.000556;
  // st.k = 0.01903;                //25°C,1atm,小弹丸
    // st.k = 0.000556;                //25°C,1atm,大弹丸
    // st.k = 0.000530;                //25°C,1atm,发光大弹丸
  //   st.bullet_type =  BULLET_17;
  //   st.current_v = 30;
  st.current_pitch = 0;
  st.current_yaw = 0;

  //   st.bias_time = 200;
  //   st.s_bias = 0.19133;
  //   st.z_bias = 0.0;

  // 初始化整车状态变量
  st.xw = 3.0;
  st.yw = 0;
  st.zw = 1.5;

  st.vxw = 0;
  st.vyw = 0;
  st.vzw = 0;
  st.v_yaw = 0;
  st.tar_yaw = 0;

  st.r1 = 0.5;
  st.r2 = 0.5;
  st.dz = 0.1;

  st.armor_id = ARMOR_INFANTRY3;
  st.armor_num = ARMOR_NUM_NORMAL;

  st.armor_jump_thre = 200;

  last_armor_jump_time = time;
}
  
  
  void SolveTrajectory::gimbalCallback(const auto_aim_interfaces::msg::GimbalFdb::SharedPtr msg)
  {
      aiming_mode = msg->aiming_mode;
  }



/*
@brief 根据最优决策得出被击打装甲板 自动解算弹道
@param pitch:rad  传出pitch
@param yaw:rad    传出yaw
@param aim_x:传出aim_x  打击目标的x
@param aim_y:传出aim_y  打击目标的y
@param aim_z:传出aim_z  打击目标的z
*/
/**
 * @brief 预测与弹道解算
 * @param target_msg 用来预测和解算的信息
 * @param calculate_delay 处理延迟，用于预测时间的补偿
 */

void SolveTrajectory::autoSolveTrajectory(const auto_aim_interfaces::msg::Target& target_msg, const double calculate_delay)
{
  //std::cout<<"yaw"<<yaw<<std::endl;
  // 小陀螺tracker的相关参数
  st.r1 = target_msg.radius_1;
  st.r2 = target_msg.radius_2;

  st.xw = target_msg.position.x;
  st.yw = target_msg.position.y;
  st.zw = target_msg.position.z;

  st.vxw = target_msg.velocity.x;
  st.vyw = target_msg.velocity.y;
  st.vzw = target_msg.velocity.z;

  st.v_yaw = target_msg.v_yaw;
  st.tar_yaw = target_msg.yaw;
  st.dz = target_msg.dz;

  // 平移tracker的相关参数
  st.axw = target_msg.armor_position.x;
  st.ayw = target_msg.armor_position.y;
  st.azw = target_msg.armor_position.z;

  st.avxw = target_msg.armor_velocity.x;
  st.avyw = target_msg.armor_velocity.y;
  st.avzw = target_msg.armor_velocity.z;

  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "当前追踪目标的装甲板id: " << target_msg.id);

  if (!target_msg.id.empty() && target_msg.id != "0"){
    st.armor_id = static_cast<ARMOR_ID>(std::stoi(target_msg.id));
  }
    
  st.armor_num = static_cast<ARMOR_NUM>(target_msg.armors_num);

  //当前pitch、yaw
  st.current_pitch = 0;
  st.current_yaw = 0;

  // 线性预测时间
  // 模式选择 当装甲板的角速度vyaw小于 2rad/s 时，使用平移tracker的数据计算
  float timeDelay;
  if (abs(st.v_yaw) < 2) {
    timeDelay = st.bias_time_com / 1000.0 + t + calculate_delay / 1000.0;
  }
  // 否则使用小陀螺tracker的数据计算
  else{
    timeDelay = st.bias_time / 1000.0 + t + calculate_delay / 1000.0;
  }
  // 异常预测时间处理
  timeDelay = (timeDelay < 1000 || !std::isnan(timeDelay)) ? timeDelay : 1000;
  if (timeDelay == 1000){
    RCLCPP_WARN_STREAM(rclcpp::get_logger("armor_tracker"), "预测时间超过了1s!");
  }
  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "fly time: " << t);

  // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "st.tar_yaw: " << st.tar_yaw);
  st.tar_yaw = angles::normalize_angle(st.tar_yaw + st.v_yaw * timeDelay);

  // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "predict st.tar_yaw: " << st.tar_yaw);

  //计算四块装甲板的位置
  //装甲板id顺序，以四块装甲板为例，逆时针编号
  //      2
  //   3     1
  //      0
  int use_1 = 1;
  int i = 0;
  int idx = 0;  // 选择的装甲板
  // armor_num = ARMOR_NUM_BALANCE 为平衡步兵
  if (st.armor_num == ARMOR_NUM_BALANCE) {//无平衡步兵 已废用
    for (i = 0; i < 2; i++) {
      float tmp_yaw = angles::normalize_angle(st.tar_yaw + i * M_PI);
      float r = st.r1;
      tar_position[i].x = st.xw - r * cos(tmp_yaw);
      tar_position[i].y = st.yw - r * sin(tmp_yaw);
      tar_position[i].z = st.zw;
      tar_position[i].yaw = tmp_yaw;
    }

    float yaw_diff_min = fabsf(yaw - tar_position[0].yaw);

    //因为是平衡步兵 只需判断两块装甲板即可
    float temp_yaw_diff = abs(angles::shortest_angular_distance(yaw, tar_position[1].yaw));
    // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker")," yaw_diff_0: " << yaw_diff_min << " yaw_diff_1"<< temp_yaw_diff << std::endl);
    if (temp_yaw_diff < yaw_diff_min) {
      yaw_diff_min = temp_yaw_diff;
      idx = 1;
    }

    //float armor_yaw_diff_score = abs(angles::shortest_angular_distance(yaw, tar_position[idx].yaw));
    //if(armor_yaw_diff_score>0.785)
    //{
    //  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), armor_yaw_diff_score);
    //  flag = true;
    //}
    RCLCPP_DEBUG_STREAM(
      rclcpp::get_logger("armor_tracker"),
      "selsct id: " << idx << " yaw_diff_min: " << yaw_diff_min);
  } else if (st.armor_num == ARMOR_NUM_OUTPOST) {  //前哨站
    for (i = 0; i < 3; i++) {
      float tmp_yaw = angles::normalize_angle(st.tar_yaw + i * 2.0 * M_PI / 3.0);  // 2/3PI
      float r = (st.r1 + st.r2) / 2;  //理论上r1=r2 这里取个平均值
      tar_position[i].x = st.xw - r * cos(tmp_yaw);
      tar_position[i].y = st.yw - r * sin(tmp_yaw);
      tar_position[i].z = st.zw;
      tar_position[i].yaw = tmp_yaw;
    }

    // TODO 选择最优装甲板 选板逻辑你们自己写，这个一般给英雄用
    auto get_score = [this](tar_pos tar_pos) {  //tar_pos包含装甲板的xyz和yaw
      //计算传入装甲板与当前枪管yaw的差值
      float temp_gimbal_yaw = (float)(atan2(tar_pos.y, tar_pos.x));
      float gimbal_yaw_diff = abs(angles::shortest_angular_distance(yaw, temp_gimbal_yaw));

      gimbal_yaw_diff = gimbal_yaw_diff < M_PI * 2 / 3 ? gimbal_yaw_diff : 0;
      //计算上一帧枪管到当前目标装甲板yaw
      float armor_yaw_diff = abs(angles::shortest_angular_distance(yaw, tar_pos.yaw));

      return gimbal_yaw_diff + armor_yaw_diff;  //？？返回二者之和？？？
      // return armor_yaw_diff;
    };

    //计算枪管到目标装甲板yaw最小的那个装甲板
    float yaw_diff_min = get_score(tar_position[0]);
    for (i = 1; i < 3; i++) {
      float temp_yaw_diff = get_score(tar_position[i]);
      if (temp_yaw_diff < yaw_diff_min) {
        yaw_diff_min = temp_yaw_diff;
        idx = i;
      }
    }
    RCLCPP_DEBUG_STREAM(
      rclcpp::get_logger("armor_tracker"),
      "selsct id: " << idx << " yaw_diff_min: " << yaw_diff_min);
  } else  // 四块装甲板目标
  {
    for (i = 0; i < 4; i++) {
      float tmp_yaw = angles::normalize_angle(st.tar_yaw + i * M_PI / 2.0);
      float r = use_1 ? st.r1 : st.r2;
      tar_position[i].x = st.xw - r * cos(tmp_yaw);
      tar_position[i].y = st.yw - r * sin(tmp_yaw);
      tar_position[i].z = use_1 ? st.zw : st.zw + st.dz;
      tar_position[i].yaw = tmp_yaw;
      use_1 = !use_1;

      // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "armor i: " << i << " tmp_yaw: " << tmp_yaw << " r: " << r);
    }
    // 2种常见决策方案：
    // 1.计算枪管到目标装甲板yaw最小的那个装甲板
    // 2.计算距离最近的装甲板

    // //计算距离最近的装甲板
    // float dis_diff_min = sqrt(tar_position[0].x * tar_position[0].x + tar_position[0].y * tar_position[0].y);
    // // int idx = 0;
    // for (i = 1; i < 4; i++)
    // {
    //   float temp_dis_diff = sqrt(tar_position[i].x * tar_position[0].x + tar_position[i].y * tar_position[0].y);
    //   if (temp_dis_diff < dis_diff_min)
    //   {
    //     dis_diff_min = temp_dis_diff;
    //     idx = i;
    //   }
    //   // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "armor i: " << i << " dis_diff: " << temp_dis_diff);
    // }

    // 装甲板选择策略，评分
    auto get_score = [this](tar_pos tar_pos) {
      //计算上一帧枪管到当前帧枪管yaw
      float temp_gimbal_yaw = (float)(atan2(tar_pos.y, tar_pos.x));
      float gimbal_yaw_diff = abs(angles::shortest_angular_distance(yaw, temp_gimbal_yaw));
      gimbal_yaw_diff = gimbal_yaw_diff < M_PI / 2 ? gimbal_yaw_diff : 0;
      //计算上一帧枪管到当前目标装甲板yaw
      float armor_yaw_diff = abs(angles::shortest_angular_distance(yaw, tar_pos.yaw));

      return gimbal_yaw_diff + armor_yaw_diff;
      // return armor_yaw_diff;
    };

    //计算枪管到目标装甲板yaw最小的那个装甲板
    float yaw_diff_min = get_score(tar_position[0]);
    for (i = 1; i < 4; i++) {
      float temp_yaw_diff = get_score(tar_position[i]);
      if (temp_yaw_diff < yaw_diff_min) {
        yaw_diff_min = temp_yaw_diff;
        idx = i;
      }
    }
    RCLCPP_DEBUG_STREAM(
      rclcpp::get_logger("armor_tracker"),
      "selsct id: " << idx << " yaw_diff_min: " << yaw_diff_min);

    // rclcpp::Time now = target_msg.header.stamp;
    // auto jump_latency = (now -last_armor_jump_time).seconds() * 1000;
    // RCLCPP_INFO_STREAM(rclcpp::get_logger("armor_tracker"), "armor jump, jump_latency: " << jump_latency << "ms");

    // // 切装甲板限制
    // if (idx != last_idx)
    // {
    //   // 切装甲板时间小于阈值，则不切换目标装甲板
    //   rclcpp::Time now = target_msg.header.stamp;
    //   auto jump_latency = (now -last_armor_jump_time).seconds() * 1000;
    //   RCLCPP_INFO_STREAM(rclcpp::get_logger("armor_tracker"), "armor jump, jump_latency: " << jump_latency << "ms");
    //   if (jump_latency < st.armor_jump_thre && target_msg.armor_jump == false)
    //   {
    //     idx = last_idx;
    //   }
    //   else
    //   {
    //     // 更新切装甲板变量
    //     last_armor_jump_time = target_msg.header.stamp;
    //     last_idx = idx;
    //   }
    // }
  }

  // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "position: "
  //                                                           << " x: " << tar_position[idx].x << " y: "
  //                                                           << tar_position[idx].y << " z: " << tar_position[idx].z);

  // 模式选择 当vyaw小于 2rad/s 时，使用普通跟踪器，否则使用整车状态跟踪器
  float vyaw = 0.9 * st.v_yaw + (1 - 0.9) * last_vyaw;
  last_vyaw = vyaw;

  //52201
  // into spin mode when spin_count_ > 100
  if (abs(vyaw) > 2 && spin_count_ < 200) {
    spin_count_++;
  } else if (abs(vyaw) < 2 && spin_count_ > 0) {
    spin_count_ -= 10;
  }


  is_spin = spin_count_ > 70 ? true : false;  //判断是否旋转以防观测误判在小陀螺
  RCLCPP_INFO_STREAM(
    rclcpp::get_logger("armor_tracker"), "solove_trajectory spin_count_: " << spin_count_);

  // 模式选择 当vyaw小于 2rad/s 时，使用普通跟踪器，否则使用整车状态跟踪器
  if (!is_spin && target_msg.id != "7")  //用com的估计的armor进行线性预测
  {
    aim_x = tar_position[idx].x + st.avxw * timeDelay;
    aim_y = tar_position[idx].y + st.avyw * timeDelay;
    aim_z = tar_position[idx].z + st.avzw * timeDelay;
    std::cout<<"111111!!!!!!!!!!!!!!!!!!!"<<std::endl;
    fire_flag = 1;
    //RCLCPP_INFO(rclcpp::get_logger("armor_tracker"),"USING Common Tracker!");
  } else {  // spin 估计的armor进行线性预测  前哨只会进这个

    selective_hitting_yaw =
      abs(angles::shortest_angular_distance(yaw, tar_position[idx].yaw)) * 180 / 3.14;
    linear_v = sqrt((st.vxw) * (st.vxw) + (st.vyw) * (st.vyw));//平面线速度
    aim_distance = sqrt(
      (tar_position[idx].x) * (tar_position[idx].x) +
      (tar_position[idx].y) * (tar_position[idx].y));//击打目标距离发射器的距离

    //对转速和平移速度做判断
    if (is_infantry) {//判断是否为步兵
      if (linear_v > linear_v_max || abs(st.vyw) > 0.3) {                 //平移状态下判断
          aim_center = 1;
      } else {  //只有小陀螺没有快速平移
          if (abs(vyaw) > 7.0) {
            aim_center = 1;
            std::cout<<"1111111111:"<<std::endl;
          } else {  //正常距离的慢速小陀螺
            if (aim_distance < 1.8)//近距离
            {
              aim_center = 0;
              shoot_yaw =28.0;
              control_diff_mode = 1;
            }else{//远距离
              aim_center = 0;
              shoot_yaw = 25.0;
              control_diff_mode = 2;
              std::cout<<"22222222222222:"<<std::endl;
            }
          }
        }
    } else {//英雄
      if (abs(vyaw) > 6.0 || (target_msg.id == "7" && abs(vyaw) > 2.5)) {
        aim_center = 1;
      } else {
        aim_center = 0;
        shoot_yaw = 25.0;
        control_diff_mode = 5;
      }
    }
    std::cout<<"aim_center:"<<aim_center<<std::endl;
    if (aim_center || aim_center_last_time > 10) {
      shoot_yaw = 1.5;  //2.5改
      control_diff_mode = 3;
      if (aim_distance < 1.5)
      {
        control_diff_mode = 4;
      }     
    }

    ////带有平移
    //if (sqrt((st.vxw) * (st.vxw) + (st.vyw) * (st.vyw)) > 0.2 && abs(st.vyw) > 0.6) {
    //  shoot_yaw = 24.0;  //高速小陀螺，认为可击打角度小只打中间
    //} else if (sqrt((st.vxw) * (st.vxw) + (st.vyw) * (st.vyw)) > 0.2 && abs(st.vyw) < 0.6) {
    //  shoot_yaw = 29.0;  //低速小陀螺
    //}

    //if (
    //  sqrt(
    //    (tar_position[idx].x) * (tar_position[idx].x) +
    //    (tar_position[idx].y) * (tar_position[idx].y)) < 1.6) {
    //  shoot_yaw = 27.0;  //距离近允许泼水
    //} else if (abs(vyaw) > 5.5 && abs(vyaw) < 8.5) {
    //  shoot_yaw = 24.0;  //距离远少打也要追求命中率
    //}

    //if (
    //  abs(vyaw) > 8.5 && sqrt(
    //                       (tar_position[idx].x) * (tar_position[idx].x) +
    //                       (tar_position[idx].y) * (tar_position[idx].y)) > 1.6) {
    //  shoot_yaw = 3.5;  //高速小陀螺锁中心
    //}

    //if (
    //  sqrt(
    //    (tar_position[idx].x) * (tar_position[idx].x) +
    //    (tar_position[idx].y) * (tar_position[idx].y)) < 1.6 ||
    //  target_msg.id == "1" || st.armor_num == ARMOR_NUM_BALANCE) {
    //  control_diff_mode = 2;  //近距离或者是英雄和平步，放宽击打角度
    //} else if (sqrt((st.vxw) * (st.vxw) + (st.vyw) * (st.vyw)) > 0.1)  //平移小陀螺
    //{
    //  if (abs(st.vyw) > 0.6) {
    //    control_diff_mode = 4;  //y方向速度较大，角度较为严格
    //  } else {
    //    control_diff_mode = 1;  //y方向速度小，放宽角度，适合追击
    //  }
    //} else if (
    //  abs(vyaw) > 8.5 && sqrt((st.vxw) * (st.vxw) + (st.vyw) * (st.vyw)) <
    //                       0.1)  //普通高速小陀螺，放宽发枪角度，追求速度
    //{
    //  control_diff_mode = 3;
    //} else if (
    //  abs(vyaw) > 5.5 && sqrt(
    //                       (tar_position[idx].x) * (tar_position[idx].x) +
    //                       (tar_position[idx].y) * (tar_position[idx].y)) >
    //                       1.6)  //普通低速小陀螺，严格发枪角度，追求命中率
    //{
    //  control_diff_mode = 0;
    //} else {
    //  control_diff_mode = 0;
    //}
    // if (target_msg.id == "1" || st.armor_num == ARMOR_NUM_BALANCE) {
    //   shoot_yaw += 8.0;
    // }
    // std::cout << "selective_hitting_yaw:" << selective_hitting_yaw << std::endl;

    float real_selective_hitting_yaw =
      angles::shortest_angular_distance(yaw, tar_position[idx].yaw) * 180 / 3.14;//判断当前选择的装甲板是否为应该击打的装甲板

    if (shoot_yaw < selective_hitting_yaw && st.armor_num != ARMOR_NUM_OUTPOST) {//判断当前选择的装甲板是否为应该击打的装甲板
      //提前选择下一块击打的装甲板,choose_armor_time防止观测带来误判
      if (
        vyaw > 2.0 &&
        real_selective_hitting_yaw > shoot_yaw)  //设定一个值为了保证测距抖动带来的影响
      {
        choose_armor_time++;
        if (choose_armor_time > 15) {
          idx += 3;
          choose_armor_time = 0;
        }
                  // std::cout<<"11111111111"<<std::endl;
      } else if (vyaw < -2.0 && real_selective_hitting_yaw < -shoot_yaw) {
        choose_armor_time++;
        if (choose_armor_time > 15) {
          idx += 1;
          choose_armor_time = 0;
        }
                  // std::cout<<"2222222222222222"<<std::endl;
      }
      // std::cout << "choose armor success" << std::endl;
      if (st.armor_num == ARMOR_NUM_BALANCE) {//平衡步兵已经废用
        idx = idx % 2;  //
      } else {
        idx = idx % 4;
      }

    }

    if (st.armor_num == ARMOR_NUM_OUTPOST)//击打前哨
    {
      //提前选择下一块击打的装甲板,choose_armor_time防止观测带来误判
      if (
        vyaw > 2.0 &&
        real_selective_hitting_yaw > shoot_yaw)  //设定一个值为了保证测距抖动带来的影响
      {
        choose_armor_time++;
        if (choose_armor_time > 15) {
          idx += 2;
          choose_armor_time = 0;
        }
                  // std::cout<<"11111111111"<<std::endl;
      } else if (vyaw < -2.0 && real_selective_hitting_yaw < -shoot_yaw) {
        choose_armor_time++;
        if (choose_armor_time > 15) {
          idx += 1;
          choose_armor_time = 0;
        }
                  // std::cout<<"2222222222222222"<<std::endl;
      }
    }

    float choose_idx_selective_hitting_yaw =
      abs(angles::shortest_angular_distance(yaw, tar_position[idx].yaw)) * 180 /
      3.14;  //判断装甲板是否在可击打角度内

    if(choose_idx_selective_hitting_yaw < shoot_yaw) {
      fire_flag = 1;
    }
      std::cout<<"choose_idx_selective_hitting_yaw"<<choose_idx_selective_hitting_yaw<<std::endl;
    // if (abs(last_yv - st.vyw) > 0.2) {//速度变化太大认为当前预测有错误
    //   fire_flag = 0;
    // }
    // std::cout<<"fire_flag:"<<fire_flag<<std::endl;

    aim_x = tar_position[idx].x + st.vxw * timeDelay;
    aim_y = tar_position[idx].y + st.vyw * timeDelay;
    aim_z = tar_position[idx].z + st.vzw * timeDelay;

    // float aim_center_yaw = angles::normalize_angle(st.tar_yaw);
    // float aim_center_x = st.xw - (st.r1 + st.r2) * cos(aim_center_yaw) / 2;
    // float aim_center_y = st.yw - (st.r1 + st.r2) * sin(aim_center_yaw) / 2;
    // float aim_center_z = tar_position[idx].z;

    std::cout<<"aim_center"<<aim_center<<std::endl;

    // double acc_v = sqrt(target_msg.armor_acceleration.x * target_msg.armor_acceleration.x + target_msg.armor_acceleration.y * target_msg.armor_acceleration.y);

    if (aim_center || aim_center_last_time > 0 ) {
         //锁的中心正常情况下判断是否该发射
         fire_flag = 0;
        for (int  i = 0; i < 4; i++)
        {
          float choose_aim_center_yaw =(angles::shortest_angular_distance(yaw, tar_position[i].yaw)) * 180 /
          3.14;
                                            std::cout<<"chooseaim:"<<choose_aim_center_yaw<<std::endl;
          float aim_target_yaw = choose_aim_center_yaw +
                                  vyaw * timeDelay;  //判断在在经历timeDelay后装甲板yaw角
                                  std::cout<<"aim_target_yaw:"<<aim_target_yaw<<std::endl;
          if (abs(aim_target_yaw) < shoot_yaw ) //判断击发后能否打到装甲板
          {
            // fire_center_time++;
            fire_flag = 1;
          }
        }
        
        // 多判断一次是否击打，防止误判，但效果差故废用
        // if (fire_center_time > 1) {
        //   fire_flag = 1;
        //   fire_center_time = 0;
        // }else{
        //   fire_flag = 0;
        // }



      aim_x = target_msg.position.x;
      aim_y = target_msg.position.y;
      aim_z = tar_position[idx].z;
      // aim_x = aim_center_x;
      // aim_y = aim_center_y;
      // aim_z = aim_center_z;
      std::cout<<"aimx"<<aim_x<<std::endl;
            std::cout<<"aim_y"<<aim_y<<std::endl;
                  std::cout<<"aim_z"<<aim_z<<std::endl;

      

      // if (aim_distance < 2.5 && abs(target_msg.a_yaw) > 1.0 && abs(vyaw) > 14.0 && abs(acc_v) > 0.5)
      // {
      //   aim_x = last_aim_x;
      //   aim_y = last_aim_y;
      //   aim_z = last_aim_z;
      // }
      
    }

    if (abs(linear_v) < 1.2 && abs(vyaw) < 9.0 && fire_flag == 1 && is_infantry &&aim_distance < 3.0)//慢速小陀螺一次可以发射两颗弹丸
    {
      fire_flag = 2;
    }
    if (fire_flag == 2 && abs(vyaw) < 5.0)
    {
      fire_flag = 3;
    }
    
    last_aim_x = aim_x;
    last_aim_y = aim_y;
    last_aim_z = aim_z;

    last_yv = st.vyw;
  if (aim_center && aim_center_last_time <500)//防止误识别突然切换成甩头模式
  {
    aim_center_last_time +=20;
  }else if (aim_center_last_time > 0)
  {
    aim_center_last_time--;
  }

  if (abs(vyaw) < 2.0 && stop_aim_center < 20)
  {
    stop_aim_center++;
  }else if (abs(vyaw) > 2.0 && stop_aim_center > 0)
  {
    stop_aim_center -= 5;
  }
  
  if (stop_aim_center > 15)
  {
    aim_center = 0;
    aim_center_last_time = 0;
  }

  if (abs(st.ayaw) > 7.0 && abs(vyaw) < 2.0)
  {
    fire_flag = 0;
  }
  }
  if (!is_spin)
  {
    aim_center = 0;
    aim_center_last_time = 0;
  }
  
  

  std::cout << "fire_flag:" <<fire_flag<<std::endl; 
  std::cout << "vyaw:" << vyaw << std::endl;
  std::cout << "平面上的线速度：" << linear_v << std::endl; 
  std::cout << "平面上y方向的线速度：" << st.vyw << std::endl; 
  std::cout << "shoot_yaw:" << shoot_yaw << std::endl;
  std::cout << "is_spin:" << is_spin << std::endl;
  std::cout << "spin_count:" << spin_count_ << std::endl; 
  std::cout << "aim_distance:" << aim_distance << std::endl; 
  std::cout << "aim_center_last_time:" << aim_center_last_time << std::endl;   

  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "timeDelay: " << timeDelay);

      if(aim_center || aim_center_last_time > 0)
      {
        temp_pitch =
        pitchTrajectoryCompensation(sqrt((aim_x) * (aim_x) + (aim_y) * (aim_y)) - (st.r1 + st.r2) / 2.0, aim_z, st.current_v) +
        st.pitch_bias;
        
      }
      else{
        temp_pitch =
        pitchTrajectoryCompensation(sqrt((aim_x) * (aim_x) + (aim_y) * (aim_y)), aim_z, st.current_v) +
        st.pitch_bias;
        //std::cout<<"666"<<temp_pitch<<std::endl;
      }

  

  if (temp_pitch) pitch = temp_pitch;
  if (aim_x || aim_y) yaw = (float)(atan2(aim_y, aim_x));
  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), " pitch: " << pitch * 180 / M_PI);
}

/*
@brief 单方向空气阻力弹道模型
@param s:m 距离
@param v:m/s 速度
@param angle:rad 角度
@return z:m
*/
float SolveTrajectory::monoDirectionalAirResistanceModel(float s, float v, float angle)
{
  float z;
  // t为给定v与angle时的飞行时间
  // t = (float)((exp(st.k * s) - 1) / (st.k * v * cos(angle)));
  t = (float)(s / (v * cos(angle)));
  // std::cout << "t :" << t << std::endl;

  // RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"),
  // "t: " << t );
  // if(t < 0)
  // {
  //   //目标点超出最大射程
  //   return 0;
  // }
  // z为给定v与angle时的高度
  z = (float)(v * sin(angle) * t - GRAVITY * t * t / 2);
  //   printf("model %f %f\n", t, z);
  RCLCPP_DEBUG_STREAM(rclcpp::get_logger("armor_tracker"), "z:" << z << "t: " << t);
  return z;
}

/*ARN
@brief 完整弹道模型
@param s:m 距离
@param v:m/s 速度
@param angle:rad 角度
@return z:m
*/
// TODO 完整弹道模型
//  float SolveTrajectory::completeAirResistanceModel(float s, float v, float angle)
//  {

////     continue;
//      return 0;

// }

/*
@brief pitch轴解算
@param s:m 距离
@param z:m 高度
@param v:m/s
@return angle_pitch:rad
*/
float SolveTrajectory::pitchTrajectoryCompensation(float s, float z, float v)
{
  //TODO:根据陀螺仪安装位置调整距离求解方式
    //降维，坐标系Y轴以垂直向上为正方向
    auto dist_vertical = z + st.z_bias;
    std::cout<<st.z_bias<<std::endl;
    auto vertical_tmp = dist_vertical;
    auto dist_horizonal = s - st.s_bias;
    // auto dist_vertical = xyz[2];
    // auto dist_horizonal = sqrt(xyz.squaredNorm() - dist_vertical * dist_vertical);
    t = (dist_horizonal/(v * cos(atan(dist_vertical / dist_horizonal))));
    auto pitch = atan(dist_vertical / dist_horizonal) * 180 / CV_PI;
    auto pitch_new = pitch;
    //开始使用龙格库塔法求解弹道补偿
    for (int i = 0; i < 10; i++)
    {
        //TODO:可以考虑将迭代起点改为世界坐标系下的枪口位置
        //初始化
        auto x = 0.0;
        auto y = 0.0;
        auto p = tan(pitch_new / 180 * CV_PI);
        auto u = v / sqrt(1 + pow(p,2));
        auto delta_x = dist_horizonal / R_K_iter;
        for (int j = 0; j < R_K_iter; j++)
        {
            auto k1_u = -st.k * u * sqrt(1 + pow(p, 2));
            auto k1_p = -GRAVITY / pow(u, 2);
            auto k1_u_sum = u + k1_u * (delta_x / 2);
            auto k1_p_sum = p + k1_p * (delta_x / 2);

            auto k2_u = -st.k * k1_u_sum * sqrt(1 + pow(k1_p_sum, 2));
            auto k2_p = -GRAVITY / pow(k1_u_sum, 2);
            auto k2_u_sum = u + k2_u * (delta_x / 2);
            auto k2_p_sum = p + k2_p * (delta_x / 2);

            auto k3_u = -st.k * k2_u_sum * sqrt(1 + pow(k2_p_sum, 2));
            auto k3_p = -GRAVITY / pow(k2_u_sum, 2);
            auto k3_u_sum = u + k3_u * (delta_x / 2);
            auto k3_p_sum = p + k3_p * (delta_x / 2);

            auto k4_u = -st.k * k3_u_sum * sqrt(1 + pow(k3_p_sum, 2));
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
            pitch_new = atan(vertical_tmp / dist_horizonal) * 180 / CV_PI;
        }
    }
    //std::cout<<"666"<<(pitch_new  * CV_PI /180)<<std::endl;
    return pitch_new  * CV_PI /180;
}


}  // namespace rm_auto_aim