#include <iostream>
#include <concepts>

template <typename T, typename V>
concept IVable = requires (V v, T t)
{
    { auto(v(t)) } -> std::same_as<V>;
};

// Non-type argument
template <auto v>
struct invokeable_struct
{
    template <typename T>
    requires IVable<T, decltype(v)>
    decltype(v) operator ()(T u) { return v(u); }
};

struct some_vable
{
    some_vable operator()(int i) 
    {
        some_vable v;
        return v; 
    }
};

auto invoke_invokeable(auto invokeable)
{
    return invokeable();
}

int main()
{
    constexpr some_vable v;
    invokeable_struct<v> i;

    some_vable u = invoke_invokeable(i);
}
