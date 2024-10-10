#include "common.hpp"
#include "echo.hpp"
#include "lexical.hpp"
#include "semantic.hpp"
#include "syntax.hpp"

void per_phrase_mode()
{
    echo(R"""(
Use standard notation to enter mathmatical phrases.
Known operations [+ - * /]. Decimal numbers and ( ) are allowed.
Additional commands:
    echo - toggles additional info on and off.
    exit - shuts down the interpreter.
)""");

    std::string literal;
    bool enable_echo = true;

    while (true)
    {
        echo_inline("> ");
        std::getline(std::cin, literal);

        if (literal == "echo")
        {
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
        {
            echo_warn("Stopping the interpreter...");
            return;
        }

        try
        {
            auto lexical = split_lexical(literal);
            if (enable_echo)
            {
                echo("Lexical split of given phrase:");
                for (auto &&lexem : lexical)
                    echo_inline(std::format("[{}]", lexem.symbol));
                echo();
            }

            auto syntaxic = generate_syntaxic(lexical.begin(), lexical.end());
            if (enable_echo)
            {
                echo("Syntaxic split that was generated:");
                echo_sequence(syntaxic);
            }

            auto semantic = construct_semantic(syntaxic);
            if (enable_echo)
            {
                echo("Semantic tree was constructed:");
                echo_tree(semantic.root);
            }

            echo_preface("Collapsed", semantic.collapse());
        }
        catch (const analysis_failed &ex)
        {
            echo_error(ex.what());
        }
    }
}
