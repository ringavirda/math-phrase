#include <iostream>
#include <ranges>
#include <type_traits>

/**
 * This file contains some examples of metaprogramming. It's a compile-time type
 * list that with some common functionality. It is a simple demonstration of how
 * meta can be written in C++. It is unrelated to the interpreter, this file was
 * added simply because it was left here before.
 */

// Pure function.
double add(double arg1, double arg2) { return arg1 + arg2; }
// Generic function.
template <typename T1, typename T2>
auto add(T1 &&arg1, T2 &&arg2) { return arg1 + arg2; }
// Pure metafunction.
template <typename T>
struct if_dereference
{
    using type = T;
    static constexpr bool value = false;
};
template <typename T>
struct if_dereference<T *>
{
    using type = T;
    static constexpr bool value = true;
};
// Metafunction.
template <typename T>
void print(T seq)
{
    if constexpr (std::is_pointer<T>())
        std::cout << *seq << std::endl;
    else
        std::cout << seq << std::endl;
}

// Prefab start

// Runtime stuff.

template <typename T>
void echo(T &&message)
{
    std::cout << std::forward<T>(message) << std::endl;
}

template <typename T0, typename... Ts>
void echo(T0 &&first, Ts &&...rest)
{
    std::cout << std::forward<T0>(first) << ", ";
    echo(std::forward<Ts>(rest)...);
}

template <typename T>
concept linked =
    std::ranges::range<T> && requires(T v) {
        v.front();
        v.pop_front();
        v.back();
        v.pop_back();
        v.empty();
    };

template <typename T, linked L>
bool contains_runtime(T &&filter, L &&list)
{
    if (list.empty())
        return false;
    else
    {
        if (filter == list.front())
            return true;
        else
        {
            list.pop_front();
            return contains_runtime(
                std::forward<T>(filter), std::forward<L>(list));
        }
    }
}

// Meta helpers.

template <typename T>
struct has_type
{
    using type = T;
};

template <bool condition, typename Then, typename Else>
struct _if;

template <typename Then, typename Else>
struct _if<true, Then, Else>
{
    using type = Then;
};
template <typename Then, typename Else>
struct _if<false, Then, Else>
{
    using type = Else;
};

// Type list.

class type_list
{
private:
    // List type.
    template <typename...>
    class _type_list
    {
    };
    // Meta front.
    template <typename L>
    struct front;
    template <typename T0, typename... Ts>
    struct front<_type_list<T0, Ts...>> : has_type<T0>
    {
    };
    // Meta pop front.
    template <typename L>
    struct pop_front;
    template <typename T0, typename... Ts>
    struct pop_front<_type_list<T0, Ts...>> : has_type<_type_list<Ts...>>
    {
    };
    template <typename T>
    struct pop_front : has_type<T>
    {
    };
    // Meta back.
    template <typename L>
    struct back : has_type<typename back<typename pop_front<L>::type>::type>
    {
    };
    template <typename Tn>
    struct back<_type_list<Tn>> : has_type<Tn>
    {
    };
    // Meta push back.
    template <typename L, typename T>
    struct push_back;
    template <typename... Ts, typename T>
    struct push_back<_type_list<Ts...>, T> : has_type<_type_list<Ts..., T>>
    {
    };
    // Meta empty.
    template <typename L>
    struct empty : std::false_type
    {
    };
    template <>
    struct empty<_type_list<>> : std::true_type
    {
    };
    // Meta contains.
    template <typename T, typename L>
    struct contains : _if<
                          empty<L>::value,
                          std::false_type,
                          typename _if<
                              std::is_same_v<T, typename front<L>::type>,
                              std::true_type,
                              contains<T, typename pop_front<L>::type>>::type>::type
    {
    };
    template <typename T>
    struct contains<T, _type_list<>> : std::false_type
    {
    };
    // Meta get.
    template <typename L, size_t index>
    struct get : has_type<typename get<
                     typename pop_front<L>::type, index - 1>::type>
    {
    };
    template <typename L>
    struct get<L, 0> : has_type<typename front<L>::type>
    {
    };

public:
    // Aliases.
    template <typename... Ts>
    using type_list_t = _type_list<Ts...>;
    // Types.
    template <typename L>
    using front_t = typename front<L>::type;
    template <typename L>
    using pop_front_t = typename pop_front<L>::type;
    template <typename L>
    using back_t = typename back<L>::type;
    template <typename L, size_t index>
    using get_t = typename get<L, index>::type;
    // Values.
    template <typename L>
    static constexpr bool empty_v = empty<L>::value;
    template <typename T, typename L>
    static constexpr bool contains_v = contains<T, L>::value;
};