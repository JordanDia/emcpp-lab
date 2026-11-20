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

class Entity
{
public:

    Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

};

bool isLucky(int)
{
    return true;
}
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;



bool NotInvocable() = delete;

template<typename T>
void processPointer(T* ptr)
{}

template<>
void processPointer(const void* ptr) = delete;

template<>
void processPointer(const char* ptr) = delete;

int main()
{
    static_assert(!std::is_copy_constructible_v<Entity>);
    static_assert(!std::is_copy_assignable_v<Entity>);

    Entity e;
    
    // copy and assignment are deleted in Entity
    // Entity y(e);
    // Entity z = e;

    isLucky(7);

    // delete functions dont work
    // isLucky('a');
    // isLucky(true);
    // isLucky(3.5); // float prefers to convert to double over int

    const auto callIsLucky = [](auto x) -> decltype(isLucky(x)) { return isLucky(x); };
    static_assert(std::is_invocable_v<decltype(callIsLucky), int>);
    static_assert(!std::is_invocable_v<decltype(callIsLucky), char>);
    static_assert(!std::is_invocable_v<decltype(callIsLucky), bool>);
    static_assert(!std::is_invocable_v<decltype(callIsLucky), double>);
    static_assert(!std::is_invocable_v<decltype(callIsLucky), float>);

    const char* y = "hello";
    bool x = false;
    processPointer(&x);
    // processPointer(&y);

}