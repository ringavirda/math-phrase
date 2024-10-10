#include "common.hpp"
#include "math-interpreter.hpp"
#include "operation.hpp"

/**
 * This file contains main entry point and loop for the per-operation mode of the
 * interpreter. There are mostly user interactions here.
 */

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
    // Used to store the buffer and result over different operations.
    double clipboard = 0.0,
           result = 0.0;
    // Top level container for the next operation literal.
    std::string operation;

    while (true)
    {
        // Echo current state.
        echo(std::format("Result: {}\tClipboard: {}", result, clipboard));
        // Parse user input.
        read_input(">", operation);

        // Translate the operation into usable format.
        auto oper = interpret_operation(operation, clipboard);

        if (oper.type == OperationType::Exit)
        { // End loop if exit command was given.
            echo_warn("Stopping the interpreter...");
            return;
        }
        // Actually perform the operation.
        execute_operation(oper, result, clipboard);
    }
}
