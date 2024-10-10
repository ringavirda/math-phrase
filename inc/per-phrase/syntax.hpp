#pragma once
#include <memory>
#include <string>
#include <vector>

#include <lexical.hpp>

enum class SymbolType
{
    Sequence,
    Operator,
    Value,
    Null
};

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

    virtual void poly() {}
};

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

    virtual void poly() {}
};

Sequence generate_syntaxic(std::vector<Lexem>::iterator begin,
                           std::vector<Lexem>::iterator end);
