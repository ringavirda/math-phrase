#pragma once
#include <string>

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

struct Operation
{
public:
    OperationType type;
    double left_operand;
    double right_operand;
};

const Operation interpret_operation(std::string &, const double);

void execute_operation(const Operation &, double &, double &);
