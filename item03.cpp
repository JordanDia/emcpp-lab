#include <type_traits>
#include <deque>
#include <cassert>

struct Widget
{

};


namespace lvalue {

    template<typename Container, typename Index>
    decltype(auto) authAndAccess(Container& c, Index i)
    {
        // authenticate
        return c[i];
    }
    
}

namespace rvalue {

    template<typename Container, typename Index>
    decltype(auto) authAndAccess(Container&& c, Index i)
    {
        // authenticate
        return std::forward<Container>(c)[i];
    }
}

decltype(auto) returnsName()
{
    int x = 5;
    return x;
}

decltype(auto) returnsParens()
{
    int x = 5;
    return (x);
}

int main()
{
    int x = 5;
    static_assert(std::is_same_v<decltype(x), int>);
    static_assert(std::is_same_v<decltype((x)), int&>);

    // lvalue expression
    static_assert(std::is_same_v<decltype((x)), int&>);

    // xvalue expression
    static_assert(std::is_same_v<decltype(std::move(x)), int&&>);

    // prvalue expression
    static_assert(std::is_same_v<decltype(x + 1), int>);

    Widget w;
    const Widget& cw = w;
    auto myWidget1 = cw;
    decltype(auto) myWidget2 = cw;

    static_assert(std::is_same_v<decltype(cw), const Widget&>); // as expected, first case of decltype on a bare name
    static_assert(std::is_same_v<decltype(myWidget1), Widget>); // auto type deduction. auto is T in this case, so case 3 of template type deduction.
    static_assert(std::is_same_v<decltype(myWidget2), const Widget&>); // uses decltype on cw since we are using decltype(auto)


    // authAndAcess lvalue
    {
        std::deque<int> d(10);
        static_assert(std::is_same_v<decltype(lvalue::authAndAccess(d, 5)), int&>);
        lvalue::authAndAccess(d, 5) = 10;
        assert(d[5] == 10);

        // lvalue version rejects rvalues
        // lvalue::authAndAccess(std::deque<int>(10), 5);
    }

    // universal reference accepts both
    {
        std::deque<int> d(10);
        static_assert(std::is_same_v<decltype(rvalue::authAndAccess(d, 5)), int&>);

        // even though container is rvalue, deque::operator[] returns int&,
        // can lead to problems if you store this inside a variable and try to use it since its dangling
        static_assert(std::is_same_v<decltype(rvalue::authAndAccess(std::deque<int>(10), 5)), int&>);

        // copying result from rvalue container is safe
        auto val = rvalue::authAndAccess(std::deque<int>(10), 5); // returns a reference, but auto type deduction in case 3 drops reference, so its just int type
        static_assert(std::is_same_v<decltype(val), int>);
    }

    static_assert(std::is_same_v<decltype(returnsName()), int>);
    static_assert(std::is_same_v<decltype(returnsParens()), int&&>);

}