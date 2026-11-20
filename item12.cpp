#include <type_traits>
#include <memory>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <cassert>
#include <tuple>
#include <cstdint>

class Base
{
public:
    virtual void mf1() const {}
    virtual void mf2(int) {}
    virtual void mf3() & {}
    void mf4() const {}
};


// all these overrides are one because difference in parameter type, virtual, const, lvalue/rvalue qualified
#if 0
class Derived : public Base
{
public:
    void mf1() override;
    void mf2(unsigned) override;
    void mf3() && override;
    void mf4() const override;
};
#endif

class Widget
{
public:
    void lvalue_only() & {}
    void rvalue_only() && {}
    void f() & {}
    void f() && {}
};

int main()
{
    Widget w;
    w.lvalue_only();
    // w.rvalue_only(); // errors
    w.f(); // calls f() &

    Widget{}.f(); // calls f() &&, rvalue
    Widget{}.rvalue_only();
    // Widget{}.lvalue_only(); errors
    std::move(w).f(); // calls f() &&
    std::move(w).rvalue_only();
}