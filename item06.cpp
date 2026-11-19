#include <type_traits>
#include <vector>
#include <cassert>

struct Widget
{};

std::vector<bool> features(const Widget& w)
{
    return std::vector<bool>(5, true);
}

int main()
{
    // showing proxy type exists
    Widget w;

    // bool is different, instead of T& it gives the proxy
    static_assert(std::is_same_v<decltype(features(w)[5]), std::vector<bool>::reference>);
    static_assert(!std::is_same_v<std::vector<bool>::reference, bool>);

    {
        auto highPriority = features(w)[5];
        static_assert(std::is_same_v<decltype(highPriority), std::vector<bool>::reference>);
    }

    // showing proxy points into vector
    std::vector<bool> v(10, true);
    auto r = v[5];       // proxy
    bool b = v[5];       // bool
    v[5] = false;
    assert(r == false);  // since r is a std::vector<bool>::reference, so it updates
    assert(b == true);   // b is a copy, so it doesn't change
    r = true;
    assert(v[5] == true);    // we can also write through the proxy, its a reference

    // explicitly typed initializer idiom
    {
        // cast to a bool, so auto is a bool now
        auto highPriority = static_cast<bool>(features(w)[5]);
        static_assert(std::is_same_v<decltype(highPriority), bool>);
    }
   
}