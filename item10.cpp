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

using UserInfo = std::tuple<std::string, std::string, std::size_t>;

template<typename Enum>
constexpr auto toUType(Enum e)
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

// can be forward declared since underlying type is int by default
enum class Scope;

// must specify underlying type to forward declare unscoped enum
enum Scoped_unscoped : std::uint8_t ;

// you can override default if you want
enum class Scope_8 : std::uint8_t;

int main()
{
    enum Color_unscoped { Red, Blue };
    enum class Color { Red, Blue };


    // unscoped leaked into scope enclosing main
    Color_unscoped red_unscoped = Red;

    // scoped into Color enum
    Color red = Color::Red;

    // Scoped enums do not implicitly convert to int
    // Color blue = 1;
    // static_assert(Color::Red == 0);

    // must static_cast
    Color blue = static_cast<Color>(1);

    static_assert(!std::is_convertible_v<Color, int>);
    static_assert(std::is_convertible_v<Color_unscoped, int>);
    static_assert(std::is_same_v<std::underlying_type_t<Color>, int>);



    // Enums are useful for tuples
    UserInfo uInfo { "Jordan", "j@email.com", 1 };

    // What is 0? Not good to have to remember each index
    auto val = std::get<0>(uInfo);

    // unscope enums implicitly cast to int
    enum UserInfoFields { uiName, uiEmail, uiReputation };
    enum class UserInfoFields_scoped { uiName, uiEmail, uiReputation };

    // readable, using enum class would require a static_cast and is more work
    auto name = std::get<uiName>(uInfo);
    auto name2 = std::get<static_cast<int>(UserInfoFields_scoped::uiName)>(uInfo);
    auto name3 = std::get<toUType(UserInfoFields_scoped::uiName)>(uInfo);

    assert(name3 == std::get<0>(uInfo));


    
}