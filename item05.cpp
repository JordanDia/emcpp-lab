#include <type_traits>
#include <functional>
#include <iostream>
#include <cassert>
#include <unordered_map>

static int g_allocs = 0;

void* operator new(std::size_t n) { ++g_allocs; return std::malloc(n); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

int main()
{
    // auto must be initialized, errors
    // auto x;

    auto x = 5;

    auto l1 = [x](int y){ return 5; };
    auto l2 = [x](int y){ return 5; };


    // two identical lambdas have different closure types
    // compiler gives every lambda its own unique unnamed class type
    static_assert(!std::is_same_v<decltype(l1), decltype(l2)>);

    // auto is not the same as std::function
    static_assert(!std::is_same_v<decltype(l1), std::function<int(int)>>);

    // both print true
    std::cout << (sizeof(l1) == sizeof(int)) << std::endl;
    std::cout << (sizeof(std::function<int(int)>) > sizeof(int)) << std::endl;

    std::array<int, 64> arr;

    auto l3 = [arr](int y){ return 5; };
    assert(g_allocs == 0);

    std::function<int(int)> l4 = [arr](int y){ return 5; };
    assert(g_allocs > 0);

    std::unordered_map<std::string, int> m{{"a", 1}, {"b", 2}};

    for (const std::pair<std::string, int>& p : m)
    {
        assert(static_cast<const void*>(&p) != static_cast<const void*>(&*m.find(p.first)));
    }

    for (const auto& p : m)
    {
        assert(&p == &*m.find(p.first));
    }
}