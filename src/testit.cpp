#include <iostream>
#include <concepts>


template <typename T>
concept IInterface = requires (T v)
{
    { auto(T::interface_member) } -> std::same_as<int>;
};

struct some_interface
{
    static constexpr int interface_member = 5;
};

template <IInterface T>
struct wrapper_one
{
    int use_interface()
    {
        return T::interface_member * 5;
    }
};

template <IInterface T>
struct wrapper_two
{
    int use_interface()
    {
        return T::interface_member - 1;
    }
};

// Now lets create some concept to try and tie the wrappers together
template <template <typename> typename T, typename I>
concept IWrapper = requires (T<I> v)
{
    { auto(v.use_interface()) } -> std::same_as<int>;
};

template <template <typename> typename T, typename V>
requires IWrapper<T, V>
auto get_wrapper_val_yuck(T<V> w)
{
    return w.use_interface();
}

int main()
{
    wrapper_one<some_interface> o;
    wrapper_two<some_interface> t;

    std::cout << get_wrapper_val_yuck(o) << "\n";
    std::cout << get_wrapper_val_yuck(t) << "\n";
}
