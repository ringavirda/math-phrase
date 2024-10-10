#include <iostream>

#include "common.hpp"
#include "semantic.hpp"

/**
 * @brief Internal function to convert syntaxic `Symbol` into semantic `Node`.
 * @param symbol a `Symbol` from syntax sequence.
 * @returns Corresponding `Node` for the semantic `Tree`. 
 */
std::shared_ptr<Node> parse_node(std::shared_ptr<Symbol> symbol)
{
    if (symbol->type == SymbolType::Value)
    { // Create a leaf if value found.
        try
        {
            return std::make_shared<Leaf>(Leaf(
                std::stod(symbol->value),
                symbol->is_negated));
        }
        catch (const std::invalid_argument &)
        { // Fail if inappropriate conversion.
            throw analysis_failed("Failed to parse double value!");
        }
    }
    else if (symbol->type == SymbolType::Sequence)
    { // Call semantic analysis if a subsequence is found. 
        auto subtree = construct_semantic(
            *std::dynamic_pointer_cast<Sequence>(symbol));
        if (symbol->is_negated)
            subtree.root->negate();
        return subtree.root;
    }
    else if (symbol->type == SymbolType::Operator)
    { // Parse operator appropriately.
        if (symbol->value == "+")
            return std::make_shared<Node>(Node(OperationType::Add, symbol->value));
        else if (symbol->value == "-")
            return std::make_shared<Node>(Node(OperationType::Sub, symbol->value));
        else if (symbol->value == "*")
            return std::make_shared<Node>(Node(OperationType::Mul, symbol->value));
        else if (symbol->value == "/")
            return std::make_shared<Node>(Node(OperationType::Div, symbol->value));
        else
            // This really shouldn't happen.
            throw analysis_failed("Couldn't infer the operation type!");
    }
    else
        // This really shouldn't happen either.
        throw analysis_failed("Wasn't able to parse node!");
}

/** @brief Internal inline function to check if symbol is a "+" or "-". */
inline bool is_first_class(std::shared_ptr<Symbol> symbol)
{
    return symbol->value == "+" || symbol->value == "-";
}

/** @brief Internal inline function to check if symbol is a "*" or "/". */
inline bool is_second_class(std::shared_ptr<Symbol> symbol)
{
    return symbol->value == "*" || symbol->value == "/";
}

/** @brief Internal inline function to extract previous node. */
inline std::shared_ptr<Symbol> find_left_operator(
    std::vector<std::shared_ptr<Symbol>>::iterator from,
    std::vector<std::shared_ptr<Symbol>>::iterator to)
{
    return (std::distance(from, to) < 2) ? nullptr : *(to - 1);
}

/** @brief Internal inline function to extract next node. */
inline std::shared_ptr<Symbol> find_right_operator(
    std::vector<std::shared_ptr<Symbol>>::iterator from,
    std::vector<std::shared_ptr<Symbol>>::iterator to)
{
    return (std::distance(from, to) < 2) ? nullptr : *(from + 1);
}

Tree construct_semantic(Sequence syntaxic)
{
    Tree tree;
    auto current = syntaxic.begin();
    // Create top level container to properly create logic.
    std::shared_ptr<Node> last_second_class = nullptr;

    if (syntaxic.size() == 1)
    { // Simply return value if it is singular.
        tree.root = parse_node(*current);
        return tree;
    }
    // Main analysis loop.
    while (current != syntaxic.end())
    {
        // Extract relatives.
        auto next = find_right_operator(current, syntaxic.end());
        auto prev = find_left_operator(syntaxic.begin(), current);

        if (prev == nullptr)
        { // Parse if current node is first in sequence.
            tree.root = parse_node(next);
            tree.root->set_left_child(parse_node((next)->left_relative));
            if (is_second_class(next))
                last_second_class = tree.root;
        }
        else if (is_first_class(prev))
        { // Parse if prev was "+" or "-".
            if (next == nullptr)
            { // End of sequence.
                tree.root->set_right_child(parse_node(prev->right_relative));
            }
            else if (is_first_class(next))
            { // Add another first class to the root.
                auto n_root = parse_node(next);
                tree.root->set_right_child(parse_node(prev->right_relative));
                n_root->set_left_child(tree.root);
                tree.root = n_root;
                last_second_class = nullptr;
            }
            else
            { // Start new branch.
                auto n_child = parse_node(next);
                tree.root->set_right_child(n_child);
                n_child->set_left_child(parse_node(next->left_relative));
                last_second_class = n_child;
            }
        }
        else if (is_second_class(prev))
        { // Parse if prev was "*" or "/".
            if (next == nullptr)
            { // End of the line.
                last_second_class->set_right_child(
                    parse_node(prev->right_relative));
            }
            else if (is_first_class(next))
            { // End branch if next is first class.
                auto n_root = parse_node(next);
                last_second_class->set_right_child(
                    parse_node(prev->right_relative));
                n_root->set_left_child(tree.root);
                tree.root = n_root;
                last_second_class = nullptr;
            }
            else
            { // Continue branch.
                auto n_child = parse_node(next);
                last_second_class->set_right_child(n_child);
                n_child->set_left_child(parse_node(next->left_relative));
                last_second_class = n_child;
            }
        }
        // Update iterator while guarding against overflow.
        current = (current + 1 == syntaxic.end())
                      ? syntaxic.end()
                      : current + 2;
    }
    // Return reconstructed semantic tree.
    return tree;
}