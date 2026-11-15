#include <type_traits>
#include <cstddef>

template<typename T, typename P>
struct Deduced {
    using T_type = T;
    using Param_type = P;
};

template <typename T>
auto ByRef(T& param) { return Deduced<T, decltype(param)>{}; }

template <typename T>
auto ByConstRef(const T& param) { return Deduced<T, decltype(param)>{}; }

template <typename T>
auto ByPtr(T* param) { return Deduced<T, decltype(param)>{}; }

template <typename T>
auto ByUniversal(T&& param) { return Deduced<T, decltype(param)>{}; }

template <typename T>
auto ByValue(T param) { return Deduced<T, decltype(param)>{}; }

void SomeFunc(int, double) {}

template <typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
    return N;
}

int main()
{
    int x = 27;
    const int cx = x;
    const int& rx = x;
    const int* px = &x;
    const char* const ptr = "Fun with pointers";
    const char name[] = "Jordan Dia";

    // Case 1
    {
        using R = decltype(ByRef(x));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, int&>);
    }
    {
        using R = decltype(ByRef(cx));
        static_assert(std::is_same_v<R::T_type, const int>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }
    {
        using R = decltype(ByRef(rx));
        static_assert(std::is_same_v<R::T_type, const int>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }

    {
        using R = decltype(ByConstRef(x));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }
    {
        using R = decltype(ByConstRef(cx));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }
    {
        using R = decltype(ByConstRef(rx));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }

    {
        using R = decltype(ByPtr(&x));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, int*>);
    }
    {
        using R = decltype(ByPtr(px));
        static_assert(std::is_same_v<R::T_type, const int>);
        static_assert(std::is_same_v<R::Param_type, const int*>);
    }

    // Case 2
    {
        using R = decltype(ByUniversal(x));
        static_assert(std::is_same_v<R::T_type, int&>);
        static_assert(std::is_same_v<R::Param_type, int&>);
    }
    {
        using R = decltype(ByUniversal(cx));
        static_assert(std::is_same_v<R::T_type, const int&>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }
    {
        using R = decltype(ByUniversal(rx));
        static_assert(std::is_same_v<R::T_type, const int&>);
        static_assert(std::is_same_v<R::Param_type, const int&>);
    }
    {
        using R = decltype(ByUniversal(27));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, int&&>);
    }

    // Case 3
    {
        using R = decltype(ByValue(x));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, int>);
    }
    {
        using R = decltype(ByValue(cx));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, int>);
    }
    {
        using R = decltype(ByValue(rx));
        static_assert(std::is_same_v<R::T_type, int>);
        static_assert(std::is_same_v<R::Param_type, int>);
    }
    {
        using R = decltype(ByValue(ptr));
        static_assert(std::is_same_v<R::T_type, const char*>);
        static_assert(std::is_same_v<R::Param_type, const char*>);
    }

    // Arrays and functions

    // when passing by value, functions turn array defs into pointer defs
    {
        using R = decltype(ByValue(name));
        static_assert(std::is_same_v<R::T_type, const char*>);
        static_assert(std::is_same_v<R::Param_type, const char*>);
    }
    // when passing by ref, the array data type doesnt turn into a pointer. Dont forget null termination character
    {
        using R = decltype(ByRef(name));
        static_assert(std::is_same_v<R::T_type, const char[11]>);
        static_assert(std::is_same_v<R::Param_type, const char(&)[11]>);
    }

    {
        using R = decltype(ByValue(SomeFunc));
        static_assert(std::is_same_v<R::T_type, void(*)(int, double)>);
        static_assert(std::is_same_v<R::Param_type, void(*)(int, double)>);
    }
    {
        using R = decltype(ByRef(SomeFunc));
        static_assert(std::is_same_v<R::T_type, void(int, double)>);
        static_assert(std::is_same_v<R::Param_type, void(&)(int, double)>);
    }

    static_assert(arraySize(name) == 11);
}