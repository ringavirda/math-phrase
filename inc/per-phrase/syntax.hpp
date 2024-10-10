#pragma once
#include <memory>
#include <string>
#include <vector>

#include <lexical.hpp>

/**
 * Syntax analysis represents the stage of interpretation where most basic rules
 * of the terminal language. Examples of that may be "braces must always be closed"
 * or "there shouldn't be two sequential operators". 
 */

/** @brief Enum that contains supported types of `Symbol`. */
enum class SymbolType
{
    Sequence,
    Operator,
    Value,
    Null
};

/**
 * @brief Base class for the syntaxic analysis. Encapsulates simple concept of
 * a symbol that may have relatives to the right and to the left. When new `Symbol`
 * is created through default constructor, its type is set to `Null`.
 */
class Symbol
{
public:
    bool is_negated = false;
    SymbolType type;
    std::string value;

    std::shared_ptr<Symbol> left_relative;
    std::shared_ptr<Symbol> right_relative;

    Symbol() { type = SymbolType::Null; }
    Symbol(SymbolType type) : Symbol() { this->type = type; }
    virtual ~Symbol() = default;

    /** @brief Empty virtual function to add class into virtualization table. */
    virtual void poly() {}
};

/**
 * @brief A composite of symbols and sequences, contains `Symbol` as shared
 * pointers to allow polymorphic behavior. Can be iterated over using standard
 * iterator interfaces.
 */
class Sequence : public Symbol
{
private:
    std::vector<std::shared_ptr<Symbol>> symbols;

public:
    Sequence(std::string id) : Symbol(SymbolType::Sequence) { value = id; }
    virtual ~Sequence() = default;

    std::vector<std::shared_ptr<Symbol>>::iterator begin()
    {
        return symbols.begin();
    }
    std::vector<std::shared_ptr<Symbol>>::iterator end()
    {
        return symbols.end();
    }
    std::shared_ptr<Symbol> last() { return symbols.back(); }
    size_t size() { return symbols.size(); }
    void add(std::shared_ptr<Symbol> symbol) { symbols.push_back(symbol); }

    /** @brief Empty virtual function to add class into virtualization table. */
    virtual void poly() {}
};

/**
 * @brief Performs actual syntaxic analysis over a vector of `Lexem`.
 * @param begin iterator to the beginning of the `Lexem` sequence.
 * @param end iterator to the end of the `Lexem` sequence.
 */
Sequence generate_syntaxic(std::vector<Lexem>::iterator begin,
                           std::vector<Lexem>::iterator end);
