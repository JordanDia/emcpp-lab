#include <type_traits>
#include <memory>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <cassert>

typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS_td;
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;

typedef void (*FuncPtr_td)(int, const std::string&);
using FuncPtr = void(*)(int, const std::string&);

void handler(int, const std::string&);

struct Widget
{
    int Get() const { return 1; }
};

using GetPtr = int (Widget::*)() const;
using IntArrRef = int(&)[10];

// alias template

template<typename T>
using MyAlloc = std::allocator<T>;


template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

// C++98 version
template<typename T>
struct MyAllocList98
{
    typedef std::list<T, MyAlloc<T>> type;
};

// Can't template a typedef
// template<typename T>
// typedef std::list<T, MyAlloc<T>> MyAllocList_td;

template<typename V>
using StringMap = std::map<std::string, V>;

// templated function type
template<typename T>
using Callback = std::function<int(T)>;

// Dependent types and typename
template<typename T>
class Holder
{

private:
    typename MyAllocList98<T>::type list98;
    MyAllocList<T> list;
};

template<typename T>
void useIt()
{
    typename MyAllocList98<T>::type x; // errors without typename, dependent on T, so its a "dependent type"
    MyAllocList<T> list; // no need for typename when using alias
}

using Int = std::remove_reference_t<int&>;


// C++14 style-trait
template<typename T> struct add_ptr { using type = T*; };

template<typename T>
using add_ptr_t = typename add_ptr<T>::type;


// primary template: the gneeral case, must write first
template<typename T>
struct remove_ptr { using type = T; };

template<typename T>
struct remove_ptr<T*> { using type = T; };

template<typename T>
using remove_ptr_t = typename remove_ptr<T>::type;

int main()
{
    static_assert(std::is_same_v<UPtrMapSS_td, UPtrMapSS>);
    static_assert(std::is_same_v<FuncPtr, decltype(&handler)>);

    Widget testWidget;
    GetPtr getFn = &Widget::Get;
    static_assert(std::is_same_v<GetPtr, decltype(&Widget::Get)>);
    (testWidget.*getFn)();
    std::invoke(getFn, testWidget);

    int arr[10];
    IntArrRef r = arr;
    static_assert(std::is_same_v<decltype(r), int(&)[10]>);

    // alias templates
    static_assert(std::is_same_v<MyAllocList<int>, std::list<int, std::allocator<int>>>);

    // modern version
    MyAllocList<Widget> l1;

    // C++98 version
    MyAllocList98<Widget>::type l2;

    static_assert(std::is_same_v<decltype(l1), decltype(l2)>);

    // partial binding, string map
    static_assert(std::is_same_v<StringMap<int>, std::map<std::string, int>>);

    Callback<int> lambda = [](int x) { return x + 1; };
    assert(lambda(1) == 2);

    // C++14 style-traits
    static_assert(std::is_same_v<add_ptr_t<int>, int*>);
    static_assert(std::is_same_v<add_ptr_t<const char>, const char*>);
    static_assert(std::is_same_v<add_ptr_t<int>, add_ptr<int>::type>);

    static_assert(std::is_same_v<remove_ptr_t<int*>, int>);
    static_assert(std::is_same_v<remove_ptr_t<int>, int>);
    static_assert(std::is_same_v<remove_ptr_t<int**>, int*>);
    static_assert(std::is_same_v<remove_ptr_t<int*>, std::remove_pointer_t<int*>>);
}