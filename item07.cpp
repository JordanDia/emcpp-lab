#include <type_traits>
#include <vector>
#include <cassert>
#include <atomic>
#include <initializer_list>


enum class Ctor { Default, Int, IntBool, IntDouble, InitList };
struct Widget
{
    Ctor c;
    Widget() : c(Ctor::Default) {}
    Widget(int i) : c(Ctor::Int) {}
    Widget(int i, bool b) : c(Ctor::IntBool) {}
    Widget(int i, double d) : c(Ctor::IntDouble) {}

    Widget(std::initializer_list<long double> il) : c(Ctor::InitList) {}

    int x;
};

int main()
{
    // three ways of initializing
    int x = 0;
    int y(0);
    int z{ 0 };
    // int z = { 0 };


    Widget w1; // default constructor
    Widget w2 = w1; // copy constructor
    w1 = w2; // assignment, operator=
 
    
    // Uncopyable objects like std::atomic can be initialized using braces or parenthesis
    // but not =

    std::atomic<int> ai1{ 0 };
    std::atomic<int> a21(0);
    std::atomic<int> ai3 = 0; // this will error before C++17
    
    // braced initialization prohibits implicit narrowing conversions

    double a, b, c;
    int sum1 { a + b + c };


    // most vexing parse, anything that can be parsed as a declaration must be interpreted as one

    Widget w3(10);
    Widget w4(); // treated as a function declaration
    static_assert(std::is_same_v<decltype(w4), Widget()>); // same type as a function

    Widget w5{}; // calls constructor no args

    // braced initializer can be bad when using std::initializer list
    // initializer list has highest priority

    Widget w6{10, false}; // doesnt call constructor with (int, bool), calls the constructor with std::initializer_list param type
    assert(w6.c == Ctor::InitList); // init list wins
    Widget w6p(10, false);
    assert(w6p.c == Ctor::IntBool); // int bool ctor wins

    // empty braces is interpreted as no argments, not an empty std::initializer list
    Widget w7{};


    // vectors with braces vs parentheses
    std::vector<int> v1(10, 20);
    assert(v1.size() == 10);

    std::vector<int> v2{ 10, 20 };
    assert(v2.size() == 2);

}