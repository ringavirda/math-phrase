#include <ranges>

#include "common.hpp"
#include "echo.hpp"

void echo_sequence(Sequence seq)
{
    // Top level sequence.
    echo("Seq" + seq.value + ":");
    for (auto &&symbol : std::ranges::subrange(seq.begin(), seq.end()))
    {
        // Process subsequence separately.
        if (symbol->type == SymbolType::Sequence)
        {
            // Add minus sign if needed.
            echo_inline(std::vformat((symbol->is_negated) ? "[-Seq{}]" : "[Seq{}]",
                                     std::make_format_args(symbol->value)));
            echo();
            // Recursively process subsequence.
            echo_sequence(*std::dynamic_pointer_cast<Sequence>(symbol));
            // Display that after this is the continuation of the top level
            // sequence.
            echo(std::format("Seq{} cont.:", seq.value));
        }
        else
            // Echo out next symbol.
            echo_inline(std::vformat((symbol->is_negated) ? "[-{}]" : "[{}]",
                                     std::make_format_args(symbol->value)));
    }
    // Add empty line after sequences to separate.
    echo();
}

void echo_tree(const std::string &prefix,
               const std::shared_ptr<Node> node,
               bool is_left)
{
    // Check for recursion end.
    if (node != nullptr)
    {
        // Indent next node.
        echo_inline(prefix);
        // Add visual guides.
        echo_inline(is_left ? "├──" : "└──");
        // Echo the node.
        echo(node->get_literal());

        // Recursively process relations.
        echo_tree(prefix + (is_left ? "│   " : "    "),
                  node->get_left_child(), true);
        echo_tree(prefix + (is_left ? "│   " : "    "),
                  node->get_right_child(), false);
    }
}

void echo_tree(const std::shared_ptr<Node> node)
{
    // Call full signature overload.
    echo_tree("", node, false);
}