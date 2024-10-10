#pragma once
#include <memory>
#include <sstream>
#include <string>

#include "analysis-failed.hpp"
#include "operation.hpp"
#include "syntax.hpp"

class Node
{
private:
    OperationType oper;
    std::string literal;
    std::shared_ptr<Node> left_child = nullptr;
    std::shared_ptr<Node> right_child = nullptr;

protected:
    bool is_negated = false;

    Node(OperationType oper, bool is_negated)
    {
        this->oper = oper;
        this->is_negated = is_negated;
        literal = "";
    }

public:
    Node(OperationType oper, std::string literal)
    {
        this->oper = oper;
        this->literal = literal;
    }
    virtual ~Node() = default;

    void negate() { is_negated = true; }
    virtual std::string get_literal()
    {
        return (is_negated) ? "-" + literal : literal;
    }
    virtual double get_result()
    {
        double result = 0;
        switch (oper)
        {
        case OperationType::Add:
            result = left_child->get_result() + right_child->get_result();
            break;
        case OperationType::Sub:
            result = left_child->get_result() - right_child->get_result();
            break;
        case OperationType::Mul:
            result = left_child->get_result() * right_child->get_result();
            break;
        case OperationType::Div:
            result = left_child->get_result() / right_child->get_result();
            break;

        default:
            throw analysis_failed("Collapsing failed!");
            break;
        }
        return (is_negated) ? -result : result;
    }

    std::shared_ptr<Node> get_left_child() { return this->left_child; }
    void set_left_child(std::shared_ptr<Node> child) { this->left_child = child; }

    std::shared_ptr<Node> get_right_child() { return this->right_child; }
    void set_right_child(std::shared_ptr<Node> child)
    {
        if (oper == OperationType::Div && child->get_result() == 0)
            throw analysis_failed("Semantically cannot divide by zero!");
        else
            this->right_child = child;
    }
};

class Leaf : public Node
{
private:
    double result;
    std::string literal;

public:
    Leaf(double value, bool is_negated) : Node(OperationType::Null, is_negated)
    {
        result = value;
    }
    virtual ~Leaf() = default;

    virtual std::string get_literal() override
    {
        std::ostringstream ss;
        if (is_negated)
            ss << "-";
        ss << result;
        return ss.str();
    }

    virtual double get_result() override
    {
        return (is_negated) ? -result : result;
    }
};

class Tree
{
public:
    std::shared_ptr<Node> root;

    double collapse() { return root->get_result(); }
};

Tree construct_semantic(Sequence);
