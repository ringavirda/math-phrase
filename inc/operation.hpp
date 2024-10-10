#pragma once
#include <string>

/**
 * Operations as a concept are used in both per-phrase and per-operation modes.
 * This is a low level class for the interpreter that 
 */

/** @brief Enumeration of known operation types for both work modes. */
enum class OperationType
{
    Add,
    Sub,
    Mul,
    Div,
    Copy,
    Paste,
    Exit,
    Null
};

/**
 * @brief Per-operation mode structure that represents an action for the
 * interpreter. It contains both operands and operation type. 
 */
struct Operation
{
public:
    OperationType type;
    double left_operand;
    double right_operand;
};

/**
 * @brief Translates a sting into `Operation` structure. Can accept previous
 * or memory values.
 * @param orig is a const reference to the string containing the operation.
 * @param clip value from interpreter's memory.
 * @return const `Operation` instance.
 */
const Operation interpret_operation(const std::string &, const double);

/**
 * @brief Performs given `Operation` instance and modify given memory cells with
 * operation results or a memorized value.
 * @param oper `Operation` instance to execute.
 * @param result reference to where calculation results are placed.
 * @param clip reference to interpreters copy buffer.
 */
void execute_operation(const Operation &, double &, double &);
