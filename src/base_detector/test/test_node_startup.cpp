// Copyright 2022 Chen Jun

#include <gtest/gtest.h>

#include <rclcpp/executors.hpp>
#include <rclcpp/node_options.hpp>
#include <rclcpp/utilities.hpp>

// STD
#include <memory>

#include "base_detector/base_node.hpp"

TEST(BaseDetectorNodeTest, NodeStartupTest)
{
  rclcpp::NodeOptions options;
  auto node = std::make_shared<rm_auto_aim::BaseDetectorNode>(options);
  node.reset();
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  rclcpp::init(argc, argv);
  auto result = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return result;
}