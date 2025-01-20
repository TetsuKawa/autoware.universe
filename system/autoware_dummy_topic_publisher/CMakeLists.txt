cmake_minimum_required(VERSION 3.14)
project(autoware_dummy_topic_publisher)

find_package(autoware_cmake REQUIRED)
autoware_package()

ament_auto_add_library(${PROJECT_NAME} SHARED
  src/dummy_topic_publisher_node.cpp
)

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "autoware::dummy_topic_publisher::DummyTopicPublisher"
  EXECUTABLE ${PROJECT_NAME}_node
  EXECUTOR MultiThreadedExecutor
)

ament_auto_package(INSTALL_TO_SHARE
  launch
  config
)
