# emcpp-lab

My worked notes for Scott Meyers' *Effective Modern C++*. Each item has its own file where I check the book's claims in code, instead of just reading them.

Most checks run at compile time. `static_assert` with `<type_traits>` confirms what type the compiler actually deduced, so if a file compiles, its claims hold. Runtime behaviour, such as overload resolution or allocation counts, is checked with `assert`. Code that is meant to fail to compile is left commented out, with a note on why it fails.

## Items

| File | Item |
|---|---|
| `item01.cpp` | 1. Understand template type deduction |
| `item02.cpp` | 2. Understand `auto` type deduction |
| `item03.cpp` | 3. Understand `decltype` |
| `item04.cpp` | 4. Know how to view deduced types |
| `item05.cpp` | 5. Prefer `auto` to explicit type declarations |
| `item06.cpp` | 6. Use the explicitly typed initializer idiom when `auto` deduces undesired types |
| `item07.cpp` | 7. Distinguish between `()` and `{}` when creating objects |
| `item08.cpp` | 8. Prefer `nullptr` to `0` and `NULL` |
| `item09.cpp` | 9. Prefer alias declarations to `typedef`s |
| `item10.cpp` | 10. Prefer scoped `enum`s to unscoped `enum`s |

## Building

Each file is standalone:

```sh
g++ -std=c++17 -Wall item09.cpp -o item09
./item09
```

Tested with MinGW-w64 g++ 15.
