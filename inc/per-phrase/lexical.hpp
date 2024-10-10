#pragma once
#include <string>
#include <vector>

/**
 * Lexical analysis is the first step of interpretation and it enforces the rules
 * about which symbols are allowed and which are not. Separates given stream of
 * characters into something that other stages can work with.
 */

/** @brief Enum that contains known `Lexem` types. */
enum class LexemType
{
    Value,
    Operator,
    PhraseStart,
    PhraseEnd
};

/** @brief Simple struct container for the reconstructed symbol and it's type. */
struct Lexem
{
    std::string symbol;
    LexemType type;
};

/**
 * @brief Performs actual lexical analysis of given `string`.
 * @param literal plain sequence of characters that contains mathematical phrase.
 * @returns Generated vector of `Lexem`. 
 */
std::vector<Lexem> split_lexical(const std::string &literal);
