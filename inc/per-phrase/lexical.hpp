#pragma once
#include <string>
#include <vector>

enum class LexemType
{
    Value,
    Operator,
    PhraseStart,
    PhraseEnd
};

struct Lexem
{
    std::string symbol;
    LexemType type;
};

std::vector<Lexem> split_lexical(const std::string &literal);