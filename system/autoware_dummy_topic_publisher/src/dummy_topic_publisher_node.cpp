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

namespace autoware::dummy_topic_publisher
{
DummyTopicPublisher::DummyTopicPublisher(const rclcpp::NodeOptions & options) : Node("dummy_topic_publisher", options)
{
    RCLCPP_INFO(get_logger(), "Hello, Autoware Dummy Topic Publisher!");
}
} // namespace autoware::dummy_topic_publisher

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(autoware::dummy_topic_publisher::DummyTopicPublisher)
