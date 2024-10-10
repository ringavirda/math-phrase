#include "common.hpp"
#include "echo.hpp"
#include "lexical.hpp"
#include "math-interpreter.hpp"
#include "semantic.hpp"
#include "syntax.hpp"

/**
 * This file contains main entry point and loop for the per-phrase mode of the
 * interpreter. There are mostly user interactions here.
 */

void per_phrase_mode()
{
    echo(R"""(
Use standard notation to enter mathmatical phrases.
Known operations [+ - * /]. Decimal numbers and ( ) are allowed.
Additional commands:
    echo - toggles additional info on and off.
    exit - shuts down the interpreter.
)""");
    // Top level container for the next command or phrase literal.
    std::string literal;
    // Toggles analysis output.
    bool enable_echo = true;

    while (true)
    {
        // Get user input.
        read_input(">", literal);

        if (literal == "echo")
        { // Toggle echo if command given.
            if (enable_echo)
            {
                echo_warn("Turning echo off...");
                enable_echo = false;
            }
            else
            {
                echo_warn("Turning echo on...");
                enable_echo = true;
            }
            continue;
        }
        else if (literal == "exit")
        { // Exit per-phrase mode if command given.
            echo_warn("Stopping the interpreter...");
            return;
        }
        // If math phrase is given then perform the analysis.
        try
        {
            // Lexical stuff.
            auto lexical = split_lexical(literal);
            if (enable_echo)
            {
                echo("Lexical split of given phrase:");
                for (auto &&lexem : lexical)
                    echo_inline(std::format("[{}]", lexem.symbol));
                echo();
            }
            // Syntaxic stuff.
            auto syntaxic = generate_syntaxic(lexical.begin(), lexical.end());
            if (enable_echo)
            {
                echo("Syntaxic split that was generated:");
                echo_sequence(syntaxic);
            }
            // Semantic stuff.
            auto semantic = construct_semantic(syntaxic);
            if (enable_echo)
            {
                echo("Semantic tree was constructed:");
                echo_tree(semantic.root);
            }
            // Print out the collapsed math phrase.
            echo_preface("Collapsed", semantic.collapse());
        }
        catch (const analysis_failed &ex)
        { // Echo any errors thrown during analysis.
            echo_error(ex.what());
        }
    }
}
