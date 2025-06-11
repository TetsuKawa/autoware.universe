// Copyright 2023 The Autoware Contributors
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

#include "config/errors.hpp"
#include "graph/graph.hpp"
#include "utils.hpp"

#include <rclcpp/clock.hpp>

#include <diagnostic_msgs/msg/diagnostic_array.hpp>
#include <diagnostic_msgs/msg/diagnostic_status.hpp>
#include <tier4_system_msgs/msg/diagnostic_graph.hpp>

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace autoware::diagnostic_graph_aggregator;  // NOLINT(build/namespaces)

using diagnostic_msgs::msg::DiagnosticArray;
using diagnostic_msgs::msg::DiagnosticStatus;

constexpr auto OK = DiagnosticStatus::OK;
constexpr auto WARN = DiagnosticStatus::WARN;
constexpr auto ERROR = DiagnosticStatus::ERROR;
constexpr auto STALE = DiagnosticStatus::STALE;

struct GraphTestParam
{
  std::string config;
  std::vector<uint8_t> inputs;
  uint8_t result;
};

class GraphTest : public testing::TestWithParam<GraphTestParam>
{
};

DiagnosticArray create_input(const rclcpp::Time & stamp, const std::vector<uint8_t> & levels)
{
  DiagnosticArray array;
  array.header.stamp = stamp;
  for (size_t i = 0; i < levels.size(); ++i) {
    DiagnosticStatus status;
    status.level = levels[i];
    status.name = "test: input-" + std::to_string(i);
    array.status.push_back(status);
  }
  return array;
};

uint8_t get_output(const Graph & graph, const rclcpp::Time & stamp)
{
  const auto struct_nodes = graph.create_struct_msg(stamp).nodes;
  const auto status_nodes = graph.create_status_msg(stamp).nodes;

  for (size_t i = 0; i < struct_nodes.size(); ++i) {
    if (struct_nodes[i].path == "output") {
      return status_nodes[i].level;
    }
  }
  throw std::runtime_error("output node is not found");
}

TEST_P(GraphTest, Aggregation)
{
  const auto stamp = rclcpp::Clock(RCL_ROS_TIME).now();
  const auto param = GetParam();
  Graph graph(resource(param.config));
  graph.update(stamp, create_input(stamp, param.inputs));
  graph.update(stamp);

  const auto output = get_output(graph, stamp);
  EXPECT_EQ(output, param.result);
}

// clang-format off

INSTANTIATE_TEST_SUITE_P(And, GraphTest,
  testing::Values(
    GraphTestParam{"logics/and.yaml", {OK,    OK   }, OK   },
    GraphTestParam{"logics/and.yaml", {OK,    WARN }, WARN },
    GraphTestParam{"logics/and.yaml", {OK,    ERROR}, ERROR},
    GraphTestParam{"logics/and.yaml", {OK,    STALE}, ERROR},
    GraphTestParam{"logics/and.yaml", {WARN,  OK   }, WARN },
    GraphTestParam{"logics/and.yaml", {WARN,  WARN }, WARN },
    GraphTestParam{"logics/and.yaml", {WARN,  ERROR}, ERROR},
    GraphTestParam{"logics/and.yaml", {WARN,  STALE}, ERROR},
    GraphTestParam{"logics/and.yaml", {ERROR, OK   }, ERROR},
    GraphTestParam{"logics/and.yaml", {ERROR, WARN }, ERROR},
    GraphTestParam{"logics/and.yaml", {ERROR, ERROR}, ERROR},
    GraphTestParam{"logics/and.yaml", {ERROR, STALE}, ERROR},
    GraphTestParam{"logics/and.yaml", {STALE, OK   }, ERROR},
    GraphTestParam{"logics/and.yaml", {STALE, WARN }, ERROR},
    GraphTestParam{"logics/and.yaml", {STALE, ERROR}, ERROR},
    GraphTestParam{"logics/and.yaml", {STALE, STALE}, ERROR}
  )
);

INSTANTIATE_TEST_SUITE_P(Or, GraphTest,
  testing::Values(
    GraphTestParam{"logics/or.yaml", {OK,    OK   }, OK   },
    GraphTestParam{"logics/or.yaml", {OK,    WARN }, OK   },
    GraphTestParam{"logics/or.yaml", {OK,    ERROR}, OK   },
    GraphTestParam{"logics/or.yaml", {OK,    STALE}, OK   },
    GraphTestParam{"logics/or.yaml", {WARN,  OK   }, OK   },
    GraphTestParam{"logics/or.yaml", {WARN,  WARN }, WARN },
    GraphTestParam{"logics/or.yaml", {WARN,  ERROR}, WARN },
    GraphTestParam{"logics/or.yaml", {WARN,  STALE}, WARN },
    GraphTestParam{"logics/or.yaml", {ERROR, OK   }, OK   },
    GraphTestParam{"logics/or.yaml", {ERROR, WARN }, WARN },
    GraphTestParam{"logics/or.yaml", {ERROR, ERROR}, ERROR},
    GraphTestParam{"logics/or.yaml", {ERROR, STALE}, ERROR},
    GraphTestParam{"logics/or.yaml", {STALE, OK   }, OK   },
    GraphTestParam{"logics/or.yaml", {STALE, WARN }, WARN },
    GraphTestParam{"logics/or.yaml", {STALE, ERROR}, ERROR},
    GraphTestParam{"logics/or.yaml", {STALE, STALE}, ERROR}
  )
);

INSTANTIATE_TEST_SUITE_P(WarnToOk, GraphTest,
  testing::Values(
    GraphTestParam{"logics/warn-to-ok.yaml", {OK   }, OK   },
    GraphTestParam{"logics/warn-to-ok.yaml", {WARN }, OK},
    GraphTestParam{"logics/warn-to-ok.yaml", {ERROR}, ERROR},
    GraphTestParam{"logics/warn-to-ok.yaml", {STALE}, ERROR}
  )
);

INSTANTIATE_TEST_SUITE_P(WarnToError, GraphTest,
  testing::Values(
    GraphTestParam{"logics/warn-to-error.yaml", {OK   }, OK   },
    GraphTestParam{"logics/warn-to-error.yaml", {WARN }, ERROR},
    GraphTestParam{"logics/warn-to-error.yaml", {ERROR}, ERROR},
    GraphTestParam{"logics/warn-to-error.yaml", {STALE}, ERROR}
  )
);

// clang-format on
