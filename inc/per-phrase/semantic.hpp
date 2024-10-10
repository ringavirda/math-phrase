#pragma once
#include <memory>
#include <sstream>
#include <string>

#include "exceptions.hpp"
#include "operation.hpp"
#include "syntax.hpp"

/**
 * Semantic analysis checks whether user made any bone-headed mistakes, such as
 * division by zero. Also applies common mathmatical rules of operand priority
 * and sequence execution to the syntactically separated phrase.
 */

/**
 * @brief Contains higher level logic for the syntaxic `Symbol`. Adds possibility 
 * of getting a result of the `Node` which is used when collapsing a phrase.
 * Mainly used to house operators.
 */
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

    /** @brief Applies minus sign to this `Node`. */
    void negate() { is_negated = true; }

    /**
     * @brief Virtual method that returns a literal representation of the operator.
     */
    virtual std::string get_literal()
    {
        return (is_negated) ? "-" + literal : literal;
    }

    /**
     * @brief Virtual method used when collapsing phrase. Negated if needed.
     * @returns Result of the operation over both relatives.
     */
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

    /**
     * @brief Getter for the left relative of the `Node`.
     * @returns Current left relative.
     */
    std::shared_ptr<Node> get_left_child() { return this->left_child; }
    /**
     * @brief Setter for the left relative of the `Node`.
     * @param child a shared pointer to the new left relative.
     */
    void set_left_child(std::shared_ptr<Node> child) { this->left_child = child; }

    /** 
     * @brief Getter for the right relative of the `Node`. 
     * @returns Current right relative.
     */
    std::shared_ptr<Node> get_right_child() { return this->right_child; }
    /** 
     * @brief Setter for the right relative of the `Node`. 
     * @param child a shared pointer to the new right relative.
     */
    void set_right_child(std::shared_ptr<Node> child)
    {
        if (oper == OperationType::Div && child->get_result() == 0)
            // Fail if trying to divide by zero.
            throw analysis_failed("Semantically cannot divide by zero!");
        else
            this->right_child = child;
    }
};

/**
 * @brief Container for the stuff branching from operators. Holds data that is
 * used for calculation when collapsing. Also inherits from `Node`.
 */
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

    /** @brief Leaf's override that returns it's literal representation. */
    virtual std::string get_literal() override
    {
        std::ostringstream ss;
        if (is_negated)
            ss << "-";
        ss << result;
        return ss.str();
    }

    /** 
     * @brief Leaf's override of collapsing function that simply returns the
     * value of `this`, negated if needed. 
     */
    virtual double get_result() override
    {
        return (is_negated) ? -result : result;
    }
};

/** 
 * @brief Simple container for root node of the reconstructed phrase tree.
 * Can be collapsed to get the mathematical result of the phrase.
 */
class Tree
{
public:
    std::shared_ptr<Node> root;

    /** @brief Calculates mathematical result of the `Tree`. */
    double collapse() { return root->get_result(); }
};

/**
 * @brief Performs actual semantic analysis of given `Sequence`.
 * @param syntaxic the result of syntaxic analysis.
 * @returns reconstructed `Tree` that can be collapsed.
 */
Tree construct_semantic(Sequence);
