#pragma once
#include <stdexcept>
#include <string>

/** 
 * @brief Exception local for the math-phrase interpreter. Raised when something
 * goes wrong during different analysis stages.
 */
class analysis_failed : std::runtime_error
{
private:
    std::string message;

public:
    analysis_failed(std::string message) : std::runtime_error(message)
    {
        this->message = message;
    }
    const char *what() const override { return message.c_str(); }
};
