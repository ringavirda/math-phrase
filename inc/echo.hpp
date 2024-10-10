#pragma once

#include "semantic.hpp"
#include "syntax.hpp"

/**
 * Special echo extensions used to print out interpreter specific objects.
 */

/**
 * @brief Prints given `Sequence` into the console using specific format and
 * naming scheme. Supports subsequences and displays them separately. 
 * @param sq a `Sequence` to echo out.
 */
void echo_sequence(Sequence);

/**
 * @brief Recursively prints a `Tree` starting from the given `Node`. Performs
 * the printing in reverse order and using special characters to create a visual
 * representation of the tree.
 * Requires `utf-8` to work properly. 
 * @param prefix used to add indentation to nodes.
 * @param node next `Node` to echo.
 * @param is_left change visual guide type.
 */
void echo_tree(const std::string &prefix,
               const std::shared_ptr<Node> node,
               bool is_left);
/**
 * @brief Simplified signature version of `echo_tree`.
 * @param node root `Node` of the tree. 
 */
void echo_tree(const std::shared_ptr<Node> node);