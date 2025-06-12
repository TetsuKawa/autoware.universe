// Copyright 2025 The Autoware Contributors
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

#include "graph/graph.hpp"
#include "tests/timeline.hpp"
#include "tests/utils.hpp"
#include "types/diagnostics.hpp"

#include <diagnostic_msgs/msg/diagnostic_array.hpp>
#include <diagnostic_msgs/msg/diagnostic_status.hpp>

#include <gtest/gtest.h>

// for debug
#include <iostream>

int main()
{
  const auto input = "OOOOOOOOOOEEEEEEEEEE";
  const auto output1 = "OOOOOOOOOOEEEEEEEEEE";
  const auto output2 = "OOOOOOOOOOEEEEEEEEEE";

  autoware::diagnostic_graph_aggregator::Timeline test;
  test.interval(0.1);
  test.set("dummy: name0", input);
  test.set("dummy: test", "WWWWWWOEEEEEO");
  test.execute();

  std::cout << test.get("path") << std::endl;
  std::cout << output1 << std::endl;
  std::cout << output2 << std::endl;
}
