#include <iostream>
#include <string>
#include <functional>

template <char ...c>
struct TString {};

template <typename T, T ...c>
constexpr TString<c...> operator"" _s() {
    return TString<c...>();
}

template <char ...l, char ...r>
constexpr TString <l..., r...> operator + (const TString<l...>& left, const TString<r...>& right) {
    return TString<l..., r...>();
}

template <char ...l, char ...r>
constexpr bool operator == (const TString<l...>& left, const TString<r...>& right)
{
    return std::is_same<TString<l...>, TString<r...>>::value;
};

int main() {
    constexpr auto hello = "hello"_s + " world"_s;
    static_assert(hello == "hello world"_s);
    return 0;
}