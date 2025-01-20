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

namespace autoware::dummy_topic_publisher
{
class DummyTopicPublisher : public rclcpp::Node
{
public:
  explicit DummyTopicPublisher(const rclcpp::NodeOptions & options);

private:

};
}  // namespace autoware::dummy_topic_publisher

#endif  // DUMMY_TOPIC_PUBLISHER_NODE_HPP_