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

#include "config/entity.hpp"
#include "config/loader.hpp"
#include "graph/links.hpp"
#include "graph/units.hpp"
#include "utils/logger.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace autoware::diagnostic_graph_aggregator
{

std::string text_node_unit(const NodeUnit * unit)
{
  std::stringstream ss;
  ss << "NodeUnit: " << std::setw(5) << std::left << unit->type() << " " << unit->path();
  return ss.str();
}

std::string text_diag_unit(const DiagUnit * unit)
{
  std::stringstream ss;
  ss << "DiagUnit: " << std::setw(5) << std::left << unit->name();
  return ss.str();
}

std::string text_link_unit(const LinkUnit * unit)
{
  std::stringstream ss;
  ss << "LinkUnit: " << unit->link();
  return ss.str();
}

std::string text_temp_node(const TempNode * unit)
{
  std::stringstream ss;
  ss << "TempNode: " << unit->yaml().str();
  return ss.str();
}

std::string text_temp_diag(const TempDiag * unit)
{
  std::stringstream ss;
  ss << "TempDiag: " << unit->yaml().str();
  return ss.str();
}

std::string text_unit_base(const BaseUnit * unit)
{
  if (const auto x = dynamic_cast<const NodeUnit *>(unit)) return text_node_unit(x);
  if (const auto x = dynamic_cast<const DiagUnit *>(unit)) return text_diag_unit(x);
  if (const auto x = dynamic_cast<const LinkUnit *>(unit)) return text_link_unit(x);
  if (const auto x = dynamic_cast<const TempNode *>(unit)) return text_temp_node(x);
  if (const auto x = dynamic_cast<const TempDiag *>(unit)) return text_temp_diag(x);

  std::stringstream ss;
  ss << "Unknown Unit[" << unit << "]";
  return ss.str();
}

void dump_file_tree(const FileData * file, const std::string & indent = "")
{
  std::cout << indent << "- " << file->resolved_path << std::endl;
  for (const auto & child : file->files) {
    dump_file_tree(child, indent + "  ");
  }
}

void dump_file_tree(const ConfigLoader & loader)
{
  return dump_file_tree(loader.root());
}

void dump_file_list(const ConfigLoader & loader)
{
  for (const auto & file : loader.files()) {
    std::cout << "- " << file->resolved_path << std::endl;
  }
}

void dump_unit_base(const BaseUnit * unit)
{
  std::cout << text_unit_base(unit) << std::endl;
  if (const auto node = dynamic_cast<const NodeUnit *>(unit)) {
    for (const auto & unit : node->children()) {
      std::cout << " - " << text_unit_base(unit) << std::endl;
    }
  }
}

void dump_unit_list(const ConfigLoader & loader)
{
  for (const auto & node : loader.nodes()) dump_unit_base(node);
  for (const auto & diag : loader.diags()) dump_unit_base(diag);
}

void dump_port_list(const std::vector<LinkPort *> & ports)
{
  for (const auto & port : ports) {
    std::cout << "Port:" << std::endl;
    for (const auto & unit : port->iterate()) {
      std::cout << " - " << text_unit_base(unit) << std::endl;
    }
  }
}

void check_graph(const std::string & path)
{
  auto loader = ConfigLoader(Logger());
  loader.load(path);
  std::cout << "Graph loaded" << std::endl;

  dump_unit_list(loader);
  dump_port_list(loader.ports());
}

}  // namespace autoware::diagnostic_graph_aggregator

int main(int argc, char ** argv)
{
  if (argc == 2) {
    autoware::diagnostic_graph_aggregator::check_graph(argv[1]);
    std::cout << "================================================" << std::endl;
    return 0;
  }

  std::vector<std::string> paths = {
    "$(find-pkg-share autoware_diagnostic_graph_aggregator)/example/graph/main.yaml",
    "$(find-pkg-share autoware_launch)/config/system/diagnostics/autoware-main.yaml",
  };

  for (const auto & path : paths) {
    autoware::diagnostic_graph_aggregator::check_graph(path);
    std::cout << "================================================" << std::endl;
  }
}
