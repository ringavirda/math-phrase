#include <ranges>
#include <sstream>

#include "exceptions.hpp"
#include "lexical.hpp"

/** @brief Array of supported operator literals. */
const std::array<char, 6> symbols = {'+', '-', '*', '/', '(', ')'};

std::vector<Lexem> split_lexical(const std::string &literal)
{
    if (literal.empty())
        // Guard against nihility.
        throw analysis_failed("Empty literal was given!");

    std::vector<Lexem> lexical;
    // Create an iterator for the next character.
    auto position = literal.begin();
    // Main analysis loop.
    while (position != literal.end())
    {
        Lexem lexem;
        auto current = *position;

        if (std::isspace(current))
        { // Skip whitespaces.
            position += 1;
            continue;
        }
        if (std::isdigit(current))
        { // Parse possible float number.
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
        { // Process braces and operators.
            lexem.symbol = current;
            if (current == '(')
                lexem.type = LexemType::PhraseStart;
            else if (current == ')')
                lexem.type = LexemType::PhraseEnd;
            else
                lexem.type = LexemType::Operator;
        }
        else
        { // Fail if unknown character is found.
            throw analysis_failed(std::format(
                "Lexical analysis failed due to [{}] at: {}",
                current, std::distance(literal.begin(), position)));
        }
        // Propagate iterator by the size of extracted lexem.
        position += lexem.symbol.size();
        lexical.push_back(lexem);
    }
    // Return a vector of phrases's lexems.
    return lexical;
}