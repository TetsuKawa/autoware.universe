// Copyright 2025 TIER IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DUMMY_TOPIC_PUBLISHER_NODE_HPP_
#define DUMMY_TOPIC_PUBLISHER_NODE_HPP_

// ROS 2 core
#include <rclcpp/rclcpp.hpp>

#include <string>
#include <yaml-cpp/yaml.h>
#include <rosidl_typesupport_cpp/message_type_support.hpp>

namespace autoware::dummy_topic_publisher
{
struct NodeParam
{
  std::string topic_name;
  std::string topic_type;
  std::string topic_value_yaml_path;
  size_t qos;
  bool transient_local;
  bool best_effort;
  int rate;
};

class DummyTopicPublisher : public rclcpp::Node
{
public:
  explicit DummyTopicPublisher(const rclcpp::NodeOptions & options);

private:
  NodeParam node_param_;
  const rosidl_message_type_support_t * type_support_handle_;
  rclcpp::SerializedMessage serialized_msg_;

  // Publisher
  rclcpp::GenericPublisher::SharedPtr pub_;

  // Timer
  rclcpp::TimerBase::SharedPtr timer_;

  // Timer callback
  void on_timer();

  rclcpp::SerializedMessage get_serialized_message(YAML::Node yaml_node);
};
}  // namespace autoware::dummy_topic_publisher

#endif  // DUMMY_TOPIC_PUBLISHER_NODE_HPP_