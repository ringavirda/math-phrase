#include "common.hpp"
#include "operation.hpp"

const Operation interpret_operation(std::string &orig,
                                    const double clip)
{
    orig.erase(
        std::remove(orig.begin(), orig.end(), ' '),
        orig.end());
    Operation oper;

    switch (orig[0])
    {
    case '+':
        oper.type = OperationType::Add;
        break;
    case '-':
        oper.type = OperationType::Sub;
        break;
    case '*':
        oper.type = OperationType::Mul;
        break;
    case '/':
        oper.type = OperationType::Div;
        break;
    case 'c':
        oper.type = OperationType::Copy;
        break;
    case 'x':
        oper.type = OperationType::Exit;
        return oper;

    default:
        echo_error(std::format("[{}] Invalid operation!", orig[0]));
        oper.type = OperationType::Null;
        return oper;
    }

    std::string operand_s = orig.substr(1, orig.size() - 1);
    if (oper.type == OperationType::Copy && operand_s.size() == 0)
    {
        oper.right_operand = 0.0;
        return oper;
    }
    else if (operand_s == "v")
    {
        oper.right_operand = clip;
        return oper;
    }
    else
    {
        try
        {
            oper.right_operand = std::stod(operand_s);
        }
        catch (const std::invalid_argument)
        {
            echo_error(std::format("[{}] - invalid operand!", operand_s));
            oper.type = OperationType::Null;
            return oper;
        }
    }

    return oper;
}

void execute_operation(const Operation &oper,
                       double &result,
                       double &clip)
{
    switch (oper.type)
    {
    case OperationType::Add:
        result += oper.right_operand;
        break;
    case OperationType::Sub:
        result -= oper.right_operand;
        break;
    case OperationType::Mul:
        result *= oper.right_operand;
        break;
    case OperationType::Div:
        if (oper.right_operand == 0.0)
        {
            echo_error("Cannot divide by zero!");
            break;
        }
        result /= oper.right_operand;
        break;

    case OperationType::Copy:
        if (oper.right_operand == 0.0)
            clip = result;
        else
            clip = oper.right_operand;
        break;

    default:
        break;
    }
}