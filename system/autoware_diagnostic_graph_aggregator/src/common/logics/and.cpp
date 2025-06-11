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

#include "logics/and.hpp"

#include "config/entity.hpp"
#include "config/yaml.hpp"
#include "graph/links.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace autoware::diagnostic_graph_aggregator
{

AndLogic::AndLogic(Parser & parser)
{
  links_ = parser.parse_node_list(parser.yaml().optional("list").list());
}

std::vector<LinkPort *> AndLogic::ports() const
{
  return {links_};
}

DiagnosticLevel AndLogic::level() const
{
  if (links_->empty()) {
    return DiagnosticStatus::OK;
  }

  DiagnosticLevel result = DiagnosticStatus::OK;
  for (const auto & level : links_->levels()) {
    result = std::max(result, level);
  }
  return result;
}

OrLogic::OrLogic(Parser & parser)
{
  links_ = parser.parse_node_list(parser.yaml().optional("list").list());
}

std::vector<LinkPort *> OrLogic::ports() const
{
  return {links_};
}

DiagnosticLevel OrLogic::level() const
{
  if (links_->empty()) {
    return DiagnosticStatus::OK;
  }

  DiagnosticLevel result = DiagnosticStatus::ERROR;
  for (const auto & level : links_->levels()) {
    result = std::min(result, level);
  }
  return result;
}

DiagLogic::DiagLogic(Parser & parser)
{
  link_ = parser.parse_diag_item(parser.yaml());
}

std::vector<LinkPort *> DiagLogic::ports() const
{
  return {link_};
}

DiagnosticLevel DiagLogic::level() const
{
  // STALE to ERROR
  return std::min(link_->level(), DiagnosticStatus::ERROR);
}

ConstLogic::ConstLogic(Parser &)
{
}

DiagnosticLevel ConstOkLogic::level() const
{
  return DiagnosticStatus::OK;
}

DiagnosticLevel ConstWarnLogic::level() const
{
  return DiagnosticStatus::WARN;
}

DiagnosticLevel ConstErrorLogic::level() const
{
  return DiagnosticStatus::ERROR;
}

DiagnosticLevel ConstStaleLogic::level() const
{
  return DiagnosticStatus::STALE;
}

WarnToOkLogic::WarnToOkLogic(Parser & parser)
{
  link_ = parser.parse_node_item(parser.yaml().required("item"));
}

std::vector<LinkPort *> WarnToOkLogic::ports() const
{
  return {link_};
}

DiagnosticLevel WarnToOkLogic::level() const
{
  if (link_->level() == DiagnosticStatus::WARN) {
    return DiagnosticStatus::OK;
  } else {
    return link_->level();
  }
}

WarnToErrorLogic::WarnToErrorLogic(Parser & parser)
{
  link_ = parser.parse_node_item(parser.yaml().required("item"));
}

std::vector<LinkPort *> WarnToErrorLogic::ports() const
{
  return {link_};
}

DiagnosticLevel WarnToErrorLogic::level() const
{
  if (link_->level() == DiagnosticStatus::WARN) {
    return DiagnosticStatus::ERROR;
  } else {
    return link_->level();
  }
}

RegisterLogic<DiagLogic> registration4("diag");
RegisterLogic<AndLogic> registration("and");
RegisterLogic<OrLogic> registration3("or");
RegisterLogic<ConstOkLogic> registration2("ok");
RegisterLogic<ConstStaleLogic> registration6("stale");
RegisterLogic<ConstWarnLogic> registration7("warn");
RegisterLogic<ConstErrorLogic> registration8("error");
RegisterLogic<AndLogic> registration5("short-circuit-and");
RegisterLogic<WarnToOkLogic> registration9("warn-to-ok");
RegisterLogic<WarnToErrorLogic> registration10("warn-to-error");

}  // namespace autoware::diagnostic_graph_aggregator
