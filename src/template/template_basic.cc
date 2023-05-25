#include <iostream>
#include <cassert>

template <typename  T>
struct Fun_ {using type = T;};

template <>
struct Fun_<int> {using type = unsigned int;};

template <>
struct Fun_<long> {using type = unsigned long;};

template <typename T>
using Fun = typename Fun_<T>::type;

int main() {
    Fun_<int>::type h = 3;
    Fun<int> i = 4;
    assert(typeid(Fun<int>) == typeid(Fun_<int>::type));
    std::cout << h << "\t" << i << std::endl;
}