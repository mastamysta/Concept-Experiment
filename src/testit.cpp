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


// Phoney wrapper. It has a use_interface() member function, but doesn't require an IInterface as
// a template parameter. This is what we want to catch with the IWrapperNice concept.
struct invalid_wrapper
{
    int use_interface()
    {
        return 0;
    }
};

// We abuse the implicit template instantiation of this function to determine whether a fully qualified
// type has template parameters which are required as specific concepts.
template <template <typename> typename T, IInterface V>
auto contains_interface(T<V> param) { }

// Now lets create some concept to try and tie the wrappers together
template <typename T>
concept IWrapperNice = requires (T v)
{
    { auto(v.use_interface()) } -> std::same_as<int>;

    contains_interface(v);

};

auto get_wrapper_val_nice(IWrapperNice auto w)
{
    return w.use_interface();
}

int main()
{
    wrapper_one<some_interface> o;
    wrapper_two<some_interface> t;

    std::cout << get_wrapper_val_yuck(o) << "\n";
    std::cout << get_wrapper_val_yuck(t) << "\n";

    std::cout << get_wrapper_val_nice(o) << "\n";
    std::cout << get_wrapper_val_nice(t) << "\n";

    // Doesn't compile! :)
    // Furthermore, we get a lovely error message which simply says:
    //      note: the required expression ‘contains_interface(v)’ is invalid
    // So it's super easy to figure out what the problem is: the passed object isn't templated upon an IInterface.
    // Also, our get_wrapper_val() function can use nice implicit template instantiation syntax without an ungainly
    // 'requires' statement.

    // invalid_wrapper i;
    // std::cout << get_wrapper_val_nice(i) << "\n";

}
