#include <ranges>
#include <sstream>

#include "analysis-failed.hpp"
#include "lexical.hpp"

const std::array<char, 6> symbols = {'+', '-', '*', '/', '(', ')'};

std::vector<Lexem> split_lexical(const std::string &literal)
{
    if (literal.empty())
        throw analysis_failed("Empty literal was given!");

    std::vector<Lexem> lexical;
    auto position = literal.begin();

    while (position != literal.end())
    {
        Lexem lexem;
        auto current = *position;

        if (std::isspace(current))
        {
            position += 1;
            continue;
        }
        if (std::isdigit(current))
        {
            std::stringstream possible_decimal;
            auto pos = position;

            while (pos != literal.end())
            {
                if (!std::isdigit(*pos) && *pos != '.')
                    break;
                possible_decimal << *pos++;
            }

            lexem.symbol = possible_decimal.str();
            lexem.type = LexemType::Value;
        }
        else if (std::ranges::find(symbols, current) != symbols.end())
        {
            lexem.symbol = current;
            if (current == '(')
                lexem.type = LexemType::PhraseStart;
            else if (current == ')')
                lexem.type = LexemType::PhraseEnd;
            else
                lexem.type = LexemType::Operator;
        }
        else
        {
            throw analysis_failed(std::format(
                "Lexical analysis failed due to [{}] at: {}",
                current, std::distance(literal.begin(), position)));
        }

        position += lexem.symbol.size();
        lexical.push_back(lexem);
    }

    return lexical;
}