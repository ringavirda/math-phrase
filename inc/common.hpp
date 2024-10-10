#ifndef _FALLEN_COMMON_INCLUDE
#define _FALLEN_COMMON_INCLUDE

#include <iomanip>  // setw
#include <iostream> // cout, cin
#include <limits>   // streamsize
#include <vector>   // vector

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

/**
 * General header for echos. Simplifies work with IO by encapsulating most of the
 * necessary printing functionality into generic functions.
 */

//
// Basic echos.
//

void echo_inline(auto &&message)
{
    std::cout << message << ' ';
}

inline void echo() { std::cout << std::endl; }

void echo(auto &&message)
{
    std::cout << message << std::endl;
}

template <typename T, typename... Ts>
void echo(T &&message, Ts &&...messages)
{
    echo_inline(std::forward<T>(message));
    echo(std::forward<Ts>(messages)...);
}

void echo_preface(auto &&preface, auto &&message)
{
    echo(std::format("{0}: {1}", preface, message));
}

void echo_emphasis(auto &&...messages)
{
#ifdef WIN32
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    echo(std::forward<decltype(messages)>(messages)...);
    SetConsoleTextAttribute(hConsole, 15);
#else
    std::cout << "\x1b[32m";
    echo(std::forward<decltype(messages)>(messages)...);
    std::cout << "\x1b[0m";
#endif
}

void echo_warn(auto &&...messages)
{
#ifdef WIN32
    SetConsoleTextAttribute(hConsole, 6 | FOREGROUND_INTENSITY);
    echo(std::forward<decltype(messages)>(messages)...);
    SetConsoleTextAttribute(hConsole, 15);
#else
    std::cout << "\x1b[33m";
    echo(std::forward<decltype(messages)>(messages)...);
    std::cout << "\x1b[0m";
#endif
}

void echo_error(auto &&...messages)
{
#ifdef WIN32
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    echo(std::forward<decltype(messages)>(messages)...);
    SetConsoleTextAttribute(hConsole, 15);
#else
    std::cout << "\x1b[31m";
    echo(std::forward<decltype(messages)>(messages)...);
    std::cout << "\x1b[0m";
#endif
}

//
// Container echos.
//

template <typename T>
void echo_array(T const *array, const size_t size,
                const size_t part_size = 10, const size_t width = 0)
{
    for (size_t i = 1; i <= size; i++)
    {
        std::cout << std::setw(width) << array[i - 1] << ' ';
        if ((i != 0 && i % part_size == 0) || i == size)
        {
            std::cout << std::endl;
        }
    }
}

template <std::ranges::range TRange>
void echo_range(TRange &&range,
                const size_t part_size = 10, const size_t width = 0)
{
    if (std::distance(range.begin(), range.end()) == 0)
    {
        std::cout << std::endl;
        return;
    }
    auto current = range.begin();
    while (current != range.end())
    {
        std::cout << std::setw(width) << *current << ' ';
        if ((current != range.begin() &&
             std::distance(range.begin(), current) % part_size == 0))
        {
            std::cout << std::endl;
        }
        current++;
    }
    std::cout << std::endl;
}

//
// User interactions.
//

void read_input(const std::string &preface, auto &out)
{
    bool first_fix = true;
    do
    {
        std::cin.clear();
        if (!first_fix)
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');
        first_fix = false;
        std::cout << preface << ' ';
    } while (!(std::cin >> out));
}

template <typename T, typename... Ts>
void read_input(const std::string &preface, T &out, Ts &...rest)
{
    read_input(preface, out);
    if constexpr (sizeof...(Ts) != 0)
        read_input(preface, rest...);
}
#endif