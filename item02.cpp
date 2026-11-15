#include <type_traits>
#include <initializer_list>
#include <vector>

void SomeFunc(int, double) {}

template<typename T, typename P>
struct Deduced
{
    using T_type = T;
    using Param_type = P;
};

template<typename T> void func1(T param);
template<typename T> auto func2(std::initializer_list<T> param) { return Deduced<T, decltype(param)>{}; }

// this should error, auto in return type does template type deduction, does not assume std::initializer_list
// auto createInitList() { return { 1, 2, 3 }; }

int main()
{
    // Part 1: auto uses the same 3 cases as Item 1
    int x = 27;
    const int cx = x;
    const int& rx = x;
    const int* px = &x;
    const char* const ptr = "Fun with pointers";
    const char name[] = "Jordan Dia";

    auto        a = 27;
    const auto  b = x;
    const auto& c = x;
    auto&&      d = x;     // universal reference with an lvalue
    auto&&      e = cx;
    auto&&      f = 27;    // universal reference with an rvalue
    auto        g = name;
    auto&       h = name;
    auto        i = SomeFunc;
    auto&       j = SomeFunc;

    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), const int>);
    static_assert(std::is_same_v<decltype(c), const int&>);
    static_assert(std::is_same_v<decltype(d), int&>);
    static_assert(std::is_same_v<decltype(e), const int&>);
    static_assert(std::is_same_v<decltype(f), int&&>);
    static_assert(std::is_same_v<decltype(g), const char*>);
    static_assert(std::is_same_v<decltype(h), const char(&)[11]>);
    static_assert(std::is_same_v<decltype(i), void(*)(int, double)>);
    static_assert(std::is_same_v<decltype(j), void(&)(int, double)>);

    // Part 2: braced initializers, where auto is different
    auto x1 = 27;
    auto x2(27);
    auto x3 = { 27 };
    auto x4{27};
    // auto x5 = { 1, 2, 3.0 }; // expect an error, auto cannot deduce T since multiple types inside initializer list

    static_assert(std::is_same_v<decltype(x1), int>);
    static_assert(std::is_same_v<decltype(x2), int>);
    static_assert(std::is_same_v<decltype(x3), std::initializer_list<int>>);
    static_assert(std::is_same_v<decltype(x4), int>);

    // func1 is supposed to error
    // func1({11, 23, 9}); // template type deductions do not assume that braced initializions are a std::initializer_list, so it errors
    func2({11, 23, 9}); // T is int
    using R = decltype(func2({11, 23, 9}));
    static_assert(std::is_same_v<R::T_type, int>);


    // errors because the compiler does not assume the parameter type is an initalizer_list
    std::vector<int> v;
    auto resetV = [&v](const auto& newValue) { v = newValue; };
    // resetV({1, 2, 3}); 

}