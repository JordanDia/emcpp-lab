#include <type_traits>
#include <vector>
#include <cassert>
#include <atomic>
#include <initializer_list>


enum class ReturnType { Int, VoidPtr };

ReturnType f(int)
{
    return ReturnType::Int;
}

ReturnType f(void*)
{
    return ReturnType::VoidPtr;
}

ReturnType g(int*)
{
    return ReturnType::VoidPtr;
}

template <typename F, typename P>
decltype(auto) call(F func, P p)
{
    return func(p);
}

int main()
{
    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);

    assert(f(0) == ReturnType::Int);
    assert(f(nullptr) == ReturnType::VoidPtr);

    assert(g(0) == ReturnType::VoidPtr); // 0 counts as nullptr
    assert(call(g, nullptr) == ReturnType::VoidPtr);
    call(g, 0); // error: P is int, can't convert int to int*
    call(g, NULL); // same error
}