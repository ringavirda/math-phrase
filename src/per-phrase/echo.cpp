#include <ranges>

#include "common.hpp"
#include "echo.hpp"

void echo_sequence(Sequence seq)
{
    echo("Seq" + seq.value + ":");
    for (auto &&symbol : std::ranges::subrange(seq.begin(), seq.end()))
    {
        if (symbol->type == SymbolType::Sequence)
        {
            echo_inline(std::vformat((symbol->is_negated) ? "[-Seq{}]" : "[Seq{}]",
                                     std::make_format_args(symbol->value)));
            echo();
            echo_sequence(*std::dynamic_pointer_cast<Sequence>(symbol));
            echo(std::format("Seq{} cont.:", seq.value));
        }
        else
            echo_inline(std::vformat((symbol->is_negated) ? "[-{}]" : "[{}]",
                                     std::make_format_args(symbol->value)));
    }
    echo();
}

void echo_tree(const std::string &prefix,
               const std::shared_ptr<Node> node,
               bool is_left)
{
    if (node != nullptr)
    {
        echo_inline(prefix);
        echo_inline(is_left ? "├──" : "└──");
        echo(node->get_literal());

        echo_tree(prefix + (is_left ? "│   " : "    "),
                  node->get_left_child(), true);
        echo_tree(prefix + (is_left ? "│   " : "    "),
                  node->get_right_child(), false);
    }
}

void echo_tree(const std::shared_ptr<Node> node)
{
    echo_tree("", node, false);
}