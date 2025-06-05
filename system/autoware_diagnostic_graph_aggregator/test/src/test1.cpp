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

#include <gtest/gtest.h>

using namespace autoware::diagnostic_graph_aggregator;  // NOLINT(build/namespaces)

TEST(ConfigFile, RootNotFound)
{
  EXPECT_THROW(Graph(resource("test1/fake-file-name.yaml")), FileNotFound);
}

TEST(ConfigFile, FileNotFound)
{
  EXPECT_THROW(Graph(resource("test1/file-not-found.yaml")), FileNotFound);
}

TEST(ConfigFile, UnknownSubstitution)
{
  EXPECT_THROW(Graph(resource("test1/unknown-substitution.yaml")), UnknownSubstitution);
}

TEST(ConfigFile, UnknownNodeType)
{
  EXPECT_THROW(Graph(resource("test1/unknown-unit-type.yaml")), UnknownLogic);
}

TEST(ConfigFile, InvalidDictType)
{
  EXPECT_THROW(Graph(resource("test1/invalid-dict-type.yaml")), InvalidType);
}

TEST(ConfigFile, InvalidListType)
{
  EXPECT_THROW(Graph(resource("test1/invalid-list-type.yaml")), InvalidType);
}

TEST(ConfigFile, FieldNotFound)
{
  EXPECT_THROW(Graph(resource("test1/field-not-found.yaml")), FieldNotFound);
}

TEST(ConfigFile, PathConflict)
{
  EXPECT_THROW(Graph(resource("test1/path-conflict.yaml")), PathConflict);
}

TEST(ConfigFile, PathNotFound)
{
  EXPECT_THROW(Graph(resource("test1/path-not-found.yaml")), LinkNotFound);
}

TEST(ConfigFile, GraphCirculation)
{
  EXPECT_THROW(Graph(resource("test1/graph-circulation.yaml")), UnitLoopFound);
}
