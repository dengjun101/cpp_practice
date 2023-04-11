#include <iostream>

class explicit_string1 {
public:
    explicit explicit_string1(char *value) {
        value_ = value;
    }

private:
    char *value_;
};

class explicit_string2 {
public:
    explicit explicit_string2(char *value) {
        value_ = value;
    }

private:
    char *value_;
};

class implicit_string1 {
public:
    implicit_string1(const char *value): value_{value} {
    }

private:
    const char *value_;
};

class implicit_string2 {
public:
    implicit_string2(const char *value): value_{value} {}

private:
    const char *value_;
};

void test_explicit1(explicit_string1 &es1, explicit_string2 &es2) {

}

void test_explicit2(const explicit_string1 &es1, const explicit_string2 &es2) {

}

void test_implicit1(implicit_string1 &es1, implicit_string2 &es2) {

}

void test_implicit2(const implicit_string1 &es1, const implicit_string1 &es2) {

}

void test_implicit2(auto&, auto&) = delete;

void test_auto(const char* a, const char* b) {
    std::cout << "const char* # const char*" << std::endl;
}

void test_auto(char* a, char* b) {
    std::cout << "char* # char*" << std::endl;
}

void test_auto(int a, int b) {
    std::cout << "int # int" << std::endl;
}

void test_auto(const auto& a, const auto& b) {
    std::cout << a << "#" << b << std::endl;
}