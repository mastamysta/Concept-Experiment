#include <concepts>
#include <iostream>
#include <variant>

template <typename T>
concept IFloater = requires (T v)
{
    { auto(v()) } -> std::same_as<float>; 
};

struct a
{
    float operator()() { return 10.5; }
};

struct b
{
    float operator()() { return 21.2; }
};

struct c
{
    float operator()() { return 19.3; }
};

using floater_variant = std::variant<a, b, c>;

auto call_floater_variant(floater_variant v) -> void
{
    std::cout << std::get<0>(v)() << "\n";
}

auto call_floater_concept(IFloater auto v) -> void
{
    std::cout << v() << "\n";
}

int main()
{
    std::cout << "Hello world\n";

    a floater;

    call_floater_concept(floater);

    call_floater_variant(floater);

    b my_b;

    call_floater_variant(my_b);
}

