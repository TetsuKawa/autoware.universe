cmake_minimum_required(VERSION 3.14)
project(autoware_diagnostic_graph_aggregator)

find_package(autoware_cmake REQUIRED)
autoware_package()

ament_auto_add_library(${PROJECT_NAME} SHARED
  "src/common/config/context.cpp"
  "src/common/config/entity.cpp"
  "src/common/config/loader.cpp"
  "src/common/config/substitutions.cpp"
  "src/common/config/yaml.cpp"
  "src/common/graph/graph.cpp"
  "src/common/graph/units.cpp"
  "src/common/graph/links.cpp"
  "src/common/graph/logic.cpp"
  "src/common/graph/levels.cpp"
  "src/common/logics/and.cpp"
  "src/common/utils/logger.cpp"
)
ament_auto_add_library(aggregator SHARED
  "src/node/aggregator.cpp"
  "src/node/command_mode_mapping.cpp"
)
ament_auto_add_library(converter SHARED
  "src/node/converter.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE src/common)
target_include_directories(aggregator PRIVATE src/common)

#ament_auto_add_executable(tree "src/tool/tree.cpp")
#ament_auto_add_executable(plantuml "src/tool/plantuml.cpp")
ament_auto_add_executable(check_graph "src/tool/check_graph.cpp")

#target_include_directories(tree PRIVATE src/common)
#target_include_directories(plantuml PRIVATE src/common)
target_include_directories(check_graph PRIVATE src/common)

rclcpp_components_register_node(aggregator
  PLUGIN "autoware::diagnostic_graph_aggregator::AggregatorNode"
  EXECUTABLE "aggregator_node"
)

rclcpp_components_register_node(converter
  PLUGIN "autoware::diagnostic_graph_aggregator::ConverterNode"
  EXECUTABLE "converter_node"
)

if(BUILD_TESTING)
  get_filename_component(RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test/files ABSOLUTE)
  ament_auto_add_gtest(gtest_${PROJECT_NAME}
    test/src/test1.cpp
    test/src/test2.cpp
    test/src/utils.cpp
  )
  target_compile_definitions(gtest_${PROJECT_NAME} PRIVATE TEST_RESOURCE_PATH="${RESOURCE_PATH}")
  target_include_directories(gtest_${PROJECT_NAME} PRIVATE src/common)
endif()

ament_auto_package(INSTALL_TO_SHARE config example launch)
