#include "operation.hpp"
#include "common.hpp"

void per_operation_mode()
{
    echo(R"""(
Use operation-first notation [ex: + 10] to manipulate data.
Known operations:
    +
    -
    *
    /
To copy current value to clipboard current value press [c].
To paste value from the clipboard use [v].
To exit the interpreter enter [x].
)""");

    double clipboard = 0.0,
           result = 0.0;
    std::string operation;

    while (true)
    {
        echo(std::format("Result: {}\tClipboard: {}", result, clipboard));
        echo_inline("> ");
        std::getline(std::cin, operation);

        auto oper = interpret_operation(operation, clipboard);

        if (oper.type == OperationType::Exit)
        {
            echo_warn("Stopping the interpreter...");
            return;
        }

        execute_operation(oper, result, clipboard);
    }
}
