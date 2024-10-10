#pragma once

#include "semantic.hpp"
#include "syntax.hpp"

void echo_sequence(Sequence seq);
void echo_tree(const std::string &prefix,
               const std::shared_ptr<Node> node,
               bool is_left);
void echo_tree(const std::shared_ptr<Node> node);