// Copyright 2025 TIER IV, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language
// governing permissions and limitations under the License.

#include "dummy_topic_publisher_node.hpp"

#include <rclcpp/serialization.hpp>
#include <rclcpp/serialized_message.hpp>
#include <stdexcept>
#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>



namespace autoware::dummy_topic_publisher
{
DummyTopicPublisher::DummyTopicPublisher(const rclcpp::NodeOptions & options) : Node("dummy_topic_publisher", options)
{
  node_param_.topic_name = declare_parameter<std::string>("topic_name");
  node_param_.topic_type = declare_parameter<std::string>("topic_type");
  node_param_.topic_value_yaml_path = declare_parameter<std::string>("topic_value_yaml_path");
  node_param_.qos = declare_parameter("qos", 1);
  node_param_.transient_local = declare_parameter<bool>("transient_local");
  node_param_.best_effort = declare_parameter<bool>("best_effort");
  node_param_.rate = declare_parameter<int>("rate");

  auto type_support_library = rclcpp::get_typesupport_library(node_param_.topic_type, "rosidl_typesupport_cpp");
  type_support_handle_ = rclcpp::get_typesupport_handle(node_param_.topic_type, "rosidl_typesupport_cpp", *type_support_library);

  if (!type_support_handle_) {
    RCLCPP_ERROR(get_logger(), "Failed to get type support handle for %s", node_param_.topic_type.c_str());
    throw std::runtime_error("Failed to get type support handle for " + node_param_.topic_type);
  }

  if (node_param_.topic_value_yaml_path.empty()) {
    RCLCPP_ERROR(get_logger(), "YAML file path is not specified.");
    throw std::runtime_error("YAML file path is not specified.");
  }

  rclcpp::QoS qos = rclcpp::QoS{node_param_.qos};
  if (node_param_.transient_local) {
    qos.transient_local();
  }
  if (node_param_.best_effort) {
    qos.best_effort();
  }

  // Publisher
  pub_ = create_generic_publisher(node_param_.topic_name, node_param_.topic_type, qos);

  auto yaml_node = YAML::LoadFile(node_param_.topic_value_yaml_path);
  rclcpp::SerializedMessage serialized_msg_ = get_serialized_message(yaml_node);

  // Timer
  const auto update_period_ns = rclcpp::Rate(node_param_.rate).period();
  timer_ = create_timer(this, get_clock(), update_period_ns, std::bind(&DummyTopicPublisher::on_timer, this));
}

void DummyTopicPublisher::on_timer()
{
  try {
    pub_->publish(serialized_msg_);
  } catch (const std::exception & e) {
    RCLCPP_ERROR(this->get_logger(), "Failed to serialize message: %s", e.what());
  }
}

rclcpp::SerializedMessage DummyTopicPublisher::get_serialized_message(YAML::Node yaml_node)
{
  rclcpp::SerializedMessage serialized_msg;
  // TOFO
  return serialized_msg;
}
}  // namespace autoware::dummy_topic_publisher

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(autoware::dummy_topic_publisher::DummyTopicPublisher)
